#pragma once

#include "NesTypes.h"

#include <array>
#include <cstdint>

namespace Nes {
class Apu {
public:
	Apu() = default;

	void Reset();
	void Step();
	void SetMuted(bool muted) { muted_ = muted; }
	bool IsMuted() const { return muted_; }

	void WriteRegister(std::uint16_t address, Byte value);
	Byte ReadRegister(std::uint16_t address) const;

private:
	std::array<Byte, 0x20> registers_{};
	bool muted_{false};
	std::uint32_t cycle_{};
};
}
