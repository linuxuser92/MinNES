#include "iNesLoader.h"

#include "Cartridge.h"

std::shared_ptr<Cartridge> iNesLoader::Load(const std::string& path) {
	return Cartridge::Load(path);
}
