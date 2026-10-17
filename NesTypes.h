#pragma once

#include <cstddef>
#include <cstdint>

namespace Nes {
using Byte = std::uint8_t;
using Word = std::uint16_t;
using DWord = std::uint32_t;

constexpr Byte kZero = 0;
constexpr Byte kOne = 1;
constexpr Word kMemoryPageSize = 0x100;
constexpr Word kCpuAddressSpace = 0x10000;
constexpr Word kPpuAddressSpace = 0x4000;

enum class MirroringMode : std::uint8_t {
	Horizontal,
	Vertical,
	SingleScreenLower,
	SingleScreenUpper,
	FourScreen
};

enum class InterruptKind : std::uint8_t {
	None,
	Nmi,
	Irq,
	Reset
};

struct CpuState {
	Byte accumulator{};
	Byte xIndex{};
	Byte yIndex{};
	Byte stackPointer{};
	Byte status{};
	Word programCounter{};
};

struct ConsoleConfig {
	bool enableAudio{true};
	bool expandedRam{false};
	bool enableFrameLimit{true};
	bool useExclusiveAudio{false};
};
}
