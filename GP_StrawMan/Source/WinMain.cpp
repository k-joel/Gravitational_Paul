/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"WinMain.cpp"
//
// Author: Justin Dumas (JuD)
//
// Last Modified: 9/3/2010
//
// Purpose: This is the main windows program. This will init all main windows stats and pass them to CGame
//			Tech Demo format styled after SGD and WP2 Win Mains, as program and logic evolves this will change to suit the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <vld.h>
//to include CGame here
//#includes for leak detection go here
#include "CGame.h"
//declare leak line here

// debug header
#include "Debug Console/DebugConsole.h"

#include <XInput.h>                       // Defines XBOX controller API
#pragma comment(lib, "XInput.lib")        // Library containing necessary 360
                                          // functions

const char* m_szWindowTitle		= "Gravitational Paul";
const char* m_szClassName		= "SGDWindowClass";
const int	m_nWindow_Width		= 1024;
const int	m_nWindow_Height	= 768;

//check if in debug mode
//full screen in release mode
#ifdef _DEBUG
	const BOOL m_bIsWindowed	= TRUE;
#else
	const BOOL m_bIsWindowed	= FALSE;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “WindowProc”
//
// Purpose: Processes messages sent through the winMain loop. Handles starting fo the program
//			window creation and destruction etc.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//main message handler for the system
	PAINTSTRUCT m_ps;
	HDC			 m_hdc;

	//check message
	switch(msg)
	{
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
		return (0);
		break;
	case WM_CLOSE:
		{
			DestroyWindow(hWnd);
		}
		break;
	case WM_ACTIVATE:
		{
			//if window is gaining focus back
			if(LOWORD(wParam) != WA_INACTIVE)
			{
				//pause game message here(load pause state?)
			}
			else //losing focus (not the main wndow)
			{
				//unpause / pause toggle messgae (pop state?)
			}
		}
		break;
	case WM_CREATE:
		{
			//initialization area
			return (0);
		}
		break;
	case WM_PAINT:
		{
			//window painting begins
			m_hdc = BeginPaint(hWnd, &m_ps);

			//end window painting
			EndPaint(hWnd, &m_ps);
			return (0);
		}
	case WM_DESTROY:
		{
			//end application when destroy posted
			PostQuitMessage(0);
			return(0);
		}
		break;

	default:
		break;
	}

	//process any remaining messages
	return(DefWindowProc(hWnd, msg, wParam, lParam));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CheckIfAlreadyRunning”
//
// Purpose: Function learned in SGD to check if the window program has already been running.
//			Will restore the window if it is already open, rather than starting another
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CheckIfAlreadyRunning(void)
{
	//check if there is a window of this type and name
	HWND hWnd = FindWindow(m_szClassName, m_szWindowTitle);

	//if there is
	if(hWnd)
	{
		//check if it minimized, restore window
		if(IsIconic(hWnd))
			ShowWindow(hWnd, SW_RESTORE);
		//and set it to front window
		SetForegroundWindow(hWnd);

		return TRUE;
	}

	//if no copies running
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “RegisterWindowClass”
//
// Purpose: Registers the widnow class for the program. Sets up member values, and links to needed resources for window icons,
//			cursurs etc
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL RegisterWindowClass(HINSTANCE hInstance)
{
	//describe the window cass
	WNDCLASSEX winClassEx;

	//Window structure members
	winClassEx.cbSize		= sizeof(winClassEx);
	winClassEx.style		= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClassEx.lpfnWndProc	= WindowProc;
	winClassEx.cbClsExtra   = 0;
	winClassEx.cbWndExtra   = 0;
	winClassEx.hInstance	= hInstance;
	winClassEx.hIcon		= LoadIcon(NULL, IDI_APPLICATION); //change this to set program icon
	winClassEx.hIconSm		= NULL;
	winClassEx.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winClassEx.hbrBackground= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClassEx.lpszMenuName = NULL;
	winClassEx.lpszClassName= m_szClassName;

	//register our window class
	return RegisterClassEx(&winClassEx);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “MakeWindow”
//
// Purpose: Functiion learned in SGD to creat a window at the size needed for a specific
//			client rect sizes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND MakeWindow(HINSTANCE hInstance)
{
	//establish window flags
	DWORD dwWindowStyleFlags = WS_VISIBLE;

	if(m_bIsWindowed)
	{
		//if the window is not fulscreen, style set to overlapped
		dwWindowStyleFlags |= WS_OVERLAPPEDWINDOW;
	}
	else
	{
		//if is fullscreen style sets to popup Cursor needed for game
		dwWindowStyleFlags |= WS_POPUP;
	}

	//creat window rect from sizes
	RECT rWindow;
	rWindow.top = 0;
	rWindow.left = 0;
	rWindow.right = m_nWindow_Width;
	rWindow.bottom = m_nWindow_Height;

	//use ex functions to adjust the rect so that our
	//client area is our requested size

	AdjustWindowRectEx(&rWindow, dwWindowStyleFlags, FALSE, WS_EX_APPWINDOW); //this can be changed to add a menu if needed

	//collect the width and heigh from this newly stratched rect
	int nWindowWidth = rWindow.right - rWindow.left;
	int nWindowHeight = rWindow.bottom - rWindow.top;

	//Create and return the window with these set values

	return CreateWindowEx(WS_EX_APPWINDOW, m_szClassName, m_szWindowTitle, dwWindowStyleFlags,
						  (GetSystemMetrics(SM_CXSCREEN)/2) - (nWindowWidth/2),
						  (GetSystemMetrics(SM_CYSCREEN)/2) - (nWindowHeight/2),   //these two tell where to create the window, centered inthis case
						  nWindowWidth, nWindowHeight, NULL, NULL, hInstance, NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “WinMain”
//
// Purpose: This is the main windows prgram loop for the project. this starts, runs, and ends the program, based on messages and
//			events in the game logic
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG		msg; //message used for the main
	HWND	hWnd;//handle to main window

	//memory detaction goes here
	/////////////////////////////

	/////////////////////////////

	//Check for Other instances of game, and prevent them from being created
	if(!hPrevInstance)
	{
		if(CheckIfAlreadyRunning())
			return FALSE;
	}

	//Register the window class for use
	if(!RegisterWindowClass(hInstance)) //if the register returns null quit
		return 0;

	//creates the window;
	hWnd = MakeWindow(hInstance);

	if(!hWnd) //if hwnd is not created quit
		return 0;

	//show the window
	ShowWindow(hWnd, nCmdShow);

	//update the window
	UpdateWindow(hWnd);

	//Game Initialization here
	CGame* pGame = CGame::GetInstance();
	pGame->Init(hWnd, hInstance, m_nWindow_Width, m_nWindow_Height, m_bIsWindowed);
	//SetupDebugConsole(); //removed it cuz it's pretty useless atm

	//mainMessageLoop
	//////////////////////////////

	while(TRUE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//if the messgae is a quit bessage leave game loop
			if(msg.message == WM_QUIT)
				break;

			//Any accelerator message get processed here
			//yay menus and others
			TranslateMessage(&msg);

			//message gets sent to the proc for handling
			DispatchMessage(&msg);
		}

		//check if game is still running
		//if not quit

		if(pGame->Main() == false)
		{
			PostQuitMessage(0);
		}
	}

	//GameSutdown area
	///////////////////////////
	CGame::GetInstance()->Shutdown();

	//Unregister window class

	UnregisterClass(m_szClassName, hInstance);

	//quit back out to windows
	return(int)(msg.wParam);
}