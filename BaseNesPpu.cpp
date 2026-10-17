#include "BaseNesPpu.h"

namespace {
constexpr uint32_t PackColor(uint8_t r, uint8_t g, uint8_t b) {
	return 0xFF000000u | (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);
}
}

void BaseNesPpu::Reset() {
	frame_.fill(0xFF000000u);
	frameReady_ = false;
	vblank_ = false;
	scanline_ = 0;
	cycle_ = 0;
}

uint8_t BaseNesPpu::ReadPatternByte(uint16_t address) const {
	return static_cast<uint8_t>((address * 13u) & 0xFFu);
}

uint8_t BaseNesPpu::ReadNametable(uint16_t address) const {
	return static_cast<uint8_t>((address * 17u) & 0xFFu);
}

uint8_t BaseNesPpu::ReadPalette(uint8_t index) const {
	return static_cast<uint8_t>(index * 29u);
}

void BaseNesPpu::Step() {
	++cycle_;
	if (cycle_ >= 341) {
		cycle_ = 0;
		++scanline_;
	}

	if (scanline_ >= 240 && scanline_ < 261) {
		vblank_ = true;
	}

	if (scanline_ == 261) {
		scanline_ = 0;
		vblank_ = false;

		for (int y = 0; y < static_cast<int>(ScreenHeight); ++y) {
			for (int x = 0; x < static_cast<int>(ScreenWidth); ++x) {
				const uint16_t patternAddress = static_cast<uint16_t>(((y / 8) * 32 + (x / 8)) * 16 + (y & 7));
				const uint8_t pattern = ReadPatternByte(patternAddress);
				const uint8_t colorIndex = static_cast<uint8_t>((pattern + (x & 7)) & 0xFFu);
				const uint8_t paletteIndex = ReadPalette(colorIndex);
				const uint8_t r = static_cast<uint8_t>((paletteIndex * 29u) & 0xFFu);
				const uint8_t g = static_cast<uint8_t>((paletteIndex * 47u) & 0xFFu);
				const uint8_t b = static_cast<uint8_t>((paletteIndex * 73u) & 0xFFu);
				frame_[static_cast<size_t>(y) * ScreenWidth + static_cast<size_t>(x)] = PackColor(r, g, b);
			}
		}

		frameReady_ = true;
	}
}
