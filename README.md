This is a beta, so not release. you can complies to run.

# MSVC:
```cmd
cl.exe /EHsc /Zi /Od /Fe:MinNES.exe Cartridge.cpp CPU.cpp Mapper.cpp MinNES.cpp NES.cpp PPU.cpp Win32Renderer.cpp
```

# MinGW:

```cmd
g++ -std=c++17 -Wall -g -O0 Cartridge.cpp CPU.cpp Mapper.cpp MinNES.cpp NES.cpp PPU.cpp Win32Renderer.cpp -o MinNES.exe
```
