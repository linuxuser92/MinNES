#pragma once

#include "Cartridge.h"
#include "CPU.h"
#include "NesTypes.h"
#include "PPU.h"

#include <array>
#include <memory>

namespace Nes {
class NesConsole {
public:
	explicit NesConsole(std::shared_ptr<Cartridge> cartridge);

	void Reset();
	void Frame();

	std::shared_ptr<Cartridge> Cartridge() const { return cartridge_; }
	const std::array<uint32_t, 256 * 240>& FrameBuffer() const { return ppu_.Frame(); }

private:
	std::shared_ptr<Cartridge> cartridge_;
	CPU cpu_;
	PPU ppu_;
	std::array<Nes::Byte, 0x800> ram_{};
	ConsoleConfig config_{};
};
}
