// MinNES.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Cartridge.h"
#include "NES.h"
#include "Win32Renderer.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2 || std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
        std::cerr << "MinNES (Minimalist Nintendo Entertainment System for Emulation Software) - A lightweight NES emulator\n\n"
            << "Usage: minnes [options] <GAME.NES>\n\n"
            << "Arguments:\n"
            << "  <GAME.NES>    Path to the NES ROM file (.nes)\n\n"
            << "Options:\n"
            << "  -h, --help    Display this help message\n"
            << "  -s, --scale   Set window scale factor (e.g., 2, 3)\n"
            << "  -m, --mute    Disable audio playback\n"
            << "Example: minnes Super_Mario_Bros.nes\n";

        // Return 1 if it's an error (missing ROM), return 0 if they explicitly asked for help
        return (argc < 2) ? 1 : 0;
    }

    try {
        auto cartridge = Cartridge::Load(argv[1]);
        NES nes(cartridge);
        nes.Reset();
        Renderer renderer;
        if (!renderer.Create(GetModuleHandleW(nullptr))) return 1;
        while (renderer.Pump()) {
            nes.Frame();
            renderer.Present(nes.FrameBuffer());
        }
        renderer.Destroy();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
