#include "Win32Renderer.h"
#include <algorithm>

LRESULT CALLBACK Renderer::WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_NCCREATE) {
		auto* create = reinterpret_cast<CREATESTRUCT*>(lParam);
		auto* renderer = static_cast<Renderer*>(create->lpCreateParams);
		SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(renderer));
		renderer->window_ = window;
	}

	auto* renderer = reinterpret_cast<Renderer*>(GetWindowLongPtr(window, GWLP_USERDATA));
	if (message == WM_PAINT) {
		PAINTSTRUCT paint{};
		HDC deviceContext = BeginPaint(window, &paint);
		if (renderer) renderer->Draw(deviceContext);
		EndPaint(window, &paint);
		return 0;
	}
	if (message == WM_ERASEBKGND) return 1;
	if (message == WM_DESTROY) {
		if (renderer) renderer->window_ = nullptr;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(window, message, wParam, lParam);
}

bool Renderer::Create(HINSTANCE instance, int width, int height) {
	WNDCLASS wc{};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = instance;
	wc.lpszClassName = L"MinNESWindow";
	if (!RegisterClass(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) return false;

	bitmapInfo_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo_.bmiHeader.biWidth = 256;
	bitmapInfo_.bmiHeader.biHeight = -240;
	bitmapInfo_.bmiHeader.biPlanes = 1;
	bitmapInfo_.bmiHeader.biBitCount = 32;
	bitmapInfo_.bmiHeader.biCompression = BI_RGB;

	window_ = CreateWindow(wc.lpszClassName, L"MinNES", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, instance, this);
	if (!window_) return false;
	ShowWindow(window_, SW_SHOW);
	return true;
}

bool Renderer::Pump() {
	MSG message{};
	while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
		if (message.message == WM_QUIT) return false;
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return true;
}

uint8_t Renderer::ControllerState() const {
	uint8_t state = 0;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) state |= 1 << 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) state |= 1 << 1;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) state |= 1 << 2;
	if (GetAsyncKeyState(VK_UP) & 0x8000) state |= 1 << 3;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000) state |= 1 << 4;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) state |= 1 << 5;
	if (GetAsyncKeyState('J') & 0x8000) state |= 1 << 6;
	if (GetAsyncKeyState('K') & 0x8000) state |= 1 << 7;
	return state;
}

void Renderer::Present(const std::array<uint32_t,256*240>& frame) {
	frame_ = frame;
	if (window_) InvalidateRect(window_, nullptr, FALSE);
}

void Renderer::Draw(HDC deviceContext) const {
	RECT client{};
	if (!GetClientRect(window_, &client)) return;
	const int width = client.right - client.left;
	const int height = client.bottom - client.top;
	if (width <= 0 || height <= 0) return;
	StretchDIBits(deviceContext, 0, 0, width, height, 0, 0, 256, 240,
		frame_.data(), &bitmapInfo_, DIB_RGB_COLORS, SRCCOPY);
}

void Renderer::Destroy() {
	if (window_) DestroyWindow(window_);
}
