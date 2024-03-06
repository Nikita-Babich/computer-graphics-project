
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
enum LineMethod selectedMethod = DDA;



void drawLine(Point start_f, Point end_f, COLORREF color, enum LineMethod method)
{
	if (method == DDA1) {
		dda1(start, end, color);
	}
	else if (method == DDA2) {
		dda2(start, end, color);
	}
	else if (method == BR) {
		br(start, end, color);
	}
	update();
}
//SetPixel(hdc, 100, 100, RGB(255, 0, 0));