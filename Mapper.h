#pragma once
#include <cstdint>
#include <memory>
#include <vector>

enum class Mirroring { Horizontal, Vertical, SingleScreenLower, SingleScreenUpper, FourScreen };

class Mapper {
public:
	virtual ~Mapper() = default;
	virtual uint8_t CpuRead(uint16_t address) const = 0;
	virtual void CpuWrite(uint16_t address, uint8_t value) = 0;
	virtual uint8_t PpuRead(uint16_t address) const = 0;
	virtual void PpuWrite(uint16_t address, uint8_t value) = 0;
	virtual Mirroring GetMirroring() const = 0;
};

std::unique_ptr<Mapper> CreateMapper(uint16_t number, std::vector<uint8_t> prg, std::vector<uint8_t> chr, Mirroring mirroring);
