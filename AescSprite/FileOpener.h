#pragma once

#include <string>

class FileOpener
{
public:
	// Returns file path.
	static std::string OpenFile();
	static std::string SaveFile();
};