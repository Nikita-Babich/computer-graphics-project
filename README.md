# computer-graphics-project
Instead of using Qt I have decided to do the project with WinAPI. This way I speed up compilation time, reduce memory load, SIGNIFICANTLY reduce project size, avoid Visual Studio.

# How to compile
Add to compiler options: -static-libgcc -lgdi32 -lcomdlg32

# My compiler log
Processing C++ source file...
--------
- C++ Compiler: C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\g++.exe
- Command: g++.exe "C:\Users\NIK\Documents\_STU\_sem4\pg\computer-graphics-project\main.cpp" -o "C:\Users\NIK\Documents\_STU\_sem4\pg\computer-graphics-project\main.exe" -g3 -I"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\include" -I"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\x86_64-w64-mingw32\include" -I"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\lib\gcc\x86_64-w64-mingw32\9.2.0\include" -I"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\lib\gcc\x86_64-w64-mingw32\9.2.0\include\c++" -L"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\lib" -L"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\x86_64-w64-mingw32\lib" -static-libgcc -lgdi32 -lcomdlg32 -g3

Compilation results...
--------
- Errors: 0
- Warnings: 0
- Output Filename: C:\Users\NIK\Documents\_STU\_sem4\pg\computer-graphics-project\main.exe
- Output Size: 4,93673992156982 MiB
- Compilation Time: 3,58s

# Known bugs
- When filling a polygon sliced with upper horizontal line, sometimes crashes.

# How to control (everything works in any mode)
- M Cycle through modes (circles, polygon, fill, Hermit curves, Bezier curves, Coons curves)
- Leftclick Add point 
- Rightclick Delete all
- Q/E Rotate
- W/S Scale vertically
- A/D Scale horisontally
- Arrows Translate (move)
- C Set background color
- V Set objects' color
- T/G Shear vertically
- F/H Shear horisontally
- B Flip between triangle fill methods
- R Create random object
- X Symmetry
- Z Cycle through drawline methods (dda1, dda2, Bresenham)
