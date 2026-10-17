#pragma once

#include "Cartridge.h"
#include <memory>
#include <string>

class FdsLoader {
public:
	static std::shared_ptr<Cartridge> Load(const std::string& path);
};
