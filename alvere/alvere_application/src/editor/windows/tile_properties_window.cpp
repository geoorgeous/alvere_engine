#include <memory>

#include "tile_properties_window.hpp"
#include "imgui/imgui_internal.h"
#include "dialogs/open_file_dialog.hpp"

TilePropertiesWindow::TilePropertiesWindow(TileWindow & tileWindow)
	: m_tileWindow(tileWindow)
{
}

void TilePropertiesWindow::Draw()
{
	ImGui::Begin(GetName().c_str(), &m_visible, m_windowflags);

	EditorTile * editorTile = m_tileWindow.GetSelectedTile();

	if (editorTile == nullptr)
	{
		ImGui::End();
		return;
	}

	Tile * tile = &editorTile->m_tile;
	ImGuiStyle & style = ImGui::GetStyle();

	const int textWidth = 80.0f;

	std::string textureString = tile->m_spritesheet ? editorTile->m_texturePath : "None";

	style.ButtonTextAlign = { 0.0f, 0.5f };

	ImGui::TextEx("Texture");
	ImGui::SameLine(textWidth, style.ItemInnerSpacing.x);
	if (ImGui::Button(textureString.c_str(), { ImGui::CalcItemWidth(), 0 }))
	{
		UserSetTileTexture(*editorTile);
	}

	ImGui::TextEx("Collides");
	ImGui::SameLine(textWidth, style.ItemInnerSpacing.x);
	ImGui::Checkbox("", &tile->m_collides);

	ImGui::End();
}

void TilePropertiesWindow::UserSetTileTexture(EditorTile & tile)
{
	alvere::OpenFileDialog openFile("Select a valid spritesheet", "", { "*.PNG" }, false);

	std::pair<bool, std::vector<std::string>> output = openFile.Show();
	if (output.first == false || output.second.size() == 0)
	{
		return;
	}

	tile.m_texturePath = output.second[0];

	alvere::Asset<alvere::Texture> texture = alvere::Texture::New(output.second[0].c_str());
	tile.m_tile.m_spritesheet.reset(new Spritesheet{ std::move(texture), { 24, 24 } });
}