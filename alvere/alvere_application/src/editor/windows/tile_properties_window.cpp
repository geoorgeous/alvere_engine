#include <memory>

#include "tile_properties_window.hpp"
#include "imgui/imgui_internal.h"
#include "dialogs/open_file_dialog.hpp"
#include "editor/utils/path_utils.hpp"

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

	std::string textureString = tile->m_spritesheet.m_texture != nullptr ? editorTile->m_texturePath : "None";

	style.ButtonTextAlign = { 0.0f, 0.5f };

	ImGui::TextEx("Texture");
	ImGui::SameLine(textWidth, style.ItemInnerSpacing.x);

	std::string filename;
	GetFilenameFromPath(textureString, filename);
	if (ImGui::Button(filename.c_str(), { ImGui::CalcItemWidth(), 0 }))
	{
		UserSetTileTexture(*editorTile);
	}

	std::unique_ptr<alvere::Texture> & texture = editorTile->m_tile.m_spritesheet.m_texture;
	if (texture && ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Image(texture->getHandle(), { (float)texture->width(), (float)texture->height() });
		ImGui::EndTooltip();
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

	std::unique_ptr<alvere::Texture> texture = alvere::Texture::New(output.second[0].c_str());
	tile.m_tile.m_spritesheet = { std::move(texture), { 24, 24 } };
}