#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <Windows.h>
#include <tchar.h>

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <wingdi.h>
#include <windowsx.h>

//sources of info
//https://learn.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=msvc-170
//compiler options add to linker: -static-libgcc -lgdi32

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR     lpCmdLine,
   _In_ int       nCmdShow
){
	
	//structure with info about app
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
	
	//Lx is the same as _T(x) ?
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
	static TCHAR szTitle[] = L"Guided Tour";

	// The parameters to CreateWindowEx explained:
	// WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	
	//HWND hWnd = CreateWindowEx(
	//WS_EX_OVERLAPPEDWINDOW,
	//   szWindowClass,
	//   szTitle,
	//   WS_OVERLAPPEDWINDOW,
	//   CW_USEDEFAULT, CW_USEDEFAULT,
	//   500, 100,
	//   NULL,
	//   NULL,
	//   hInstance,
	//   NULL
	//);

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

	printf("hello");
	
	
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
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.
			
			//TCHAR greeting[] = L"Hello, Windows desktop!";
			//TextOut(hdc, 5, 5, greeting, lstrlen(greeting));
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
			
			SetPixel(hdc, 100, 100, RGB(255, 0, 0));
            SetPixel(hdc,50,50,RGB(50,50,50));
			//TextOut(hdc,5, 5,greeting, len(greeting));
			//TextOut(hdc,5, 5, greeting, 50);
            EndPaint(hwnd, &ps);
        }
        return 0;
        
    case WM_LBUTTONDOWN:
            // Handle left button click here
            static TCHAR lckickmessage[] = L"DesktopApp";
            MessageBox(hwnd, L"Left mouse button clicked!", L"Mouse Click", MB_OK);
            break;
            
	default:
      	return DefWindowProc(hwnd, uMsg, wParam, lParam);
      	break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}




