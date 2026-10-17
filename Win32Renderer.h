#pragma once
#include <windows.h>
#include <array>
#include <cstdint>
class Renderer {
public:
	bool Create(HINSTANCE instance, int width=768, int height=720); bool Pump(); void Present(const std::array<uint32_t,256*240>& frame); void Destroy();
	uint8_t ControllerState() const;
private:
	static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
	void Draw(HDC deviceContext) const;
	HWND window_{};
	std::array<uint32_t,256*240> frame_{};
	BITMAPINFO bitmapInfo_{};
};
