#include "Cartridge.h"
#include <fstream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace {
size_t RomSize(uint8_t lsb, uint8_t msbNibble, size_t unit, bool nes2) {
	if (!nes2 || msbNibble != 0x0F) {
		const uint64_t count = static_cast<uint64_t>(lsb) | (static_cast<uint64_t>(msbNibble) << 8);
		if (count > std::numeric_limits<size_t>::max() / unit) throw std::runtime_error("ROM is too large");
		return static_cast<size_t>(count) * unit;
	}

	// NES 2.0 uses the value in byte 4/5 as E.M.M.M.M.M.M: 2^E * (2*M+1).
	const unsigned exponent = lsb >> 2;
	const uint64_t multiplier = 2 * (lsb & 3) + 1;
	if (exponent >= 63) throw std::runtime_error("ROM size is too large");
	const uint64_t count = (uint64_t{1} << exponent) * multiplier;
	if (count > std::numeric_limits<size_t>::max() / unit) throw std::runtime_error("ROM is too large");
	return static_cast<size_t>(count) * unit;
}
}

std::shared_ptr<Cartridge> Cartridge::Load(const std::string& path) {
	std::ifstream file(path, std::ios::binary); if (!file) throw std::runtime_error("Cannot open ROM: " + path);
	std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), {});
	if (data.size() < 16 || data[0] != 'N' || data[1] != 'E' || data[2] != 'S' || data[3] != 0x1A) throw std::runtime_error("Invalid iNES ROM");
	const uint8_t flags6 = data[6], flags7 = data[7];
	const bool nes2 = (flags7 & 0x0C) == 0x08;
	const size_t prgSize = RomSize(data[4], nes2 ? data[9] & 0x0F : 0, 16384, nes2);
	const size_t chrSize = RomSize(data[5], nes2 ? data[9] >> 4 : 0, 8192, nes2);
	const size_t trainer = (flags6 & 4) ? 512 : 0, start = 16 + trainer;
	if (data.size() < start + prgSize + chrSize) throw std::runtime_error("Truncated ROM");
	std::vector<uint8_t> prg(data.begin() + start, data.begin() + start + prgSize);
	std::vector<uint8_t> chr(data.begin() + start + prgSize, data.begin() + start + prgSize + chrSize);
	auto cartridge = std::make_shared<Cartridge>();
	cartridge->mapperNumber_ = static_cast<uint16_t>((flags6 >> 4) | (flags7 & 0xF0));
	if (nes2) cartridge->mapperNumber_ |= static_cast<uint16_t>((data[8] & 0x0F) << 8);
	const Mirroring mirroring = (flags6 & 8) ? Mirroring::FourScreen : ((flags6 & 1) ? Mirroring::Vertical : Mirroring::Horizontal);
	cartridge->mapper_ = CreateMapper(cartridge->mapperNumber_, std::move(prg), std::move(chr), mirroring); return cartridge;
}
