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

enum progState {
    INPUT_POINTS = 1,
    INPUT_COMPLETE = 2
};
enum progState PROGRAM_STATE = INPUT_POINTS;


//Declarations
void drawLine(HDC hdc, Point start_float, Point end_float, COLORREF color);
void dda1(HDC hdc, Pixel start, Pixel end, COLORREF color);
void dda2(HDC hdc, Pixel start, Pixel end, COLORREF color);
//void br(Pixel start, Pixel end, COLORREF color);


//Hints
//SetPixel(hdc, 100, 100, RGB(255, 0, 0));

//Implementations
void drawLine(HDC hdc, Point start_float, Point end_float, COLORREF color){
	Pixel start = convertPointToPixel(start_float);
	Pixel end = convertPointToPixel(end_float);
	if (selectedMethod == DDA1) {
		dda1(hdc, start, end, color);
	}
	else if (selectedMethod == DDA2) {
		dda2(hdc, start, end, color);
	}
	else if (selectedMethod == Bresenham) {
		//br(start, end, color);
	}
	//update();
}


void dda1(HDC hdc, Pixel start, Pixel end, COLORREF color){
	int x1 = start.x; int y1 = start.y;
	int x2 = end.x; int y2 = end.y;
	int DX = x2 - x1;
	int DY = y2 - y1;
	int minx, maxx, miny, maxy;
	float m;
	//horisontal and vertical
	if (DX == 0) {
		miny = (y1 < y2) ? y1 : y2;
		maxy = (y1 >= y2) ? y1 : y2;
		for (int y = miny; y <= maxy; y++) {
			SetPixel(hdc, x1, y, color);
		}
		return;
	}
	else if (DY == 0) {
		minx = (x1 < x2) ? x1 : x2;
		maxx = (x1 >= x2) ? x1 : x2;
		for (int x = minx; x <= maxx; x++) {
			SetPixel(hdc, x, y1, color);
		}
		return;
	}
	//along Ox
	if (abs(DX) >= abs(DY)) {
		if (x2 > x1) { miny = y1; minx = x1; maxy = y2; maxx = x2; }
		else { miny = y2; minx = x2; maxy = y1; maxx = x1; }
		float y = static_cast<float>(miny);
		m = static_cast<float>(DY) / static_cast<float>(DX);
		for (int x = minx; x <= maxx; x++) {
			SetPixel(hdc, x, static_cast<int>(y), color);
			y += m;
		}
	}
	else { // along Oy
		if (y2 > y1) { miny = y1; minx = x1; maxy = y2; maxx = x2; }
		else { miny = y2; minx = x2; maxy = y1; maxx = x1; }
		float x = static_cast<float>(minx);
		m = static_cast<float>(DX) / static_cast<float>(DY);
		for (int y = miny; y <= maxy; y++) {
			SetPixel(hdc, static_cast<int>(x), y, color);
			x += m;
		}
	}
	return;
}

void dda2(HDC hdc, Pixel start, Pixel end, COLORREF color) {
	int x1 = start.x; int y1 = start.y;
	int x2 = end.x; int y2 = end.y;
	int DX = x2 - x1;
	int DY = y2 - y1;
	float dx, dy;
	int steps; //equals longest dimension
	if (abs(DX) >= abs(DY)) steps = abs(DX);
		else steps = abs(DY);
	dx = DX / (float)steps;
	dy = DY / (float)steps;
	float x = x1;
	float y = y1;
	int i = 0;
	while (i <= steps) {
		SetPixel(hdc, static_cast<int>(x), static_cast<int>(y), color);
		x = x + dx;
		y = y + dy;
		i++;
	}
};
#endif // PICASSO_H_INCLUDED
