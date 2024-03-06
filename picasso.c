
void drawLine(Point start_float, Point end_float, COLORREF color, enum LineMethod method)
{
	start = convertPointToPixel(start_f);
	end = convertPointToPixel(end_float);
	if (method == DDA1) {
		dda1(start, end, color);
	}
	else if (method == DDA2) {
		//dda2(start, end, color);
	}
	else if (method == Bresenham) {
		//br(start, end, color);
	}
	//update();
}