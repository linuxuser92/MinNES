#pragma once
#include <windows.h>
#include <d3d11.h>
#include <array>
class Renderer {
public:
	bool Create(HINSTANCE instance, int width=768, int height=720); bool Pump(); void Present(const std::array<uint32_t,256*240>& frame); void Destroy();
private:
	static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM); HWND window_{}; ID3D11Device* device_{}; ID3D11DeviceContext* context_{}; IDXGISwapChain* swapChain_{}; ID3D11Texture2D* texture_{}; ID3D11ShaderResourceView* view_{}; ID3D11RenderTargetView* target_{};
};
