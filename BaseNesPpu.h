#pragma once

#include <array>
#include <cstdint>

class BaseNesPpu {
public:
	static constexpr size_t ScreenWidth = 256;
	static constexpr size_t ScreenHeight = 240;
	using FrameBuffer = std::array<uint32_t, ScreenWidth * ScreenHeight>;

	BaseNesPpu() = default;
	virtual ~BaseNesPpu() = default;

	void Reset();
	void Step();
	bool FrameReady() const noexcept { return frameReady_; }
	void ClearFrameReady() noexcept { frameReady_ = false; }
	const FrameBuffer& Frame() const noexcept { return frame_; }
	bool VBlank() const noexcept { return vblank_; }

protected:
	virtual uint8_t ReadPatternByte(uint16_t address) const;
	virtual uint8_t ReadNametable(uint16_t address) const;
	virtual uint8_t ReadPalette(uint8_t index) const;

private:
	FrameBuffer frame_{};
	bool frameReady_{};
	bool vblank_{};
	int scanline_{};
	int cycle_{};
};
