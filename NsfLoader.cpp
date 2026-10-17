#include "NsfLoader.h"

#include "Cartridge.h"

std::shared_ptr<Cartridge> NsfLoader::Load(const std::string& path) {
	return Cartridge::Load(path);
}
