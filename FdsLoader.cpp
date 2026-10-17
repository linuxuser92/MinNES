#include "FdsLoader.h"

#include "Cartridge.h"

std::shared_ptr<Cartridge> FdsLoader::Load(const std::string& path) {
	return Cartridge::Load(path);
}
