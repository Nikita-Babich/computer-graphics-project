#ifndef PICASSO_INCLUDED
#define PICASSO_INCLUDED

// Check if <windows.h> is already included
#ifndef _WINDOWS_
#include <windows.h>
#endif

typedef struct {
    int x;          // X-coordinate
    int y;          // Y-coordinate
    //COLORREF color; // Color information
} Pixel;

typedef struct {
    float x;          // X-coordinate
    float y;          // Y-coordinate
    //COLORREF color; // Color information
} Point;

Pixel convertPointToPixel(Point p) { return (Pixel){(int)p.x, (int)p.y}; }
Point convertPixelToPoint(Pixel px) { return (Point){(float)px.x, (float)px.y}; }


enum LineMethod {
    DDA1 = 1,
    DDA2 = 2,
    Bresenham = 3
};
enum LineMethod selectedMethod = DDA1;



void drawLine(Point start_float, Point end_float, COLORREF color, enum LineMethod method);
void dda1(Pixel start, Pixel end, COLORREF color);
void dda2(Pixel start, Pixel end, COLORREF color);
void br(Pixel start, Pixel end, COLORREF color);



//SetPixel(hdc, 100, 100, RGB(255, 0, 0));