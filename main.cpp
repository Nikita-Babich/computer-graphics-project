#ifndef UNICODE
#define UNICODE
#endif 

//API
#include <windows.h>
#include <commdlg.h>
#include <WindowsX.h>

//Frequently used
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Task specific
#include <iostream>
#include <vector>
#include <algorithm>
#include <cfloat>

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
        20,20,WINDOW_WIDTH,WINDOW_HEIGHT,

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
	
	// Segment for setup
	InitializeBitmapInfo(&bmi);
	InitializeBuffer();
	
	ShowWindow(hwnd, nCmdShow);
	srand(time(NULL));
	main_contour = rcont(6);
	//main_color = (COLORREF){255,0,0};
	
	//PAINTSTRUCT ps;
    //HDC hdc;
	//UPDATE;
	
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
	//HDC scrDC;
	//PAINTSTRUCT ps;
    //HDC hdc;
    
    switch (uMsg)
    {
    case WM_DESTROY:
    	printf("\nWindow is closing");
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
        	InitializeBuffer();
        	
			drawContour( main_contour, BLUE); 
			
			UPDATE;
        }
        return 0;
        
    case WM_LBUTTONDOWN:
            mouseX = LOWORD(lParam);
            mouseY = HIWORD(lParam);
            printf("\n Left mouse click \t %d %d ",mouseX,mouseY);
            
            Point new_point;
            new_point.x = mouseX;
            new_point.y = mouseY;
            main_contour.push_back(new_point);
            		
            //add a Point to Figure
//            switch(PROGRAM_MODE){
//            	case MODE_CIRCLES:
//            		new_point.x = mouseX;
//            		new_point.y = mouseY;
//            		main_contour.push_back(new_point);
//            		break;
//            	case MODE_CONTOUR:
//            		new_point.x = mouseX;
//            		new_point.y = mouseY;
//            		main_contour.push_back(new_point);
//            		break;
//            	case MODE_CONTOUR_FILLED:
//            		break;
//            	case MODE_HERMIT_CURVE:
//            		break;
//            	case MODE_HERMIT_CURVE:
//            		break;
//            	case MODE_HERMIT_CURVE:
//            		break;
//			}
			InvalidateRect(hwnd, NULL, FALSE);
            break;
            
    case WM_MOUSEMOVE: 
    		{	
    		
    		}
    		break;
            
    case WM_RBUTTONDOWN:
    		main_contour.clear();
			InvalidateRect(hwnd, NULL, FALSE);
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
				//random_main_color();
				OpenColorPicker(hwnd,0);
				break;
			case 'V':
				//random_main_color();
				OpenColorPicker(hwnd,1);
				break;
				
			case 'Q': rotateMainContour(LEFT);  break;
			case 'E': rotateMainContour(RIGHT);  break;
			
			case 'W': scaleMainContour(UP);  break;
			case 'S': scaleMainContour(DOWN);  break;
			case 'D': scaleMainContour(RIGHT);  break;
			case 'A': scaleMainContour(LEFT);  break;
			
			case 'T': shearMainContour(UP);  break;
			case 'G': shearMainContour(DOWN);  break;
			case 'H': shearMainContour(RIGHT);  break;
			case 'F': shearMainContour(LEFT);  break;
			
			case 'B': triangle_method = (triangle_method+1)%2;  break;
			case 'R': main_contour = rcont(10);  break;
			case 'X': symmetryMainContour();  break;
			case 'Z': selectedMethod = static_cast<LineMethod>((selectedMethod + 1)%3); printf("\nmethod %d", selectedMethod); break;
			case 'M': PROGRAM_MODE = static_cast<progState>((PROGRAM_MODE + 1)%6); break;
			default: 
				// Process other non-character keystrokes. 
				break; 
        }
        InvalidateRect(hwnd, NULL, FALSE);
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
