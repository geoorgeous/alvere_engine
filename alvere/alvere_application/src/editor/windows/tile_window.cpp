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

	m_TEMP_tileTextureCollisionOn = alvere::Texture::New("res/img/cobbles/specular.png");
	m_TEMP_tileTextureCollisionOff = alvere::Texture::New("res/img/cobbles/normal.png");
}

void TileWindow::Draw()
{
	ImGuiStyle & style = ImGui::GetStyle();

	ImGui::Begin(GetName().c_str(), &m_visible, m_windowflags);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

	ImVec2 contentSize = ImGui::GetWindowContentRegionMax();
	contentSize.x -= style.WindowPadding.x;
	contentSize.y -= style.WindowPadding.y;

	int xCount = contentSize.x / (m_tileSize.x + style.FrameBorderSize * 2.0f);
	int yCount = contentSize.y / (m_tileSize.y + style.FrameBorderSize * 2.0f);

	for (int y = 0; y < yCount; ++y)
	{
		for (int x = 0; x < xCount; ++x)
		{
			DrawTile({ x, y }, { xCount, yCount });
		}

		ImGui::NewLine();
	}

	ImGui::PopStyleVar(3);

	ImGui::End();
}

void TileWindow::DrawTile(alvere::Vector2i position, alvere::Vector2i gridSize)
{
	auto tileMappingIter = m_tilePositionMapping.find(position);

	Tile * tile = tileMappingIter != m_tilePositionMapping.end()
		? m_tiles.m_tiles[tileMappingIter->second].get()
		: nullptr;

	ImVec4 border = m_selectedPosition == position ? ImColor(1.0f, 1.0f, 0.0) : ImColor(0.0f, 0.0f, 0.0f);

	int padding = ImGui::GetStyle().FrameBorderSize;

	ImGui::SameLine();

	ImGui::PushID(position[0] + position[1] * gridSize[0]);

	ImGui::PushStyleColor(ImGuiCol_Border, border);
	if (tile == nullptr)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0.2f, 0.2f, 0.2f));

		if (ImGui::Button("", ImVec2(m_tileSize.x + padding * 2, m_tileSize.y + padding * 2)))
		{
			m_selectedPosition = position;
		}

		ImGui::PopStyleColor(1);
	}
	else
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding, padding));
		ImTextureID textureID = tile->m_collides ? m_TEMP_tileTextureCollisionOn->getHandle() : m_TEMP_tileTextureCollisionOff->getHandle();
		if (ImGui::ImageButton(textureID, m_tileSize))
		{
			m_selectedPosition = position;
		}
		ImGui::PopStyleVar(1);
	}
	ImGui::PopStyleColor(1);

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