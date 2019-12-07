#include <alvere\world\archetype\archetype_query.hpp>
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_camera.hpp>
#include <alvere\application\window.hpp>

#include "pan_tool.hpp"
#include "editor/imgui_editor.hpp"

PanTool::PanTool(ImGuiEditor & editor, alvere::Window & window)
	: m_editor(editor)
	, m_window(window)
	, m_leftMouse(window, alvere::MouseButton::Left)
{
}

void PanTool::Update(float deltaTime)
{
	m_leftMouse.Update();

	if (m_leftMouse.IsDown() == false)
	{
		return;
	}

	alvere::Archetype::Query cameraQuery;
	cameraQuery.Include<alvere::C_Transform>();
	cameraQuery.Include<alvere::C_Camera>();

	std::vector<std::reference_wrapper<alvere::Archetype>> cameras;
	m_editor.GetFocusedWorld()->m_world.QueryArchetypes(cameraQuery, cameras);
	alvere::C_Transform & cameraTransform = *cameras[0].get().GetProvider<alvere::C_Transform>().begin();
	alvere::C_Camera & camera = *cameras[0].get().GetProvider<alvere::C_Camera>().begin();

	alvere::Vector2 newMousePos = m_window.getMouse().position;

	//First frame do not try to move the camera as we have no previous mouse position
	if (m_leftMouse.IsPressed() == false)
	{
		alvere::Vector3 mouseDifference = camera.ScreenToWorld(newMousePos - m_mousePosition);
		cameraTransform->move({ -mouseDifference.x, mouseDifference.y, 0 });
	}

	m_mousePosition = newMousePos;
}

void PanTool::Render()
{
}