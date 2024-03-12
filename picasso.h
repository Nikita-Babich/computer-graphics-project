#ifndef PICASSO_INCLUDED
#define PICASSO_INCLUDED


// Check if <windows.h> is already included
#ifndef _WINDOWS_
#include <windows.h>
#endif

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800
#define DRAW_HEIGHT 400
#define DRAW_WIDTH 400



typedef struct {
    int x;          // X-coordinate
    int y;          // Y-coordinate
    COLORREF color; // Color information
} Pixel;


typedef struct {
    float x;          // X-coordinate
    float y;          // Y-coordinate
    //COLORREF color; // Color information
} Point;

typedef struct {
    Point start;          //
    Point finish;          //
    //COLORREF color; // Color information
} Segment;

typedef std::vector<Segment> Segments;
typedef std::vector<Point> Contour;
typedef std::vector<Contour> Objects;

//math
float scalarProduct(Point A, Point B){ return A.x*B.x+A.y*B.y; }
Point dif(Point A, Point B){ return (Point){B.x-A.x,B.y-A.y}; }
Point orthoVector(Point A, Point B){Point d = dif(A,B); return (Point){-d.y,d.x};}

// Converters
Pixel convertPointToPixel(Point p) { return (Pixel){(int)p.x, (int)p.y}; }
Point convertPixelToPoint(Pixel px) { return (Point){(float)px.x, (float)px.y}; }
//Contour convertSegmentsToContour(Segments f);
Segments convertContourToSegments(Contour c){
	int size = c.size();
	Segments result;
	Segment segment;
	for (int i=0; i<size-1; i++) {
		segment = {};
		segment.start = c[i];
		segment.finish = c[i+1];
		result.push_back(segment);
	};
	if(size >=3){
		segment = {};
		segment.start = c[size-1];
		segment.finish = c[0];
		result.push_back(segment);
	}
	return result;
};

//random creation
Pixel rpi(){  return (Pixel){rand() % DRAW_WIDTH, rand() % DRAW_HEIGHT}; } //random Pixel 
Point rpo(){ return (Point){rand() % DRAW_WIDTH, rand() % DRAW_HEIGHT}; } //random Point
Segment rs(){ return (Segment){ convertPixelToPoint(rpi()), convertPixelToPoint(rpi()) }; }; // random segment
Segments rf(){
	Segments result;
	int size = 10;
	for(size_t i=0; i<size; i++){
		Segment segment = rs();
		result.push_back(segment);
	}
	return result;
}
Contour rcont(int size){
	Contour result;
	for(size_t i=0; i<size; i++){
		Point p = rpo();
		result.push_back(p);
	}
	return result;
}
COLORREF rc(){ return RGB(rand()%255, rand()%255, rand()%255); };




// Global objects
Segments main_Segments;
Contour main_contour;
Objects scene; //for future if needed to drw separte objects

COLORREF main_color = RGB(0, 0, 0);
COLORREF BLACK = RGB(0, 0, 0); 
COLORREF RED = RGB(255, 0, 0);
COLORREF BLUE = RGB(0, 0, 255);
COLORREF GREEN = RGB(0, 255, 0);
COLORREF PINK = RGB(255, 0, 255);

void OpenColorPicker(HWND hwnd) {
	printf("\n Color picker start %d", main_color);
    CHOOSECOLOR cc;
    ZeroMemory(&cc, sizeof(CHOOSECOLOR));
    cc.lStructSize = sizeof(CHOOSECOLOR); //This line causes problems and runtime crashes
    cc.hwndOwner = hwnd;
    cc.lpCustColors = NULL;
    cc.rgbResult = main_color;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        // User selected a color
        main_color = cc.rgbResult;
    }
    printf("\n Color picker end %d", main_color);
}

// enum controllers
enum LineMethod {
    DDA1 = 1,
    DDA2 = 2,
    Bresenham = 3
};
enum LineMethod selectedMethod = DDA1;

enum SliceMethod {
    CyrusBeck = 1,
    SH = 2
};

enum Direction {
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3
};

enum progState {
    INPUT_POINTS = 1,
    INPUT_COMPLETE = 2
};
enum progState PROGRAM_STATE = INPUT_POINTS;


//Declarations of drawing
void drawLine(HDC hdc, Point start_float, Point end_float, COLORREF color);
void dda1(HDC hdc, Pixel start, Pixel end, COLORREF color);
void dda2(HDC hdc, Pixel start, Pixel end, COLORREF color);
void br(HDC hdc, Pixel start, Pixel end, COLORREF color);
void br_circle(HDC hdc, Pixel start, Pixel end, COLORREF color);
void drawSegment(HDC hdc, Segment s, COLORREF color){ drawLine(hdc, s.start, s.finish, color); };
void drawSegments(HDC hdc, Segments f, COLORREF color);
void drawContour(HDC hdc, Contour C, COLORREF color);
void drawRect(HDC hdc,Point A, Point B, COLORREF color);
void redrawAll();

//Modifications
void translatePoint(Direction dir, Point& p);
void translateMainContour(Direction dir);
void rotateMainContour(Direction dir);
void scaleMainContour(Direction dir);
void shearMainContour(Direction dir);
void symmetryMainContour();
bool pointInDraw(Point a){ return a.x<DRAW_WIDTH & a.x>0 & a.y<DRAW_HEIGHT & a.y>0;};
Contour sliceContour(Contour original, Contour edges);


void OnMouseMoveDraw(HDC hdc, int x, int y){
	SetPixel(hdc, x, y, rc());
	
}


//Hints
//SetPixel(hdc, 100, 100, RGB(255, 0, 0));

//Implementations of drawing
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
void drawSegments(HDC hdc, Segments f, COLORREF color){
	for (const Segment& segment : f) {
		drawSegment(hdc, segment, color);
	};
}
void drawContour(HDC hdc, Contour C, COLORREF color){
	Contour E = {
		(Point){0,0}, 
		(Point){DRAW_WIDTH,0}, 
		(Point){DRAW_WIDTH,DRAW_HEIGHT}, 
		(Point){0,DRAW_HEIGHT}, 
		(Point){0,0}
	};
	C = sliceContour(C,E); //problematic
	Segments f = convertContourToSegments(C);
	drawSegments(hdc, f, color);
}
void redrawAll(HWND hwnd) {
    // Invalidate the entire client area
    InvalidateRect(hwnd, NULL, TRUE);

    // Trigger a repaint message
    UpdateWindow(hwnd);
}
void drawRect(HDC hdc,Point A, Point C, COLORREF color){
	Point B = (Point){C.x, A.y};
	Point D = (Point){A.x, C.y};
	drawLine(hdc, A, B, RED);
	drawLine(hdc, B, C, GREEN);
	drawLine(hdc, C, D, BLUE);
	drawLine(hdc, D, A, PINK);
}

//Implementations of modifications
void translatePoint(Direction dir, Point& p){
	int translationSpeed = 3;
	switch(dir){
		case LEFT: p.x = p.x - translationSpeed; break;
		case RIGHT: p.x = p.x + translationSpeed; break;
		case UP: p.y = p.y - translationSpeed; break;
		case DOWN: p.y = p.y + translationSpeed; break;
	}
}
void translateMainContour(Direction dir){
	for (int i=0; i<main_contour.size(); i++) {
		translatePoint(dir, main_contour[i]);
	};
}
void rotateMainContour(Direction dir){
	float rotSpeed = 0.03;
	Point& o = main_contour[0];
	for (int i=1; i<main_contour.size(); i++) {
		Point& p = main_contour[i];
		Point d = dif(o,p);
		
		p.x = d.x*cos(rotSpeed) + ((dir == RIGHT)?-1:1)*d.y*sin(rotSpeed) + o.x;
		p.y = ((dir == RIGHT)?1:-1)*d.x*sin(rotSpeed) + d.y*cos(rotSpeed) + o.y;
	};
}
void scaleMainContour(Direction dir){
	float scaleSpeed = 1.05;
	Point& o = main_contour[0];
	for (int i=1; i<main_contour.size(); i++) {
		Point& p = main_contour[i];
		Point d = dif(o,p);
		switch(dir){
			case UP: p.y = d.y*scaleSpeed + o.y; break;
			case DOWN: p.y = d.y/scaleSpeed+ o.y;break;
			case LEFT: p.x = d.x/scaleSpeed+ o.x;break;
			case RIGHT: p.x = d.x*scaleSpeed+ o.x;break;
		}
	}
}
void shearMainContour(Direction dir){
	float sheerSpeed = 0.05;
	Point& o = main_contour[0];
	for (int i=1; i<main_contour.size(); i++) {
		Point& p = main_contour[i];
		Point d = dif(o,p);
		switch(dir){
			case UP: p.y = p.y - sheerSpeed*d.x; break;
			case DOWN: p.y = p.y + sheerSpeed*d.x; break;
			case LEFT: p.x = p.x + sheerSpeed*d.y; break;
			case RIGHT: p.x = p.x - sheerSpeed*d.y; break;
		}
	}
}
void symmetryMainContour(){
	Point A = main_contour[0];
	Point B = main_contour[1];
	Point d = dif(A,B);
	float u = d.x, v = d.y;
	float a = v, b = -u, c = -a*B.x - b*B.y;
	for (int i=2; i<main_contour.size(); i++) {
		Point& p = main_contour[i];
		Point old = p;
		float param = (a*old.x+b*old.y+c) / (a*a+b*b);
		p.x = old.x - 2*a * param;
		p.y = old.y - 2*b * param;
	}
}

Contour sliceContour(Contour original, Contour edges){
	Contour result;
	
	int size = original.size();
	if(size==2){
		
		Point A = original[0];
		Point B = original[1];
		Point d = dif(A,B);
		//bool inside = pointInDraw(A) | pointInDraw(B);
		//if(inside == FALSE) return result;
		float tl=0, tu=1;
		int edgesCount = edges.size();
		for (int i=0; i < edgesCount-1; i++){
			Point normal = orthoVector(edges[i], edges[i+1]);
			float dn = scalarProduct(d, normal), wn = scalarProduct(dif(edges[i],A), normal);
			if(dn!=0){
				float t = -wn / dn;
				if(dn>0 & t<=1){
					tl=std::max(t,tl);
				}
				if(dn<0 & t>=0){
					tu=std::min(t,tu);
				}
			}
		}
		if(tl<tu){
			A.x += d.x*tl;
			A.y += d.y*tl;
			
			B.x += d.x*tu;
			B.y += d.y*tu;
		}
		result.push_back(B);
		result.push_back(A);
	} else {
		return original;
	}
	return result;
}

#endif // PICASSO_H_INCLUDED
