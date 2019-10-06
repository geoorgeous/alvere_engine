#include <alvere/alvere.hpp>
#include <alvere/assets.hpp>
#include <alvere/graphics/camera.hpp>
#include <alvere/graphics/mesh.hpp>
#include <alvere/graphics/renderer.hpp>
#include <alvere/graphics/sprite_batcher.hpp>
#include <alvere/graphics/texture.hpp>
#include <alvere/math/constants.hpp>
#include <alvere/math/matrix/transformations.hpp>
#include <alvere/utils/file_reader.hpp>
#include <alvere/world/world.hpp>
#include <alvere/world/ecs_testing.hpp>
#include <alvere/utils/exceptions.hpp>
#include <alvere/world/system/systems/renderer_system.hpp>

#include "tile_drawer.hpp"
#include "world_cell.hpp"
#include "world_cell_area.hpp"
#include "world_generation.hpp"

/* todo
 *
 * camera component
 * mover/controller component+systems?
 * rich text formatting
 * command console work
 * command console argument parsing
 */

using namespace alvere;

struct AlvereApplication : public Application
{
	Asset<Shader> m_vertexShader;
	Asset<Shader> m_fragmentShader;
	Asset<ShaderProgram> m_shaderProgram;
	Asset<Texture> m_texture;
	Mesh * mesh;
	Material * material;
	MaterialInstance * m_materialInstance;

	World world;
	Camera sceneCamera;
	Camera uiCamera;
	Asset<SpriteBatcher> m_spriteBatcher;
	Renderer * m_renderer;

	TileDrawer m_tileDrawer;
	WorldCellArea * m_worldCellArea;

	Font::Face * m_fontFace;

	AlvereApplication() : Application(),
		m_tileDrawer("res/img/tilesheet.png")
	{
		RunTests();

		AlvAssert(false, "grrr failed an assert");

		m_spriteBatcher = SpriteBatcher::New();

		world_generation::Generate(m_worldCellArea, 0);

		m_fontFace = new Font::Face("res/fonts/arial/arial.ttf");

		m_vertexShader = Shader::New(Shader::Type::Vertex, file::read("res/shaders/flat_colour.vert"));
		m_fragmentShader = Shader::New(Shader::Type::Fragment, file::read("res/shaders/flat_colour.frag"));
		m_shaderProgram = ShaderProgram::New();
		m_shaderProgram->SetShader(alvere::AssetRef<Shader>(m_vertexShader.get()));
		m_shaderProgram->SetShader(alvere::AssetRef<Shader>(m_fragmentShader.get()));
		m_shaderProgram->build();

		m_texture = Texture::New("res/img/test.png");

		mesh = new Mesh("res/meshes/teapot2.obj");
		
		material = new Material(m_shaderProgram.get());

		m_materialInstance = new MaterialInstance(material);
		m_materialInstance->get<Shader::DataType::Float3>("u_colour")->m_value = Vector3(0.8f, 0.1f, 0.3f);
		m_materialInstance->get<Shader::DataType::Sampler2D>("u_albedo")->m_value = m_texture.get();

		sceneCamera.SetPosition(0, 0, 10);
		sceneCamera.SetPerspective(67.0f * _TAU_DIV_360, 1.0f, 0.01f, 1000.0f);

		uiCamera.SetOrthographic(0, 800, 800, 0, -1.0f, 1.0f);

		m_renderer = Renderer::New();

		EntityHandle entity = world.SpawnEntity<C_Transform, C_RenderableMesh>();
		C_Transform & transform = world.GetComponent<C_Transform>(entity);
		transform->setScale(Vector3(0.05f, 0.05f, 0.05f));
		C_RenderableMesh & renderableMesh = world.GetComponent<C_RenderableMesh>(entity);
		renderableMesh.m_material = m_materialInstance;
		renderableMesh.m_mesh = mesh;

		world.AddSystem<RendererSystem>( *m_renderer );
	}

	~AlvereApplication()
	{
		delete m_worldCellArea;
		delete mesh;
		delete material;
		delete m_materialInstance;
		delete m_renderer;
		delete m_fontFace;
	}

	void update(float deltaTime) override
	{
		Vector3 velocity;
		Vector3 rotation;

		const float moveSpeed = 4.0f;
		const float turnSpeed = 2.0f;
		const float mouseDamper = 0.1f;

		if (m_window->getKey(Key::W)) velocity += Camera::forward * moveSpeed;
		if (m_window->getKey(Key::S)) velocity -= Camera::forward * moveSpeed;
		if (m_window->getKey(Key::D)) velocity += Camera::right * moveSpeed;
		if (m_window->getKey(Key::A)) velocity -= Camera::right * moveSpeed;
		if (m_window->getKey(Key::Space)) velocity += Camera::up * moveSpeed;
		if (m_window->getKey(Key::LeftShift)) velocity -= Camera::up * moveSpeed;

		velocity *= deltaTime;
		sceneCamera.Move(velocity * sceneCamera.GetRotation());

		if (m_window->getMouse().getButton(MouseButton::Left))
		{
			m_window->disableCursor();

			rotation -= Camera::up * m_window->getMouse().delta.x * mouseDamper;
			rotation -= Camera::right * m_window->getMouse().delta.y * mouseDamper;
		}
		else
		{
			m_window->enableCursor();

			if (m_window->getKey(Key::Left)) rotation += Camera::up * turnSpeed;
			if (m_window->getKey(Key::Right)) rotation -= Camera::up * turnSpeed;
			if (m_window->getKey(Key::Up)) rotation += Camera::right * turnSpeed;
			if (m_window->getKey(Key::Down)) rotation -= Camera::right * turnSpeed;
		}
		
		if (m_window->getKey(Key::E)) rotation += Camera::forward * turnSpeed;
		if (m_window->getKey(Key::Q)) rotation -= Camera::forward * turnSpeed;

		sceneCamera.Rotate(Quaternion::fromEulerAngles(rotation * deltaTime));

		world.Update(deltaTime);
	}

	void render() override
	{
		m_spriteBatcher->begin(sceneCamera.GetProjectionViewMatrix());
		 
		for (unsigned int x = 0; x < m_worldCellArea->GetWidth(); x++)
		{
			for (unsigned int y = 0; y < m_worldCellArea->GetHeight(); y++)
			{
				WorldCell& cell = m_worldCellArea->At(x, y);
				m_tileDrawer.DrawTile(*m_spriteBatcher, cell.m_x, cell.m_y, cell.m_Type);
			}
		}

		m_spriteBatcher->end();

		m_renderer->begin(sceneCamera);

		world.Render();

		m_renderer->end();

		/*m_spriteBatcher->begin(uiCamera.GetProjectionViewMatrix());

		std::string cameraPositionString = "x: " + std::to_string(sceneCamera.GetPosition().x) + "\ny: " + std::to_string(sceneCamera.GetPosition().y) + "\nz: " + std::to_string(sceneCamera.GetPosition().z);

		m_spriteBatcher->submit(*m_fontFace->getBitmap(18), cameraPositionString, Vector2(0, 800 - 18), Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		m_spriteBatcher->end();*/
	}
};

ALV_NEW_APP_FUNC(AlvereApplication)