#pragma once

#include "Cartridge.h"
#include <memory>
#include <string>

class Loader {
public:
	static std::shared_ptr<Cartridge> LoadRom(const std::string& path);
	static bool IsKnownRomType(const std::string& path);
};
