// MinNES.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Cartridge.h"
#include "Logger.h"
#include "NES.h"
#include "Win32Renderer.h"
#include <iostream>
#include <string>

namespace {
bool ParseVerboseOption(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        const std::string argument = argv[i];
        if (argument == "-v" || argument == "--verbose") return true;
    }
    return false;
}
}

int main(int argc, char* argv[])
{
    const bool verbose = ParseVerboseOption(argc, argv);
    SetLogLevel(verbose ? LogLevel::Info : LogLevel::Warning);

    if (argc < 2 || std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
        std::cerr << "MinNES (Minimalist Nintendo Entertainment System for Emulation Software) - A lightweight NES emulator\n\n"
            << "Usage: minnes [options] <GAME.NES>\n\n"
            << "Arguments:\n"
            << "  <GAME.NES>    Path to the NES ROM file (.nes)\n\n"
            << "Options:\n"
            << "  -h, --help    Display this help message\n"
            << "  -v, --verbose Log detailed emulator activity\n"
            << "  -s, --scale   Set window scale factor (e.g., 2, 3)\n"
            << "  -m, --mute    Disable audio playback\n"
            << "Example: minnes Super_Mario_Bros.nes\n";

        if (argc < 2) {
            LogError("No ROM path supplied.");
            return 1;
        }
        return 0;
    }

    LogInfo("MinNES starting");
    try {
        auto cartridge = Cartridge::Load(argv[1]);
        NES nes(cartridge);
        nes.Reset();
        LogInfo("ROM loaded successfully; booting emulator");
        Renderer renderer;
        if (!renderer.Create(GetModuleHandleW(nullptr))) return 1;
        while (renderer.Pump()) {
            nes.SetControllerState(renderer.ControllerState());
            nes.Frame();
            renderer.Present(nes.FrameBuffer());
        }
        renderer.Destroy();
    } catch (const std::exception& error) {
        LogError(error.what());
        return 1;
    }
    return 0;
}
