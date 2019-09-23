#include <alvere/alvere.hpp>
#include <alvere/assets.hpp>
#include <alvere/graphics/camera.hpp>
#include <alvere/graphics/mesh.hpp>
#include <alvere/graphics/renderer.hpp>
#include <alvere/graphics/sprite_batcher.hpp>
#include <alvere/graphics/text/text_renderer.hpp>
#include <alvere/graphics/texture.hpp>
#include <alvere/math/constants.hpp>
#include <alvere/math/matrix/transformations.hpp>
#include <alvere/utils/file_reader.hpp>
#include <alvere/world/scene.hpp>
#include <alvere/world/entity_components/ec_camera.hpp>
#include <alvere/world/entity_components/ec_rendered_mesh.hpp>

#include "tile_drawer.hpp"
#include "world_cell.hpp"
#include "world_cell_area.hpp"
#include "world_generation.hpp"

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

	Scene scene;
	Camera camera;
	Asset<SpriteBatcher> m_spriteBatcher;
	Renderer * m_renderer;
	EntityHandle entity1;
	EntityHandle entity2;

	TileDrawer m_tileDrawer;
	WorldCellArea * m_worldCellArea;

	TextRenderer m_textRenderer;
	Font::Face * m_fontFace;

	AlvereApplication() : Application(),
		m_tileDrawer("res/img/tilesheet.png")
	{
		m_spriteBatcher = SpriteBatcher::New();

		world_generation::Generate(m_worldCellArea, 0);

		m_vertexShader = Shader::New(Shader::Type::Vertex, file::read("res/shaders/flat_colour.vert"));
		m_fragmentShader = Shader::New(Shader::Type::Fragment, file::read("res/shaders/flat_colour.frag"));
		m_shaderProgram = ShaderProgram::New();
		m_shaderProgram->SetShader(alvere::AssetRef<Shader>(m_vertexShader.get()));
		m_shaderProgram->SetShader(alvere::AssetRef<Shader>(m_fragmentShader.get()));
		m_shaderProgram->build();

		m_texture = Texture::New("res/img/falcon.jpg");

		mesh = new Mesh("res/meshes/teapot2.obj");
		
		material = new Material(m_shaderProgram.get());

		m_materialInstance = new MaterialInstance(material);
		m_materialInstance->get<Shader::DataType::Float3>("u_colour")->m_value = Vector3(0.8f, 0.1f, 0.3f);
		m_materialInstance->get<Shader::DataType::Sampler2D>("u_albedo")->m_value = m_texture.get();

		m_renderer = Renderer::New();

		camera.SetPosition(0, 0, 0);
		//camera.SetPerspective(67.0f * _TAU_DIV_360, 1.0f, 0.01f, 1000.0f);
		camera.SetOrthographic(0, 800, 800, 0, -1.0f, 1.0f);

		/*
		{
			entity1 = scene.createEntity();

			ECCamera * ec_camera = scene.createEntityComponent<ECCamera>(entity1);
			camera = &ec_camera->camera;
			camera->SetPosition(0, 0, 10);
			camera->SetPerspective(67.0f * _TAU_DIV_360, 1.0f, 0.01f, 1000.0f);

			ECRenderedMesh * ec_renderedMesh = scene.createEntityComponent<ECRenderedMesh>(entity1);
			ec_renderedMesh->mesh = mesh;
			ec_renderedMesh->material = m_materialInstance;

			entity1.get()->transform().setScale(Vector3(0.1f));
		}

		{
			entity2 = scene.createEntity();

			ECRenderedMesh * ec_renderedMesh = scene.createEntityComponent<ECRenderedMesh>(entity2);
			ec_renderedMesh->mesh = mesh;
			ec_renderedMesh->material = m_materialInstance;

			scene.setEntityParent(entity2, entity1);

			entity2.get()->transform().setPosition(Vector3(1.0f));
		}
		*/

		m_fontFace = new Font::Face("res/fonts/arial/arial.ttf");
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

		if (m_window->GetKey(Key::W)) velocity += Camera::forward * moveSpeed;
		if (m_window->GetKey(Key::S)) velocity -= Camera::forward * moveSpeed;
		if (m_window->GetKey(Key::D)) velocity += Camera::right * moveSpeed;
		if (m_window->GetKey(Key::A)) velocity -= Camera::right * moveSpeed;
		if (m_window->GetKey(Key::Space)) velocity += Camera::up * moveSpeed;
		if (m_window->GetKey(Key::LeftShift)) velocity -= Camera::up * moveSpeed;

		velocity *= deltaTime;
		camera.Move(velocity * camera.GetRotation());

		if (m_window->GetMouse().GetButton(MouseButton::Left))
		{
			m_window->DisableCursor();

			rotation -= Camera::up * m_window->GetMouse().delta.x * mouseDamper;
			rotation -= Camera::right * m_window->GetMouse().delta.y * mouseDamper;
		}
		else
		{
			m_window->EnableCursor();

			if (m_window->GetKey(Key::Left)) rotation += Camera::up * turnSpeed;
			if (m_window->GetKey(Key::Right)) rotation -= Camera::up * turnSpeed;
			if (m_window->GetKey(Key::Up)) rotation += Camera::right * turnSpeed;
			if (m_window->GetKey(Key::Down)) rotation -= Camera::right * turnSpeed;
		}
		
		if (m_window->GetKey(Key::E)) rotation += Camera::forward * turnSpeed;
		if (m_window->GetKey(Key::Q)) rotation -= Camera::forward * turnSpeed;

		camera.Rotate(Quaternion::fromEulerAngles(rotation * deltaTime));
	}

	void render() override
	{
		m_spriteBatcher->Begin(camera.GetProjectionViewMatrix());
		 
		for (unsigned int x = 0; x < m_worldCellArea->GetWidth(); x++)
		{
			for (unsigned int y = 0; y < m_worldCellArea->GetHeight(); y++)
			{
				WorldCell& cell = m_worldCellArea->At(x, y);
				m_tileDrawer.DrawTile(*m_spriteBatcher, cell.m_x, cell.m_y, cell.m_Type);
			}
		}

		Texture * fontBitmap = m_fontFace->getBitmap(102)->getTexture();

		//m_spriteBatcher->Submit(fontBitmap, Rect(-10, -10, 20, 20 * ((float)fontBitmap->height() / fontBitmap->width())));

		m_textRenderer.drawText(*m_spriteBatcher, m_fontFace, 
			R"(
				Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
				Dolor sed viverra ipsum nunc aliquet bibendum enim. In massa tempor nec feugiat.
				Nunc aliquet bibendum enim facilisis gravida. Nisl nunc mi ipsum faucibus vitae aliquet nec ullamcorper.
				Amet luctus venenatis lectus magna fringilla. Volutpat maecenas volutpat blandit aliquam etiam erat velit scelerisque in.
				Egestas egestas fringilla phasellus faucibus scelerisque eleifend. Sagittis orci a scelerisque purus semper eget duis.
				Nulla pharetra diam sit amet nisl suscipit. Sed adipiscing diam donec adipiscing tristique risus nec feugiat in.
				Fusce ut placerat orci nulla. Pharetra vel turpis nunc eget lorem dolor.
				Tristique senectus et netus et malesuada.)", Vector2(0, 800), 18, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		m_spriteBatcher->end();

		scene.updateSystems(0.0f);
	}
};

ALV_NEW_APP_FUNC(AlvereApplication)