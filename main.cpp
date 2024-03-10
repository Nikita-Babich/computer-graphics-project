#ifndef UNICODE
#define UNICODE
#endif 

//API
#include <windows.h>
#include <commdlg.h> //colorpicker

//Frequently used
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
//Task specific
#include <iostream>
#include <vector>

//My header,
#include "picasso.h"

////Likely will not be used
//#include <Windows.h>
//#include <tchar.h>
//#include <wingdi.h>
//#include <windowsx.h>



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR     lpCmdLine,
   _In_ int       nCmdShow
){
	
	//Structure with info about app
	WNDCLASSEX wcex;
	wcex.cbSize         = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WindowProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = L"Nick";
	wcex.lpszClassName  = L"Sample Window Class";
	wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	
	//Register window
	if (!RegisterClassEx(&wcex))
	{
	   MessageBox(NULL,
	      L"Call to RegisterClassEx failed!",
	      L"Windows Desktop Guided Tour",
	      MB_OK); //I don't know what parameter uType is for, works with NULL
	
	   return 1;
	}
	
	static TCHAR szWindowClass[] = L"DesktopApp";
	static TCHAR szTitle[] = L"Project";

	const wchar_t CLASS_NAME[]  = L"Sample Window Class";
	HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Nikita Babich graphics project",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Position and size
        //CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        20,20,WINDOW_HEIGHT,WINDOW_WIDTH,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
        
        
	if (!hwnd)
	{
	   MessageBox(NULL,
	      L"Call to CreateWindowEx failed!",
	      L"Windows Desktop Guided Tour",
	      MB_OK);
	
	   return 1;
	}
	
	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hwnd, nCmdShow);
	// Segment for setup
	srand(time(NULL));
	main_contour = rcont();
	
	UpdateWindow(hwnd);
	printf("\nWindow is running");
	
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	  TranslateMessage(&msg);
	  DispatchMessage(&msg);
	}

	return (int) msg.wParam;
};


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int mouseX,mouseY;
    switch (uMsg)
    {
    case WM_DESTROY:
    	printf("\nWindow is closing");
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps); //Handle to Device Context

            // All painting occurs here, between BeginPaint and EndPaint.
			//SetPixel(hdc, 100, 100, RGB(255, 0, 0));
			
			//selectedMethod = Bresenham;
			//br_circle(hdc, rpi(), rpi(), rc() );
			//br_circle(hdc, rpi(), rpi(), rc() );
			//br_circle(hdc, rpi(), rpi(), rc() );
			
			//Figure f1 = rf();
			//drawFigure(hdc, f1, rc()); 
			
			//HDC screenDC = GetDC(NULL);
			drawContour(hdc, main_contour, rc()); //main_color
			//ReleaseDC(NULL, screenDC);
			
			//take shape
			//transform //i don't have to remember original state, so this can be moved away
			//cut by screen edges
			//draw updated shape
			
			
            EndPaint(hwnd, &ps);
        }
        return 0;
        
    case WM_LBUTTONDOWN:
            mouseX = LOWORD(lParam);
            mouseY = HIWORD(lParam);
            printf("\n Left mouse click \t %d %d ",mouseX,mouseY);
            //add a Point to Figure
            switch(PROGRAM_STATE){
            	case INPUT_POINTS:
            		break;
			}
            
            
            break;
            
    case WM_RBUTTONDOWN:
            mouseX = LOWORD(lParam);
            mouseY = HIWORD(lParam);
            printf("\n Right mouse click \t %d %d ",mouseX,mouseY);
            //complete shape 
            PROGRAM_STATE = INPUT_COMPLETE;
            break;
            
    case WM_KEYDOWN:
        // Handle keydown event
        switch (wParam) {
            case VK_LEFT: translateMainContour(LEFT); break;
            case VK_RIGHT: translateMainContour(RIGHT);  break;
            case VK_UP: translateMainContour(UP); break; 
			case VK_DOWN: translateMainContour(DOWN);  break; 
	
			case VK_HOME: 
				// Process the HOME key. 
				break; 
			case VK_END: 
				// Process the END key. 
				break; 
			case VK_INSERT: 
				// Process the INS key. 
				break; 
			case VK_DELETE: 
				// Process the DEL key. 
				break; 
			case VK_F2: 
				// Process the F2 key. 
				break; 
				
			case 'C':
				//OpenColorPicker(hwnd);
				break;
				
			case 'Q': rotateMainContour(LEFT);  break;
			case 'E': rotateMainContour(RIGHT);  break;
			case 'W': scaleMainContour(UP);  break;
			case 'S': scaleMainContour(DOWN);  break;
			case 'D': scaleMainContour(RIGHT);  break;
			case 'A': scaleMainContour(LEFT);  break;
	
			default: 
				// Process other non-character keystrokes. 
				break; 
        }
        redrawAll(hwnd);
        break;

    case WM_KEYUP:
        // Handle keyup event, might not need those
        switch (wParam) {
            case VK_LEFT:
                break;
            case VK_RIGHT:
                break;
        }
        break;        
        
	default:
      	return DefWindowProc(hwnd, uMsg, wParam, lParam);
      	break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
