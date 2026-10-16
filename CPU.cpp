#include "CPU.h"
void CPU::Reset() { a_=x_=y_=0; sp_=0xFD; p_=0x24; stopped_=false; pc_=static_cast<uint16_t>(Read(0xFFFC) | (Read(0xFFFD) << 8)); }
uint16_t CPU::Fetch16() { uint16_t lo=Read(pc_++), hi=Read(pc_++); return lo | (hi<<8); }
void CPU::SetZN(uint8_t v) { if(v) p_ &= ~2; else p_ |= 2; if(v&0x80) p_ |= 0x80; else p_ &= ~0x80; }
uint8_t CPU::Step() {
	if(stopped_) return 0; const uint8_t op=Read(pc_++); uint16_t addr; uint8_t v;
	switch(op) {
	case 0x00: stopped_=true; return 7;
	case 0xEA: return 2;
	case 0xA9: a_=Read(pc_++); SetZN(a_); return 2;
	case 0xA2: x_=Read(pc_++); SetZN(x_); return 2;
	case 0xA0: y_=Read(pc_++); SetZN(y_); return 2;
	case 0xA5: v=Read(Read(pc_++)); a_=v; SetZN(a_); return 3;
	case 0xAD: a_=Read(Fetch16()); SetZN(a_); return 4;
	case 0x85: Write(Read(pc_++),a_); return 3;
	case 0x8D: Write(Fetch16(),a_); return 4;
	case 0x86: Write(Read(pc_++),x_); return 3;
	case 0x8E: Write(Fetch16(),x_); return 4;
	case 0x84: Write(Read(pc_++),y_); return 3;
	case 0xAA: x_=a_; SetZN(x_); return 2;
	case 0x8A: a_=x_; SetZN(a_); return 2;
	case 0xE8: ++x_; SetZN(x_); return 2;
	case 0xCA: --x_; SetZN(x_); return 2;
	case 0xC8: ++y_; SetZN(y_); return 2;
	case 0x88: --y_; SetZN(y_); return 2;
	case 0x4C: pc_=Fetch16(); return 3;
	case 0x20: addr=Fetch16(); Write(static_cast<uint16_t>(0x100|sp_--),static_cast<uint8_t>((pc_-1)>>8)); Write(static_cast<uint16_t>(0x100|sp_--),static_cast<uint8_t>(pc_-1)); pc_=addr; return 6;
	case 0x60: pc_=Read(static_cast<uint16_t>(0x100|++sp_)); pc_|=Read(static_cast<uint16_t>(0x100|++sp_))<<8; ++pc_; return 6;
	case 0xD0: v=Read(pc_++); if(!(p_&2)) pc_+=static_cast<int8_t>(v); return 2;
	case 0xF0: v=Read(pc_++); if(p_&2) pc_+=static_cast<int8_t>(v); return 2;
	case 0x69: v=Read(pc_++); a_=static_cast<uint8_t>(a_+v+((p_&1)?1:0)); SetZN(a_); return 2;
	case 0xE9: v=Read(pc_++); a_=static_cast<uint8_t>(a_-v-((p_&1)?0:1)); SetZN(a_); return 2;
	default: stopped_=true; return 2;
	}
}
