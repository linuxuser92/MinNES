#pragma once
#include <cstdint>

class CpuBus { public: virtual ~CpuBus() = default; virtual uint8_t CpuRead(uint16_t address) = 0; virtual void CpuWrite(uint16_t address, uint8_t value) = 0; };
class CPU {
public:
	void Connect(CpuBus* bus) { bus_ = bus; }
	void Reset();
	uint8_t Step();
	bool Stopped() const { return stopped_; }
private:
	uint8_t Read(uint16_t address) { return bus_->CpuRead(address); }
	void Write(uint16_t address, uint8_t value) { bus_->CpuWrite(address, value); }
	uint16_t Fetch16(); void SetZN(uint8_t value);
	CpuBus* bus_{}; uint16_t pc_{}; uint8_t a_{}, x_{}, y_{}, sp_{0xFD}, p_{0x24}; bool stopped_{};
};
