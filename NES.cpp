#include "NES.h"
NES::NES(std::shared_ptr<Cartridge> cartridge):cartridge_(std::move(cartridge)){cpu_.Connect(this); ppu_.Connect(cartridge_);}
void NES::Reset(){cpu_.Reset();}
uint8_t NES::CpuRead(uint16_t address){if(address<0x2000)return ram_[address&0x7FF]; if(address<0x4000)return ppu_.CpuRead(0x2000|(address&7)); if(address==0x4016)return static_cast<uint8_t>((controllerShift_>>7)&1); if(address>=0x8000)return cartridge_->CpuRead(address); return 0;}
void NES::CpuWrite(uint16_t address,uint8_t value){if(address<0x2000)ram_[address&0x7FF]=value; else if(address<0x4000)ppu_.CpuWrite(0x2000|(address&7),value); else if(address==0x4016){if(value&1)controllerShift_=controller_;} else if(address>=0x8000)cartridge_->CpuWrite(address,value);}
void NES::Frame(){for(int i=0;i<29780;++i){const uint8_t cycles=cpu_.Step(); for(uint8_t c=0;c<cycles*3;++c)ppu_.Step(); if(cpu_.Stopped())break;}}
