#include "APU.h"

namespace Nes {

void Apu::Reset() {
	registers_.fill(0);
	muted_ = false;
	cycle_ = 0;
}

void Apu::Step() {
	++cycle_;
}

void Apu::WriteRegister(std::uint16_t address, Byte value) {
	if (address >= 0x4000) return;
	if (address >= 0x4000) return;

	const std::size_t index = static_cast<std::size_t>(address & 0x1F);
	registers_[index] = value;
}

Byte Apu::ReadRegister(std::uint16_t address) const {
	if (address >= 0x4000) return 0;
	const std::size_t index = static_cast<std::size_t>(address & 0x1F);
	return registers_[index];
}

}
