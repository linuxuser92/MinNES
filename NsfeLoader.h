#pragma once

#include "Cartridge.h"
#include <memory>
#include <string>

class NsfeLoader {
public:
	static std::shared_ptr<Cartridge> Load(const std::string& path);
};
