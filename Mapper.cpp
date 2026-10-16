#include "Mapper.h"
#include <stdexcept>
#include <algorithm>
#include <string>

namespace {
class Nrom final : public Mapper {
public:
	Nrom(std::vector<uint8_t> prg, std::vector<uint8_t> chr, Mirroring mirroring)
		: prg_(std::move(prg)), chr_(std::move(chr)), mirroring_(mirroring) { if (chr_.empty()) chr_.resize(8192); }
	uint8_t CpuRead(uint16_t address) const override {
		if (address < 0x8000 || prg_.empty()) return 0;
		size_t offset = (address - 0x8000) % prg_.size();
		return prg_[offset];
	}
	void CpuWrite(uint16_t, uint8_t) override {}
	uint8_t PpuRead(uint16_t address) const override { return address < 0x2000 ? chr_[address % chr_.size()] : 0; }
	void PpuWrite(uint16_t address, uint8_t value) override { if (address < 0x2000) chr_[address % chr_.size()] = value; }
	Mirroring GetMirroring() const override { return mirroring_; }
private:
	std::vector<uint8_t> prg_, chr_; Mirroring mirroring_;
};

class Mmc1 final : public Mapper {
public:
	Mmc1(std::vector<uint8_t> prg, std::vector<uint8_t> chr, Mirroring mirroring)
		: prg_(std::move(prg)), chr_(std::move(chr)), mirroring_(mirroring) {
		if (chr_.empty()) chr_.resize(8192); shift_ = 0x10; control_ = 0x0C;
	}
	uint8_t CpuRead(uint16_t address) const override {
		if (address < 0x8000 || prg_.empty()) return 0;
		const size_t bank = prg_.size() / 0x4000;
		size_t offset;
		if (address < 0xC000) {
			if ((control_ & 8) == 0) offset = (static_cast<size_t>(prgBank_ & 0x0E) * 0x4000) + (address - 0x8000);
			else offset = (static_cast<size_t>(prgBank_ & 0x0F) % bank) * 0x4000 + (address - 0x8000);
		} else {
			const size_t last = bank ? bank - 1 : 0;
			const size_t selected = (control_ & 8) == 0 ? (static_cast<size_t>(prgBank_ & 0x0E) + 1) % bank : last;
			offset = selected * 0x4000 + (address - 0xC000);
		}
		return prg_[offset % prg_.size()];
	}
	void CpuWrite(uint16_t address, uint8_t value) override {
		if (address < 0x8000) return;
		if (value & 0x80) { shift_ = 0x10; control_ |= 0x0C; return; }
		const bool complete = shift_ & 1;
		shift_ >>= 1; shift_ |= (value & 1) << 4;
		if (complete) {
			switch ((address >> 13) & 3) { case 0: control_ = shift_ & 0x1F; break; case 1: chrBank0_ = shift_ & 0x1F; break; case 2: chrBank1_ = shift_ & 0x1F; break; default: prgBank_ = shift_ & 0x0F; break; }
			shift_ = 0x10;
		}
	}
	uint8_t PpuRead(uint16_t address) const override {
		if (address >= 0x2000) return 0;
		size_t offset;
		if (control_ & 0x10) offset = (address < 0x1000 ? chrBank0_ : chrBank1_) * 0x1000 + (address & 0x0FFF);
		else offset = (static_cast<size_t>(chrBank0_ & 0x1E) * 0x1000) + address;
		return chr_[offset % chr_.size()];
	}
	void PpuWrite(uint16_t address, uint8_t value) override { if (address < 0x2000) chr_[address % chr_.size()] = value; }
	Mirroring GetMirroring() const override {
		switch (control_ & 3) { case 0: return Mirroring::SingleScreenLower; case 1: return Mirroring::SingleScreenUpper; case 2: return Mirroring::Vertical; default: return Mirroring::Horizontal; }
	}
private:
	std::vector<uint8_t> prg_, chr_; Mirroring mirroring_; uint8_t shift_{0x10}, control_{0x0C}, chrBank0_{}, chrBank1_{}, prgBank_{};
};
}

std::unique_ptr<Mapper> CreateMapper(uint16_t number, std::vector<uint8_t> prg, std::vector<uint8_t> chr, Mirroring mirroring) {
	if (number == 0) return std::make_unique<Nrom>(std::move(prg), std::move(chr), mirroring);
	if (number == 1) return std::make_unique<Mmc1>(std::move(prg), std::move(chr), mirroring);
	throw std::runtime_error("Unsupported NES mapper: " + std::to_string(number));
}
