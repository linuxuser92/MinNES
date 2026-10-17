#include "ROMLoader.h"

#include "Cartridge.h"
#include "FdsLoader.h"
#include "Loader.h"
#include "NsfLoader.h"
#include "NsfeLoader.h"

std::shared_ptr<Cartridge> ROMLoader::Load(const std::string& path) {
	const std::string lower = path;
	if (lower.size() >= 4 && lower.substr(lower.size() - 4) == ".fds") {
		return FdsLoader::Load(path);
	}
	if (lower.size() >= 4 && lower.substr(lower.size() - 4) == ".nsf") {
		return NsfLoader::Load(path);
	}
	if (lower.size() >= 5 && lower.substr(lower.size() - 5) == ".nsfe") {
		return NsfeLoader::Load(path);
	}
	return Loader::LoadRom(path);
}

bool ROMLoader::IsSupportedFormat(const std::string& path) {
	const std::string lower = path;
	return (lower.size() >= 4 && lower.substr(lower.size() - 4) == ".nes") ||
		   (lower.size() >= 4 && lower.substr(lower.size() - 4) == ".fds") ||
		   (lower.size() >= 4 && lower.substr(lower.size() - 4) == ".nsf") ||
		   (lower.size() >= 5 && lower.substr(lower.size() - 5) == ".nsfe");
}
