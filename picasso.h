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

#define UPDATE {\
			PAINTSTRUCT ps;\
    		HDC hdc;\
			hdc = BeginPaint(hwnd, &ps); \
        	UpdateScreen(hdc);\
        	EndPaint(hwnd, &ps);};

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
Segment orthoSegment(Segment A){
	Point newd = orthoVector(A.start, A.finish);
	A.finish.x = A.start.x + newd.x;
	A.finish.y = A.start.y + newd.y;
	return A;
}

// Converters
Pixel convertPointToPixel(Point p) { return (Pixel){(int)p.x, (int)p.y}; }
Point convertPixelToPoint(Pixel px) { return (Point){(float)px.x, (float)px.y}; }
//Contour convertSegmentsToContour(Segments f);
Segments convertContourToSegments(Contour c){
	int size = c.size();
	Segments result;
	Segment segment;
	if (size>=2){
		for (int i=0; i<size-1; i++) {
			segment = {};
			segment.start = c[i];
			segment.finish = c[i+1];
			result.push_back(segment);
		};
	}
	if(size >=3){
		segment = {};
		segment.start = c[size-1];
		segment.finish = c[0];
		result.push_back(segment);
	}
	return result;
};
Contour flip90(Contour original){
	Contour result;
	for (const Point& point : original) {
		Point temp;
		temp.x = point.y;
		temp.y = -point.x;
		result.push_back(temp);
	};
	return result;
}

//random creation
Pixel rpi(){  return (Pixel){rand() % DRAW_WIDTH, rand() % DRAW_HEIGHT}; } //random Pixel 
Point rpo(){ return (Point){rand() % DRAW_WIDTH, rand() % DRAW_HEIGHT}; } //random Point
Segment rs(){ return (Segment){ convertPixelToPoint(rpi()), convertPixelToPoint(rpi()) }; }; // random segment
Segments rss(int size = 10){
	Segments result;
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


COLORREF BLACK = RGB(0, 0, 0); 
COLORREF WHITE = RGB(255, 255, 255); 
COLORREF RED = RGB(0, 0, 255);
COLORREF BLUE = RGB(255, 0, 0);
COLORREF GREEN = RGB(0, 255, 0);
COLORREF PINK = RGB(255, 0, 255);

COLORREF main_color = BLACK;
COLORREF background_color = WHITE;

//circles
//Pixel circle11 = rpi();
//Pixel circle12 = rpi();
//Pixel circle21 = rpi();
//Pixel circle22 = rpi();
//COLORREF circle_color = RGB(40, 255, 100);

// Speeding up by using buffer


BITMAPINFO bmi;
void InitializeBitmapInfo(BITMAPINFO* bmi) {
    ZeroMemory(bmi, sizeof(BITMAPINFO));
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = WINDOW_WIDTH;
    bmi->bmiHeader.biHeight = -WINDOW_HEIGHT;  // Use negative height for top-down bitmap
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = 32;  // Assuming you are using 32-bit color depth (COLORREF)
    bmi->bmiHeader.biCompression = BI_RGB;
};

COLORREF buffer[WINDOW_HEIGHT][WINDOW_WIDTH];
// Function to initialize buffer
void InitializeBuffer() {
    //buffer = (COLORREF*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(COLORREF));
    if (buffer == NULL) {
        // memory allocation failure, no idea ho to handle
    }
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        // Iterate over each column in the current row
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            // Set the color of the current pixel in the buffer
            buffer[i][j] = background_color; //  background_color or RGB(255, 0, 0)
        }
    }
    // initialize buffer to background color or any default color
    //memset(buffer, RGB(255, 100, 255), WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(COLORREF));
    //printf( "\n buffer has %d %d %d", buffer[0], buffer[1], buffer[2]);
}
// Function to draw a pixel in buffer
void DrawPixel(int x, int y, COLORREF color) {
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
        buffer[y][x]= color;
    }
}
void DrawPoint(Point A, COLORREF color){
	DrawPixel(static_cast<float>(A.x),static_cast<float>(A.y),color);
}
// Function to transfer buffer to screen
void UpdateScreen(HDC hdc) {
    if (buffer != NULL) {
    	//printf("\n buffer exists, update called ");
        SetDIBitsToDevice(hdc,  0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, WINDOW_HEIGHT, buffer, &bmi, DIB_RGB_COLORS); //DIB_PAL_COLORS
    }
}


//CHOOSECOLOR cc = {0};
void OpenColorPicker(HWND hwnd, int which) {
	printf("\n Color picker start %d", main_color);
    CHOOSECOLOR cc;
    COLORREF custColors[16] = { 0 };
    
    //cc = {0};
    ZeroMemory(&cc, sizeof(CHOOSECOLOR));
    cc.lStructSize = sizeof(CHOOSECOLOR); //This line causes problems and runtime crashes
    cc.hwndOwner = hwnd;
    cc.lpCustColors = custColors;
    cc.rgbResult = main_color;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	
	COLORREF output;
    if (ChooseColor(&cc)) {
        // User selected a color
        //main_color = cc.rgbResult;
        output = RGB(GetBValue(cc.rgbResult), GetGValue(cc.rgbResult), GetRValue(cc.rgbResult)); //solving reverse color issue
    }
    (which)?(main_color = output):(background_color = output);
    printf("\n Color picker end %x", output);
}

// enum controllers
enum LineMethod {
    DDA1 = 0,
    DDA2 = 1,
    Bresenham = 2
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
    MODE_CIRCLES = 0,
    MODE_CONTOUR = 1,
    MODE_CONTOUR_FILLED = 2,
    MODE_HERMIT_CURVE = 3,
    MODE_BEZIER_CURVE = 4,
    MODE_COONS_CURVE = 5,
    
};
enum progState PROGRAM_MODE = MODE_CIRCLES;


//Declarations of drawing
void drawLine(  Point start_float, Point end_float, COLORREF color);
void dda1(  Pixel start, Pixel end, COLORREF color);
void dda2(  Pixel start, Pixel end, COLORREF color);
void br(  Pixel start, Pixel end, COLORREF color);
void br_circle(  Pixel start, Pixel end, COLORREF color);
void drawSegment(  Segment s, COLORREF color){ drawLine(  s.start, s.finish, color); };
void drawSegments(  Segments f, COLORREF color);
void drawContour(  Contour C, COLORREF color);
void drawRect( Point A, Point B, COLORREF color);
//void redrawAll();

//Modifications
void translatePoint(Direction dir, Point& p);
void translateMainContour(Direction dir);
void rotateMainContour(Direction dir);
void scaleMainContour(Direction dir);
void shearMainContour(Direction dir);
void symmetryMainContour();
bool pointInDraw(Point a){ return a.x<DRAW_WIDTH & a.x>0 & a.y<DRAW_HEIGHT & a.y>0;};
bool segmentInDraw(Point A, Point B){
	Point d = dif(A,B);
	Point tester = A;
	d.x = d.x/10; d.y = d.y/10;
	bool result = pointInDraw(A);
	for(int i = 0; i<10; i++){
		tester.x += d.x;
		tester.y += d.y;
		result = result || pointInDraw(tester);
	}
	return result;
}
Contour sliceContour(Contour original, Contour edges, int xmin);


void OnMouseMoveDraw( int x, int y){
	//SetPixel(hdc, x, y, rc());
	DrawPixel(x,y, BLACK);
}


//Hints
//SetPixel(  100, 100, RGB(255, 0, 0));

//Implementations of drawing
void drawLine( Point start_float, Point end_float, COLORREF color){
	Pixel start = convertPointToPixel(start_float);
	Pixel end = convertPointToPixel(end_float);
	if (selectedMethod == DDA1) {
		dda1(  start, end, color);
	}
	else if (selectedMethod == DDA2) {
		dda2(  start, end, color);
	}
	else if (selectedMethod == Bresenham) {
		br(  start, end, color);
	}
}
void dda1( Pixel start, Pixel end, COLORREF color){
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
			//SetPixel(hdc, x1, y, color);
			DrawPixel(x1, y, color);
		}
		return;
	}
	else if (DY == 0) {
		minx = (x1 < x2) ? x1 : x2;
		maxx = (x1 >= x2) ? x1 : x2;
		for (int x = minx; x <= maxx; x++) {
			//SetPixel(  x, y1, color);
			DrawPixel(x, y1, color);
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
			//SetPixel(hdc, x, static_cast<int>(y), color);
			DrawPixel(x, static_cast<int>(y), color);
			y += m;
		}
	}
	else { // along Oy
		if (y2 > y1) { miny = y1; minx = x1; maxy = y2; maxx = x2; }
		else { miny = y2; minx = x2; maxy = y1; maxx = x1; }
		float x = static_cast<float>(minx);
		m = static_cast<float>(DX) / static_cast<float>(DY);
		for (int y = miny; y <= maxy; y++) {
			//SetPixel(  static_cast<int>(x), y, color);
			DrawPixel(static_cast<int>(x), y, color);
			x += m;
		}
	}
	return;
}
void dda2( Pixel start, Pixel end, COLORREF color) {
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
		//SetPixel(hdc, static_cast<int>(x), static_cast<int>(y), color);
		DrawPixel(static_cast<int>(x), static_cast<int>(y), color);
		x = x + dx;
		y = y + dy;
		i++;
	}
};
void br( Pixel start, Pixel end, COLORREF color) {
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
		//SetPixel(  x, y, color);
		DrawPixel(x+1, y+1, color);
		while (x < x2) {
			x++;
			if (DY > 0) {
				if (p > 0) { y++; p += k2; } else p += k1;
			}
			else {
				if (p < 0) { y--; p += k2; } else p += k1;
			}
			//SetPixel(hdc, x, y, color);
			DrawPixel(x+1, y+1, color);
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
		//SetPixel(  x, y, color);
		DrawPixel(x+1, y+1, color);
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
			//SetPixel(hdc, x, y, color);
			DrawPixel(x+1, y+1, color);
		}
	}
};
void br_circle(  Pixel start, Pixel end, COLORREF color) {
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

		DrawPixel( xc + x, yc  - y, color); //1
		DrawPixel( xc - x, yc  - y, color); //8
		DrawPixel( xc  - y, yc - x, color); //3
		DrawPixel( xc  - y, yc + x, color); //2

		DrawPixel( xc + x, yc  + y, color); //4
		DrawPixel( xc - x, yc  + y, color); //5
		DrawPixel( xc  + y, yc - x, color); //6
		DrawPixel( xc  + y, yc + x, color); //7

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
void drawPlus(Point point,COLORREF color){
	drawLine( (Point){point.x-5,point.y}, (Point){point.x+5,point.y}, color);
	drawLine( (Point){point.x,point.y-5}, (Point){point.x,point.y+5}, color);
}
void drawPluses(Contour C,COLORREF color){
	for (const Point& point : C) {
		drawPlus(point, color);
	};
}
void drawSegments(  Segments f, COLORREF color){
	for (const Segment& segment : f) {
		drawSegment(  segment, color);
	};
}

float F0(float t){return 2*t*t*t - 3*t*t + 1;}
float F1(float t){return -2*t*t*t + 3*t*t;}
float F2(float t){return t*t*t - 2*t*t + t;}
float F3(float t){return t*t*t - t*t;}
Point tv(Contour c, int i){ //tangent vector
	Point a = c[i];
	Point b = c[i+1];
	return (Point){b.x-a.x, b.y-a.y	};
}
void drawHermit(Contour C){
	int size = C.size();
	int n = size/2;
	//Contour points;
	//Contour tangents;
	Segment s;
	float deltat = 0.01;
	float t;
	for(int i = 1; i<=n-1; i++){
		drawSegment((Segment){
			C[2*i-1], C[2*i-2]
		}, PINK);
		s.start = C[(i-1)*2];
		t = deltat;
		while(t<1){
			Point Q1 = 
				(Point){
				C[(i-1)*2].x*F0(t) + C[i*2].x*F1(t) + tv(C,(i-1)*2).x*F2(t) + tv(C,2*i).x*F3(t),
				C[(i-1)*2].y*F0(t) + C[i*2].y*F1(t) + tv(C,(i-1)*2).y*F2(t) + tv(C,2*i).y*F3(t)
			};
			s.finish = Q1;
			drawSegment(s, main_color);
			s.start = Q1;
			t = t+deltat;
		}
		s.finish = C[i*2];
		drawSegment(s, main_color);
	}
	drawSegment((Segment){ C[size-1], C[size-2] }, PINK);
}
void printBezierCurve(const std::vector<Contour>& P) { //for debug
    printf("Bezier Curve Contours:\n");
    for (size_t i = 0; i < P.size(); ++i) {
        printf("t = %zu:\t", i);
        const Contour& contour = P[i];
        for (const Point& point : contour) {
            printf("(%d, %d)\t", point.x, point.y);
        }
        printf("\n");
    }
}
void drawBezier(Contour C){
	int n = C.size();
    float deltat = 0.001;
    float t = deltat;
    Segment s;
    s.start = C[0];
    
    std::vector<Contour> P;
    
    while(t<1){
    	P.clear();
        P.push_back(C);
    	for(int i = 1; i<=n; i++){
    		Contour row;
    		for(int j = 0; j<=n-i; j++){
    			Point p = (Point){
    				(1-t)*P[i-1][j].x + t * P[i-1][j+1].x,
    				(1-t)*P[i-1][j].y + t * P[i-1][j+1].y
				};
				row.push_back(p);
    		}
    		P.push_back(row);
		}
		//printBezierCurve(P); //extremely slow debug function
		s.finish = P[n-1][0];
    	drawSegment(s, main_color);
    	s.start = s.finish;
    	t = t+deltat;
	}
	s.finish = C[n-1];
	drawSegment(s, main_color);
}

float B0(float t){	return -t*t*t/6 + t*t/2 - t/2 + 1.0/6;	};
float B1(float t){	return t*t*t/2 - t*t + 2.0/3;	};
float B2(float t){	return -t*t*t/2 + t*t/2 + t/2 + 1.0/6;	};
float B3(float t){	return t*t*t/6;	};
void drawCoons(Contour C){
	float deltat = 0.1;
	Segment s;
	for(int i=3; i<=C.size()-1; i++){
		float t = 0;
		s.start = (Point){
			C[i-3].x*B0(0) + C[i-2].x*B1(0) + C[i-1].x*B2(0) + C[i].x*B3(0),
			C[i-3].y*B0(0) + C[i-2].y*B1(0) + C[i-1].y*B2(0) + C[i].y*B3(0)
		};
		while(t<1){
			t += deltat;
			s.finish = (Point){
				C[i-3].x*B0(t) + C[i-2].x*B1(t) + C[i-1].x*B2(t) + C[i].x*B3(t),
				C[i-3].y*B0(t) + C[i-2].y*B1(t) + C[i-1].y*B2(t) + C[i].y*B3(t)
			};
			drawSegment(s, main_color);
			s.start = s.finish;
		}
		drawPlus(s.start, BLACK);
	}
}

int triangle_method = 0;
COLORREF colorchooser(float x, float y, Contour C){
	if(triangle_method){ //blend
		
	} else { //closest point
		
	};
	return RED;
}
void fill_sliced_triangle(Contour C){
	Segment e1, e2; float we1, we2;
	float y = e1.start.y; float ymax = e1.finish.y;
	float x1 = e1.start.x; float x2 = e2.start.x;
	if(C[0].y == C[1].y){
		e1 = (Segment){ C[0], C[2] };
		e2 = (Segment){ C[1], C[2] };
		we1 = (C[2].x - C[0].x) / (C[2].y - C[0].y);
		we2 = (C[2].x - C[1].x) / (C[2].y - C[1].y);
		y = e1.start.y;
		ymax = e1.finish.y;
		x1 = e1.start.x;
		x2 = e2.start.x;
	} else {
		e1 = (Segment){ C[0], C[1] };
		e2 = (Segment){ C[0], C[2] };
		we1 = (C[1].x - C[0].x) / (C[1].y - C[0].y);
		we2 = (C[2].x - C[0].x) / (C[2].y - C[0].y);
		y = e2.start.y;
		ymax = e2.finish.y;
		x1 = e1.start.x;
		x2 = e2.start.x;
	};
	
	while(y < ymax){
		for(float i = std::min(x1, x2); i <= std::max(x1, x2); i=i+1){
			DrawPixel(static_cast<int>(i),static_cast<int>(y), colorchooser(i,y,C));
		}
		x1 += we1;
		x2 += we2;
		y+=1;
	}    	
	
}
void fill_triangle(Contour C){
	std::sort(C.begin(), C.end(), [](const Point& a, const Point& b) {
        if (a.y == b.y) return a.x < b.x;
        return a.y < b.y;
    });
    if(C[0].y == C[1].y || C[1].y == C[2].y) {
    	fill_sliced_triangle(C);
	} else { // slicing
		float m = (C[2].y - C[0].y)/(C[2].x - C[0].x);
		Point P = (Point){
			(C[1].y-C[0].y)/m + C[0].x,
			C[1].y
		};
		Contour upper;
		Contour lower;
		if(C[1].x < P.x){
			upper.push_back(C[0]); upper.push_back(C[1]); upper.push_back(P);
			lower.push_back(C[1]); lower.push_back(P); lower.push_back(C[2]);
			fill_sliced_triangle(upper);
			fill_sliced_triangle(lower);
		} else {
			upper.push_back(C[0]); upper.push_back(P); upper.push_back(C[1]);
			lower.push_back(P); lower.push_back(C[1]); lower.push_back(C[2]);
			fill_sliced_triangle(upper);
			fill_sliced_triangle(lower);
		}
	}
};
void fill_poly(Contour C){
	
	
}
void drawContour(  Contour C, COLORREF color){
	Contour E = {
				(Point){0,0}, 
				(Point){DRAW_WIDTH,0}, 
				(Point){DRAW_WIDTH,DRAW_HEIGHT}, 
				(Point){0,DRAW_HEIGHT}, 
				(Point){0,0}
			};
	Segments f;	
	int size = C.size();
	Contour C2;	
			
	switch(PROGRAM_MODE){
    	case MODE_CIRCLES:
    		for(int i=0; i<size-1; i+=2){
    			br_circle(  convertPointToPixel(C[i]), convertPointToPixel(C[i+1]), main_color );
			}
    		break;
    	case MODE_CONTOUR:
			drawRect((Point){0,0}, (Point){DRAW_WIDTH,DRAW_HEIGHT}, RED);
			(size==1) ? drawPlus(C[0], RED) : (void)0	;
			if(size==2){
				C2 = sliceContour(C,E,0); //problematic
				f = convertContourToSegments(C2);
				drawSegments(  f, main_color);
			} else if (size>2){
				C = sliceContour(C,E,0);
				C = flip90(C);
				C = sliceContour(C,E,0);
				C = flip90(C);
				C = sliceContour(C,E,-DRAW_WIDTH);
				C = flip90(C);
				C = sliceContour(C,E,-DRAW_HEIGHT);
				C = flip90(C);
				f = convertContourToSegments(C);
				drawSegments(  f, main_color);
			}
    		break;
    	case MODE_CONTOUR_FILLED:
    		drawRect((Point){0,0}, (Point){DRAW_WIDTH,DRAW_HEIGHT}, RED);
			if(size==2){
				C = sliceContour(C,E,0);
				f = convertContourToSegments(C);
				drawSegments(  f, main_color);
			} else if (size>2){
				C = sliceContour(C,E,0);
				C = flip90(C);
				C = sliceContour(C,E,0);
				C = flip90(C);
				C = sliceContour(C,E,-DRAW_WIDTH);
				C = flip90(C);
				C = sliceContour(C,E,-DRAW_HEIGHT);
				C = flip90(C);
			}
			if(C.size()==3) { fill_triangle(C);} else { fill_poly(C); };
    		break;
    	case MODE_HERMIT_CURVE:
    		if(size>=1) drawPluses(C,RED);
    		if(size>=4 && size%2==0) drawHermit(C);
    		break;
    	case MODE_BEZIER_CURVE:
    		if(size>=1) drawPluses(C,GREEN);
    		if(size>=2) drawBezier(C);
    		break;
    	case MODE_COONS_CURVE:
    		if(size>=1) drawPluses(C,BLUE);
    		if(size>=4) drawCoons(C);
    		break;
	}
	
}

void drawRect( Point A, Point C, COLORREF color){
	Point B = (Point){C.x, A.y};
	Point D = (Point){A.x, C.y};
	drawLine(  A, B, RED);
	drawLine(  B, C, GREEN);
	drawLine(  C, D, BLUE);
	drawLine(  D, A, GREEN);
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



Contour sliceContour(Contour original, Contour edges, int xmin_){
	Contour result;
	int size = original.size();
	if(size==2){ //segment
		Point A = original[0];
		Point B = original[1];
		if (segmentInDraw(A, B)){ //(pointInDraw(A) || pointInDraw(B)){
			Point d = dif(A,B);
			float tl=0, tu=1; //
			int edgesCount = edges.size();//
			for (int i=0; i < edgesCount-1; i++){
				Point normal = orthoVector(edges[i], edges[i+1]);
				float dn = scalarProduct(d, normal), wn = scalarProduct(dif(edges[i],A), normal);
				if(dn!=0){
					float t = -wn / dn;
					if(dn>0 && t<=1){
						tl=std::max(t,tl);
					}
					if(dn<0 && t>=0){
						tu=std::min(t,tu);
					}
				}
			}
			if(tl==0 && tu==1){
				//printf("\n no need in slice");
				return original; 
			} else if(tl<tu){
				Point P1, P2;
				P1.x += A.x + d.x*tl;
				P1.y += A.y + d.y*tl;
				P2.x += A.x + d.x*tu;
				P2.y += A.y + d.y*tu;
				result.push_back(P1);
				result.push_back(P2);
				drawPlus(P1, PINK);
				drawPlus(P2, PINK);
			}
		}
	} else if (size == 1 or size == 0){
		//pass
	} else {
		Contour V = original;
		//push
		//V.pop_back();
		//size = V.size();
		Point S = V[size-1];
		float xmin = xmin_;
		for(int i=0; i<size; i++){
			if(V[i].x >= xmin){
				if(S.x >= xmin){
					result.push_back(V[i]);
				} else {
					Point P = {xmin, S.y + (xmin - S.x)*(V[i].y - S.y)/(V[i].x - S.x)};
					result.push_back(P);
					result.push_back(V[i]);
				
				}
			} else{
				if(S.x >= xmin){
					Point P = {xmin, S.y + (xmin - S.x)*(V[i].y - S.y)/(V[i].x - S.x)};
					result.push_back(P);
				}
			}
			S = V[i];
		}
	}
	return result;
}


#endif // PICASSO_H_INCLUDED
