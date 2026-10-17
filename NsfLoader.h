#pragma once

#include "Cartridge.h"
#include <memory>
#include <string>

class NsfLoader {
public:
	static std::shared_ptr<Cartridge> Load(const std::string& path);
};
