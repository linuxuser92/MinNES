#pragma once

#include "Cartridge.h"
#include <memory>
#include <string>

class ROMLoader {
public:
	static std::shared_ptr<Cartridge> Load(const std::string& path);
	static bool IsSupportedFormat(const std::string& path);
};
