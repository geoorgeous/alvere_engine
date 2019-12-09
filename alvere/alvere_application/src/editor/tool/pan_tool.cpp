#include <algorithm>

#include <alvere\application\window.hpp>
#include <alvere/math/matrix/transformations.hpp>
#include <alvere\world\archetype\archetype_query.hpp>
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_camera.hpp>

#include "editor/tool/pan_tool.hpp"
#include "editor/imgui_editor.hpp"
#include "editor/editor_world.hpp"

PanTool::PanTool(ImGuiEditor & editor, alvere::Window & window)
	: m_editor(editor)
	, m_window(window)
	, m_leftMouse(window, alvere::MouseButton::Left)
{
}

void PanTool::Update(float deltaTime)
{
	m_leftMouse.Update();

	EditorWorld * focusedWorld = m_editor.GetFocusedWorld();
	if (focusedWorld == nullptr)
	{
		return;
	}

	UpdateZoom(*focusedWorld);

	if (m_leftMouse.IsDown())
	{
		UpdatePan(*focusedWorld);
	}
}

void PanTool::UpdatePan(EditorWorld & focusedWorld)
{
	alvere::Archetype::Query cameraQuery;
	cameraQuery.Include<alvere::C_Transform>();
	cameraQuery.Include<alvere::C_Camera>();

	std::vector<std::reference_wrapper<alvere::Archetype>> cameras;
	focusedWorld.m_world.QueryArchetypes(cameraQuery, cameras);
	alvere::C_Transform & cameraTransform = *cameras[0].get().GetProvider<alvere::C_Transform>().begin();
	alvere::C_Camera & camera = *cameras[0].get().GetProvider<alvere::C_Camera>().begin();

	alvere::Vector2 newMousePos = m_window.getMouse().position;

	//First frame do not try to move the camera as we have no previous mouse position
	if (m_leftMouse.IsPressed() == false)
	{
		alvere::Vector3 mousePosWorld = camera.screenToWorld(m_mousePosition, m_window.getWidth(), m_window.getHeight());
		alvere::Vector3 newMousePosWorld = camera.screenToWorld(newMousePos, m_window.getWidth(), m_window.getHeight());
		cameraTransform->move(mousePosWorld - newMousePosWorld);
	}

	m_mousePosition = newMousePos;
}

void PanTool::UpdateZoom(EditorWorld & focusedWorld)
{
	float scroll = m_window.getMouse().scrollDelta.y;
	if (scroll == 0.0f)
	{
		return;
	}

	float aspect = (float) m_window.getProperties().resHeight / m_window.getProperties().resWidth;
	float currentScale = focusedWorld.m_camera->getOrthographicsScale() / 2.0f;
	float newScale = currentScale - scroll;

	newScale = std::max(0.1f, newScale);

	focusedWorld.m_camera->setOrthographic(-newScale, newScale, newScale * aspect, -newScale * aspect, -1.0f, 1.0f);
}