#include <alvere/utils/assets.hpp>

#include "tile_window.hpp"

TileWindow::TileWindow()
	: m_selectedPosition({ 0, 0 })
{
	Spritesheet * airSpritesheet = new Spritesheet(alvere::Texture::New("res/img/tiles/air.png"), alvere::Vector2i{ 1, 1 });
	alvere::Asset<Tile> airTile = std::make_unique<Tile>(Tile{ false, airSpritesheet });

	Spritesheet * wallSpritesheet = new Spritesheet(alvere::Texture::New("res/img/tiles/ground.png"), alvere::Vector2i{ 24, 24 });
	alvere::Asset<Tile> wallTile = std::make_unique<Tile>(Tile{ true, wallSpritesheet });

	m_tiles.m_tiles.emplace_back(std::move(airTile));
	m_tiles.m_tiles.emplace_back(std::move(wallTile));

	m_tilePositionMapping[{ 0, 0 }] = 0;
	m_tilePositionMapping[{ 1, 0 }] = 1;

	m_TEMP_tileTexture = alvere::Texture::New("res/img/cobbles/normal.png");
}

void TileWindow::Draw()
{
	ImGuiStyle & style = ImGui::GetStyle();

	ImGui::Begin(GetName().c_str(), &m_visible, m_windowflags);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

	ImVec2 contentSize = ImGui::GetWindowContentRegionMax();
	contentSize.x -= style.WindowPadding.x;

	int xCount = contentSize.x / m_tileSize.x;
	int yCount = contentSize.y / m_tileSize.y;
	yCount -= 1;

	for (int y = 0; y < yCount; ++y)
	{
		for (int x = 0; x < xCount; ++x)
		{
			DrawTile({ x, y }, { xCount, yCount });
		}

		ImGui::NewLine();
	}

	ImGui::PopStyleVar(2);

	ImGui::End();
}

void TileWindow::DrawMenu()
{
	if (ImGui::BeginMenuBar() == false)
	{
		return;
	}

	if (ImGui::BeginMenu("New Tile"))
	{
		/*if (ImGui::MenuItem("New", NULL, false, false))
		{
			std::cout << "Copied stuff" << std::endl;
		}

		if (ImGui::MenuItem("Open", NULL, false, true))
		{
			alvere::OpenFileDialog openFileDialog("Select a map to open", "", { "*.map" }, false);
			auto result = openFileDialog.Show();

			std::cout << result.first << std::endl;
			if (result.first)
				std::cout << result.second[0] << std::endl;
		}*/

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
}

void TileWindow::DrawTile(alvere::Vector2i position, alvere::Vector2i gridSize)
{
	auto tileMappingIter = m_tilePositionMapping.find(position);

	Tile * tile = tileMappingIter != m_tilePositionMapping.end()
		? m_tiles.m_tiles[tileMappingIter->second].get()
		: nullptr;

	ImVec4 color;
	if (tile == nullptr)
		color = ImColor(0.2f, 0.2f, 0.2f);
	else if (tile->m_collides)
		color = ImColor(1.0f, 0.0f, 0.0f);
	else
		color = ImColor(0.4f, 0.4f, 0.4f);

	ImVec4 border = m_selectedPosition == position ? ImColor(1.0f, 1.0f, 0.0) : ImColor(0.0f, 0.0f, 0.0f);

	ImGui::SameLine();


	ImGui::PushID(position[0] + position[1] * gridSize[0]);

	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
	ImGui::PushStyleColor(ImGuiCol_Border, border);
	ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4) ImColor(0.0f, 0.0f, 0.0f));
	if (ImGui::ImageButton(m_TEMP_tileTexture->getHandle(), m_tileSize))
	{
		m_selectedPosition = position;
	}

	ImGui::PopStyleColor(5);

	if (ImGui::BeginPopupContextItem("item context menu"))
	{
		if (tile == nullptr)
		{
			if (ImGui::Selectable("New Tile"))
			{
				m_tilePositionMapping[position] = m_tiles.m_tiles.size();
				m_tiles.m_tiles.emplace_back(std::make_unique<Tile>());
			}
		}
		else
		{
			if (ImGui::Selectable("Delete Tile"))
			{
				m_tiles.m_tiles.erase(m_tiles.m_tiles.begin() + tileMappingIter->second);
				m_tilePositionMapping.erase(position);
			}
		}

		ImGui::EndPopup();
	}

	ImGui::PopID();
}