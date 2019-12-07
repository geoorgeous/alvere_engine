#include "tile_properties_window.hpp"

TilePropertiesWindow::TilePropertiesWindow(TileWindow & tileWindow)
	: m_tileWindow(tileWindow)
{
}

void TilePropertiesWindow::Draw()
{
	ImGui::Begin(GetName().c_str(), &m_visible, m_windowflags);

	Tile * tile = m_tileWindow.GetSelectedTile();

	if (tile == nullptr)
	{
		ImGui::End();
		return;
	}

	ImGui::Checkbox("Collides", &tile->m_collides);

	ImGui::End();
}