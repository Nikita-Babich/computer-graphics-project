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
Pixel rp(){ return (Pixel){rand() % 800, rand() % 800}; } //random Pixel

typedef struct {
    float x;          // X-coordinate
    float y;          // Y-coordinate
    //COLORREF color; // Color information
} Point;

Pixel convertPointToPixel(Point p) { return (Pixel){(int)p.x, (int)p.y}; }
Point convertPixelToPoint(Pixel px) { return (Point){(float)px.x, (float)px.y}; }

typedef struct {
    Point start;          //
    Point finish;          //
    //COLORREF color; // Color information
} Segment;

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
void br(HDC hdc, Pixel start, Pixel end, COLORREF color);
void br_circle(HDC hdc, Pixel start, Pixel end, COLORREF color);


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
		br(hdc, start, end, color);
	}
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

void br(HDC hdc, Pixel start, Pixel end, COLORREF color) {
	int x1 = start.x; int y1 = start.y;
	int x2 = end.x; int y2 = end.y;
	int DX, DY;
	int helper;
	int k1, k2, p, x, y;
	if (abs(x2-x1) > abs(y2-y1)) { //riadiaca Ox
		//sorting
		if (x2 < x1) { 
			helper = x1; x1 = x2; x2 = helper;
			helper = y1; y1 = y2; y2 = helper;}
		DX = x2 - x1;
		DY = y2 - y1;
		//algo
		k1 = 2 * DY; k2 = k1 + 2 * DX * ((DY > 0) ? -1 : 1);
		p = k1 - DX;
		x = x1; y = y1;
		SetPixel(hdc, x, y, color);
		while (x < x2) {
			x++;
			if (DY > 0) {
				if (p > 0) { y++; p += k2; } else p += k1;
			}
			else {
				if (p < 0) { y--; p += k2; } else p += k1;
			}
			SetPixel(hdc, x, y, color);
		}
	}
	else { //riadiaca Oy
		//sorting
		if (y2 < y1) {
			helper = x1; x1 = x2; x2 = helper;
			helper = y1; y1 = y2; y2 = helper;
		}
		DX = x2 - x1;
		DY = y2 - y1;
		//algo
		k1 = 2 * DX; k2 = k1 + 2 * DY * ((DX > 0) ? -1 : 1);
		p = k1 - DY;
		x = x1; y = y1;
		SetPixel(hdc, x, y, color);
		while (y < y2) {
			y++;
			if (DX > 0) {
				if (p > 0) { x++; p += k2; }
				else p += k1;
			}
			else {
				if (p < 0) { x--; p += k2; }
				else p += k1;
			}
			SetPixel(hdc, x, y, color);
		}
	}
};

void br_circle(HDC hdc, Pixel start, Pixel end, COLORREF color) {
	int xc = start.x;
	int yc = start.y;
	int ex = end.x;
	int ey = end.y;
	float rd = sqrt((ex - xc) * (float)(ex - xc) + (ey - yc) * (ey - yc)); // (int)sqrt // sqrt4
	int r = (rd < INT16_MAX) ? (int)rd : 10; //prevention of too big numbers

	int p = 1 - r;
	int x = 0; int y = r;
	int dvaX = 3; int dvaY = 2*r - 2;
	while (x <= y) {
		//  54
		//6    3
		//7    2
		//  81
		SetPixel(hdc, xc + x, yc  - y, color); //1
		SetPixel(hdc, xc - x, yc  - y, color); //8
		SetPixel(hdc, xc  - y, yc - x, color); //3
		SetPixel(hdc, xc  - y, yc + x, color); //2

		SetPixel(hdc, xc + x, yc  + y, color); //4
		SetPixel(hdc, xc - x, yc  + y, color); //5
		SetPixel(hdc, xc  + y, yc - x, color); //6
		SetPixel(hdc, xc  + y, yc + x, color); //7

		if (p > 0) {
			p = p - dvaY;
			y--;
			dvaY = dvaY - 2;
		}
		p = p + dvaX;
		dvaX = dvaX + 2;
		x++;
	}
}

#endif // PICASSO_H_INCLUDED
