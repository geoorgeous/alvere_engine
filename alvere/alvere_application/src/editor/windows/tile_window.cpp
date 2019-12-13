#include <alvere/utils/assets.hpp>

#include "tile_window.hpp"

TileWindow::TileWindow()
	: m_selectedPosition({ 0, 0 })
	, m_gridSize({ 0, 0 })
	, m_noTilePreview(alvere::Texture::New("res/img/editor/no_tile_preview.png"))
{
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

	m_gridSize = { (int) (contentSize.x / (m_tileSize.x + style.FrameBorderSize * 2.0f))
				 , (int) (contentSize.y / (m_tileSize.y + style.FrameBorderSize * 2.0f)) };

	for (int y = 0; y < m_gridSize[1]; ++y)
	{
		for (int x = 0; x < m_gridSize[0]; ++x)
		{
			DrawTile({ x, y }, m_gridSize);
		}

		ImGui::NewLine();
	}

	ImGui::PopStyleVar(3);

	ImGui::End();
}

void TileWindow::DrawTile(alvere::Vector2i position, alvere::Vector2i gridSize)
{
	auto tileMappingIter = m_tilePositionMapping.find(position);

	EditorTile * tile = tileMappingIter != m_tilePositionMapping.end()
		? tileMappingIter->second.get()
		: nullptr;

	ImGui::SameLine();

	ImGui::PushID(position[0] + position[1] * gridSize[0]);

	if (tile == nullptr)
	{
		DrawInvalidTile(position);
	}
	else
	{
		DrawValidTile(*tile, position);
	}

	//All tiles can be a drop target
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("TILE"))
		{
			IM_ASSERT(payload->DataSize == sizeof(alvere::Vector2i));
			alvere::Vector2i payload_n = *(const alvere::Vector2i *) payload->Data;

			m_tilePositionMapping[position] = std::move(m_tilePositionMapping[payload_n]);
			m_tilePositionMapping.erase(payload_n);
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::PopID();
}

void TileWindow::DrawInvalidTile(alvere::Vector2i position)
{
	int padding = ImGui::GetStyle().FrameBorderSize;

	ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4) GetBorderColor(position));
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(0.2f, 0.2f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(0.4f, 0.4f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(0.5f, 0.5f, 0.5f));

	//The image button does borders differently to normal buttons so we need to draw slightly larger than expected
	if (ImGui::Button("", ImVec2(m_tileSize.x + padding * 2, m_tileSize.y + padding * 2)))
	{
		m_selectedPosition = position;
	}

	ImGui::PopStyleColor(4);

	if (ImGui::BeginPopupContextItem("Tile Context Menu"))
	{
		//Only thing we can do with an invalid tile is give the user the option to make it valid
		if (ImGui::Selectable("New Tile"))
		{
			m_tilePositionMapping[position] = std::make_unique<EditorTile>();
		}

		ImGui::EndPopup();
	}
}

void TileWindow::DrawValidTile(EditorTile & tile, alvere::Vector2i position)
{
	int padding = ImGui::GetStyle().FrameBorderSize;

	ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4) GetBorderColor(position));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding, padding));

	alvere::Texture * previewTexture = tile.GetPreviewTexture();

	//In the case that the tile has not yet had a spritesheet assigned
	if (previewTexture == nullptr)
	{
		previewTexture = m_noTilePreview.get();
	}

	if (ImGui::ImageButton(previewTexture->getHandle(), m_tileSize))
	{
		m_selectedPosition = position;
	}

	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);

	if (ImGui::BeginPopupContextItem("Tile Context Menu"))
	{
		if (ImGui::Selectable("Delete Tile"))
		{
			m_tilePositionMapping.erase(position);
		}

		ImGui::EndPopup();
	}

	//Only buttons with valid tiles can be a move source
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
	{
		ImGui::SetDragDropPayload("TILE", &position, sizeof(alvere::Vector2i));
		ImGui::Image(previewTexture->getHandle(), m_tileSize);
		ImGui::EndDragDropSource();
	}
}

EditorTile * TileWindow::GetSelectedTile()
{
	auto iter = m_tilePositionMapping.find(m_selectedPosition);
	return iter != m_tilePositionMapping.end()
		? iter->second.get()
		: nullptr;
}

Tile & TileWindow::GetOrAddTile(const Tile & tile)
{
	for (auto & mapping : m_tilePositionMapping)
	{
		if (mapping.second->m_tile == tile)
		{
			return mapping.second->m_tile;
		}
	}

	alvere::Vector2i freePosition = GetFirstFreePosition();

	m_tilePositionMapping[freePosition] = std::make_unique<EditorTile>(tile);
	return m_tilePositionMapping[freePosition]->m_tile;
}

alvere::Vector2i TileWindow::GetFirstFreePosition()
{
	for (int y = 0; y < m_gridSize[1]; ++y)
	{
		for (int x = 0; x < m_gridSize[0]; ++x)
		{
			if (m_tilePositionMapping[{ x, y }] == false)
			{
				return { x, y };
			}
		}
	}

	return { 0, 0 };
}