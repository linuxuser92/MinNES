This is a beta. under developments, so not release. you can complies to run.
Bugs Fixes:
Fix black screen due to CPU/PPU issues

# MSVC:
```cmd
cl.exe /EHsc /Zi /Od /Fe:MinNES.exe Cartridge.cpp CPU.cpp Mapper.cpp MinNES.cpp NES.cpp PPU.cpp Win32Renderer.cpp NsfLoader.cpp NsfeLoader.cpp ROMLoader.cpp Loader.cpp FdsLoader.cpp BaseNesPpu.cpp APU.cpp
```

# MinGW:

```cmd
g++ -std=c++17 -Wall -g -O0 Cartridge.cpp CPU.cpp Mapper.cpp MinNES.cpp NES.cpp PPU.cpp Win32Renderer.cpp -o MinNES.exe
```
