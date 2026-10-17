This is a beta. under developments, so not release. you can complies to run.
Bugs Fixes:
Fix black screen due to CPU/PPU issues

# MSVC:
```cmd
cl /EHsc /std:c++17 /O2 /Fe:MinNES.exe *.cpp /link user32.lib gdi32.lib
```

# MinGW:

```cmd
g++ -std=c++17 -O2 -Wall *.cpp -o MinNES.exe -lgdi32 -luser32
```
