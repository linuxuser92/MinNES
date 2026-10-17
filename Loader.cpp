#include "Loader.h"

#include "Cartridge.h"

std::shared_ptr<Cartridge> Loader::LoadRom(const std::string& path) {
	return Cartridge::Load(path);
}

bool Loader::IsKnownRomType(const std::string& path) {
	const std::string lower = path;
	return lower.size() >= 4 && (
		lower.substr(lower.size() - 4) == ".nes" ||
		lower.substr(lower.size() - 4) == ".fds");
}
