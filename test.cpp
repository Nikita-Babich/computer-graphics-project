#ifndef UNICODE
#define UNICODE
#endif 

//API
#include <windows.h>

//Frequently used
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Project subparts
//#include <structs.h>
//#include <lines.h>
//#include <transform.h>
//#include <fill.h>



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
	      NULL);
	
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
        20,20,800,800,

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
	      NULL);
	
	   return 1;
	}
	
	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hwnd, nCmdShow);
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
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.
			SetPixel(hdc, 100, 100, RGB(255, 0, 0));
			
			
            EndPaint(hwnd, &ps);
        }
        return 0;
        
    case WM_LBUTTONDOWN:
            // Handle left button click here
            
            // Extract the x and y coordinates from lParam
            mouseX = LOWORD(lParam);
            mouseY = HIWORD(lParam);
            printf("\n left mouse click \t %d %d ",mouseX,mouseY);
            //static TCHAR lckickmessage[] = L"DesktopApp";
            //MessageBox(hwnd, L"Left mouse button clicked!", L"Mouse Click", MB_OK);
            break;
            
    case WM_RBUTTONDOWN:
            // Handle left button click here
            mouseX = LOWORD(lParam);
            mouseY = HIWORD(lParam);
            printf("\n right mouse click \t %d %d ",mouseX,mouseY);
            
            //static TCHAR rckickmessage[] = L"DesktopApp";
            //MessageBox(hwnd, L"Right mouse button clicked!", L"Mouse Click", MB_OK);
            break;
            
	default:
      	return DefWindowProc(hwnd, uMsg, wParam, lParam);
      	break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
