#pragma once
#include "CPU.h"
#include "PPU.h"
#include <memory>
class NES final : public CpuBus {
public:
	explicit NES(std::shared_ptr<Cartridge> cartridge);
	void Reset(); void Frame();
	uint8_t CpuRead(uint16_t address) override; void CpuWrite(uint16_t address,uint8_t value) override;
	const std::array<uint32_t,256*240>& FrameBuffer() const { return ppu_.Frame(); }
private:
	std::shared_ptr<Cartridge> cartridge_; CPU cpu_; PPU ppu_; std::array<uint8_t,2048> ram_{}; uint8_t controller_{}, controllerShift_{};
};
