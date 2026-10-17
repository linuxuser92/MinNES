#include "NsfeLoader.h"

#include "Cartridge.h"

std::shared_ptr<Cartridge> NsfeLoader::Load(const std::string& path) {
	return Cartridge::Load(path);
}
