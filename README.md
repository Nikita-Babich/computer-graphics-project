# computer-graphics-project
Instead of using Qt I have decided to do the project with WinAPI. This way I speed up compilation time, reduce memory load, SIGNIFICANTLY reduce project size, avoid Visual Studio.

# How to compile
Add to compiler options: -static-libgcc -lgdi32 -lcomdlg32

# Known bugs
- When filling a polygon sliced with horizontal line, crashes.

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
