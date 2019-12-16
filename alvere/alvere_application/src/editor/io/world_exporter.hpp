#pragma once

#include <fstream>

class EditorWorld;
class C_Tilemap;

class WorldExporter
{
public:

	void operator()(const std::string & filepath, const EditorWorld & world);

private:

	void ExportTilemap(std::fstream & file, const C_Tilemap & tilemap);

};