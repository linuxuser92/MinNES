#pragma once
#include "Mapper.h"
#include <memory>
#include <string>

class Cartridge {
public:
	static std::shared_ptr<Cartridge> Load(const std::string& path);
	uint8_t CpuRead(uint16_t address) const { return mapper_->CpuRead(address); }
	void CpuWrite(uint16_t address, uint8_t value) { mapper_->CpuWrite(address, value); }
	uint8_t PpuRead(uint16_t address) const { return mapper_->PpuRead(address); }
	void PpuWrite(uint16_t address, uint8_t value) { mapper_->PpuWrite(address, value); }
	Mirroring GetMirroring() const { return mapper_->GetMirroring(); }
	uint16_t MapperNumber() const { return mapperNumber_; }
private:
	std::unique_ptr<Mapper> mapper_; uint16_t mapperNumber_{};
};
