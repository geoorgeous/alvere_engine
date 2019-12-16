#pragma once

#include <string>

void GetFilenameFromPath(const std::string & path, std::string & filename);

void GetFilenameFromPath(const std::string & path, std::string & filename, std::string & extension);

bool HasExtension(const std::string & path);

bool SharesPath(const std::string & rootPath, const std::string & path, std::string & remaining);