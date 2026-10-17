#include "Mapper.h"
#include <algorithm>
#include <stdexcept>
#include <string>

namespace {
class Nrom final : public Mapper {
public:
	Nrom(std::vector<uint8_t> prg, std::vector<uint8_t> chr, std::vector<uint8_t> prgRam, Mirroring mirroring, bool chrRam)
		: prg_(std::move(prg)), chr_(std::move(chr)), prgRam_(std::move(prgRam)), mirroring_(mirroring), chrRam_(chrRam) {
		if (prg_.size() != 0x4000 && prg_.size() != 0x8000) throw std::runtime_error("Mapper 0 requires 16 or 32 KiB of PRG ROM");
		if (chr_.empty()) throw std::runtime_error("Mapper 0 requires CHR ROM or RAM");
	}
	uint8_t CpuRead(uint16_t address) const override {
		if (address >= 0x6000 && address < 0x8000) return prgRam_.empty() ? 0 : prgRam_[(address - 0x6000) % prgRam_.size()];
		if (address >= 0x8000) return prg_[(address - 0x8000) % prg_.size()];
		return 0;
	}
	void CpuWrite(uint16_t address, uint8_t value) override {
		if (address >= 0x6000 && address < 0x8000 && !prgRam_.empty()) prgRam_[(address - 0x6000) % prgRam_.size()] = value;
	}
	uint8_t PpuRead(uint16_t address) const override { return address < 0x2000 ? chr_[address % chr_.size()] : 0; }
	void PpuWrite(uint16_t address, uint8_t value) override { if (chrRam_ && address < 0x2000) chr_[address % chr_.size()] = value; }
	Mirroring GetMirroring() const override { return mirroring_; }
private:
	std::vector<uint8_t> prg_, chr_, prgRam_;
	Mirroring mirroring_;
	bool chrRam_;
};

class UxRom final : public Mapper {
public:
	UxRom(std::vector<uint8_t> prg, std::vector<uint8_t> chr, std::vector<uint8_t> prgRam, Mirroring mirroring, bool chrRam)
		: prg_(std::move(prg)), chr_(std::move(chr)), prgRam_(std::move(prgRam)), mirroring_(mirroring), chrRam_(chrRam) {
		if (prg_.empty() || (prg_.size() % 0x4000) != 0) throw std::runtime_error("Mapper 2 requires PRG ROM in 16 KiB banks");
		if (chr_.empty()) throw std::runtime_error("Mapper 2 requires CHR ROM or RAM");
	}
	uint8_t CpuRead(uint16_t address) const override {
		if (address >= 0x6000 && address < 0x8000) return prgRam_.empty() ? 0 : prgRam_[(address - 0x6000) % prgRam_.size()];
		if (address < 0x8000) return 0;

		const size_t bankCount = prg_.size() / 0x4000;
		const size_t bank = address < 0xC000 ? bankSelect_ % bankCount : bankCount - 1;
		return prg_[bank * 0x4000 + (address & 0x3FFF)];
	}
	void CpuWrite(uint16_t address, uint8_t value) override {
		if (address >= 0x6000 && address < 0x8000 && !prgRam_.empty()) prgRam_[(address - 0x6000) % prgRam_.size()] = value;
		if (address >= 0x8000 && address <= 0xFFFF) bankSelect_ = value & 0x0F;
	}
	uint8_t PpuRead(uint16_t address) const override { return address < 0x2000 ? chr_[address % chr_.size()] : 0; }
	void PpuWrite(uint16_t address, uint8_t value) override { if (chrRam_ && address < 0x2000) chr_[address % chr_.size()] = value; }
	Mirroring GetMirroring() const override { return mirroring_; }
private:
	std::vector<uint8_t> prg_, chr_, prgRam_;
	Mirroring mirroring_;
	bool chrRam_;
	uint8_t bankSelect_{0};
};

class Mmc1 final : public Mapper {
public:
	Mmc1(std::vector<uint8_t> prg, std::vector<uint8_t> chr, std::vector<uint8_t> prgRam, Mirroring mirroring, bool chrRam)
		: prg_(std::move(prg)), chr_(std::move(chr)), prgRam_(std::move(prgRam)), mirroring_(mirroring), chrRam_(chrRam) {
		if (prg_.empty() || (prg_.size() % 0x4000) != 0) throw std::runtime_error("Mapper 1 requires PRG ROM in 16 KiB banks");
		if (chr_.empty()) throw std::runtime_error("Mapper 1 requires CHR ROM or RAM");
	}
	uint8_t CpuRead(uint16_t address) const override {
		if (address >= 0x6000 && address < 0x8000)
			return (prgBank_ & 0x10) || prgRam_.empty() ? 0 : prgRam_[(address - 0x6000) % prgRam_.size()];
		if (address < 0x8000) return 0;

		const size_t bankCount = prg_.size() / 0x4000;
		const size_t selectedBank = prgBank_ & 0x0F;
		const size_t mode = (control_ >> 2) & 3;
		size_t bank;
		if (mode < 2) {
			const size_t pair = (selectedBank & 0x0E) % bankCount;
			bank = (pair + (address >= 0xC000 ? 1 : 0)) % bankCount;
		} else if (mode == 2) {
			bank = address < 0xC000 ? 0 : selectedBank % bankCount;
		} else {
			bank = address < 0xC000 ? selectedBank % bankCount : bankCount - 1;
		}
		return prg_[bank * 0x4000 + (address & 0x3FFF)];
	}
	void CpuWrite(uint16_t address, uint8_t value) override {
		if (address >= 0x6000 && address < 0x8000) {
			if (!(prgBank_ & 0x10) && !prgRam_.empty()) prgRam_[(address - 0x6000) % prgRam_.size()] = value;
			return;
		}
		if (address < 0x8000) return;
		if (value & 0x80) { shift_ = 0x10; control_ |= 0x0C; return; }
		const bool complete = (shift_ & 1) != 0;
		shift_ = static_cast<uint8_t>((shift_ >> 1) | ((value & 1) << 4));
		if (complete) {
			switch ((address >> 13) & 3) {
			case 0: control_ = shift_ & 0x1F; break;
			case 1: chrBank0_ = shift_ & 0x1F; break;
			case 2: chrBank1_ = shift_ & 0x1F; break;
			default: prgBank_ = shift_ & 0x1F; break;
			}
			shift_ = 0x10;
		}
	}
	uint8_t PpuRead(uint16_t address) const override {
		if (address >= 0x2000) return 0;
		const size_t offset = (control_ & 0x10)
			? static_cast<size_t>(address < 0x1000 ? chrBank0_ : chrBank1_) * 0x1000 + (address & 0x0FFF)
			: static_cast<size_t>(chrBank0_ & 0x1E) * 0x1000 + address;
		return chr_[offset % chr_.size()];
	}
	void PpuWrite(uint16_t address, uint8_t value) override {
		if (chrRam_ && address < 0x2000) {
			const size_t offset = (control_ & 0x10)
				? static_cast<size_t>(address < 0x1000 ? chrBank0_ : chrBank1_) * 0x1000 + (address & 0x0FFF)
				: static_cast<size_t>(chrBank0_ & 0x1E) * 0x1000 + address;
			chr_[offset % chr_.size()] = value;
		}
	}
	Mirroring GetMirroring() const override {
		if (mirroring_ == Mirroring::FourScreen) return mirroring_;
		switch (control_ & 3) {
		case 0: return Mirroring::SingleScreenLower;
		case 1: return Mirroring::SingleScreenUpper;
		case 2: return Mirroring::Vertical;
		default: return Mirroring::Horizontal;
		}
	}
private:
	std::vector<uint8_t> prg_, chr_, prgRam_;
	Mirroring mirroring_;
	bool chrRam_;
	uint8_t shift_{0x10}, control_{0x0C}, chrBank0_{}, chrBank1_{}, prgBank_{};
};
}

std::unique_ptr<Mapper> CreateMapper(uint16_t number, std::vector<uint8_t> prg, std::vector<uint8_t> chr, std::vector<uint8_t> prgRam, Mirroring mirroring, bool chrRam) {
	if (number == 0) return std::make_unique<Nrom>(std::move(prg), std::move(chr), std::move(prgRam), mirroring, chrRam);
	if (number == 1) return std::make_unique<Mmc1>(std::move(prg), std::move(chr), std::move(prgRam), mirroring, chrRam);
	if (number == 2) return std::make_unique<UxRom>(std::move(prg), std::move(chr), std::move(prgRam), mirroring, chrRam);
	throw std::runtime_error("Unsupported NES mapper: " + std::to_string(number));
}
