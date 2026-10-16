#pragma once
#include "Cartridge.h"
#include <array>
#include <cstdint>
class PPU {
public:
	void Connect(std::shared_ptr<Cartridge> cartridge) { cartridge_=std::move(cartridge); }
	uint8_t CpuRead(uint16_t address); void CpuWrite(uint16_t address, uint8_t value);
	void Step(); bool FrameReady() const { return frameReady_; } void ClearFrameReady() { frameReady_=false; }
	const std::array<uint32_t,256*240>& Frame() const { return frame_; }
private:
	std::shared_ptr<Cartridge> cartridge_; std::array<uint8_t,2048> vram_{}; std::array<uint8_t,32> palette_{}; std::array<uint32_t,256*240> frame_{};
	uint8_t regs_[8]{}; uint16_t address_{}, temp_{}, fineX_{}; bool addressLatch_{}, frameReady_{}; int scanline_{}, cycle_{};
};
