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

	template<typename T>
	void Write(std::fstream & file, const T & value) const;
};

template<typename T>
void WorldExporter::Write(std::fstream & file, const T & value) const
{
	file.write(reinterpret_cast<const char *>(&value), sizeof(value));
}