#pragma once

#include <memory>
#include <fstream>

namespace alvere
{
	class Window;
}

class EditorWorld;
class C_Tilemap;
class ImGuiEditor;

class WorldImporter
{
	ImGuiEditor & m_editor;
	alvere::Window & m_window;

public:

	WorldImporter(ImGuiEditor & editor, alvere::Window & window);

	std::unique_ptr<EditorWorld> operator()(const std::string & filepath);

private:

	void ImportTilemap(std::fstream & file, C_Tilemap & tilemap);
};