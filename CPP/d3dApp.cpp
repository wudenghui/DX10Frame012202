//=======================================================================================
// d3dApp.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "d3dApp.h"

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static D3DApp* app = 0;

	switch( msg )
	{
		case WM_CREATE:
		{
			// Get the 'this' pointer we passed to CreateWindow via the lpParam parameter.
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			app = (D3DApp*)cs->lpCreateParams;
			return 0;
		}
	}

	// Don't start processing messages until after WM_CREATE.
	if( app )
		return app->msgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance)
{
	mhAppInst   = hInstance;
	mhMainWnd   = 0;
	mAppPaused  = false;
	mMinimized  = false;
	mMaximized  = false;
	mResizing   = false;
 
	md3dDevice          = 0;
	mSwapChain          = 0;
	mDepthStencilBuffer = 0;
	mRenderTargetView   = 0;
	mDepthStencilView   = 0;
	
	
	md3dDriverType  = D3D10_DRIVER_TYPE_HARDWARE;
	mClearColor     = CLEAR_COLOR;
	
	initApp();

}

D3DApp::~D3DApp()
{
	// 图形
	DestroyAll();
}

int D3DApp::run()
{
	MSG msg = {0};
 
	GetTime().reset();

	while(msg.message != WM_QUIT)
	{
		if( GetKeyboard().KeyPressed(DIK_ESCAPE) )
		{
			PostQuitMessage( 0 );		// 不错过之后的处理内容（必须这样写，不然内存泄露/访问冲突）
		}
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			GetTime().tick();

			if( !mAppPaused )
				updateScene(GetTime().getDeltaTime());	
			else
				Sleep(50);

			drawScene();
        }
    }
	return (int)msg.wParam;
}

void D3DApp::initApp()
{
	initMainWindow();
	// 图形
	InitAll();
}
 


void D3DApp::updateScene(float dt)
{
	
	////////////////////////////////////////////////
	if(curScene)
		curScene->ActionUpdate(dt);
	///////////////////////////////////////////////////
}

void D3DApp::drawScene()
{
	md3dDevice->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);

	///////////////////////////////////////////////
	if(curScene)
		curScene->DrawContent();
	///////////////////////////////////////////////


	mSwapChain->Present(0, 0);
}

LRESULT D3DApp::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	POSITION mousePos, mOldMousePos;
	int dx = 0;
	int dy = 0;
	switch( msg )
	{
	// WM_ACTIVATE is sent when the window is activated or deactivated.  
	// We pause the game when the window is deactivated and unpause it 
	// when it becomes active.  
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			mAppPaused = true;
			GetTime().stop();
		}
		else
		{
			mAppPaused = false;
			GetTime().start();
		}
		return 0;

	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		mClientWidth  = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);
		if( md3dDevice )
		{
			if( wParam == SIZE_MINIMIZED )
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if( wParam == SIZE_RESTORED )
			{
				
				// Restoring from minimized state?
				if( mMinimized )
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if( mMaximized )
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if( mResizing )
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

	//frameStateIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing  = true;
		GetTime().stop();
		return 0;

	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing  = false;
		GetTime().start();
		OnResize();
		return 0;
 
	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	// The WM_MENUCHAR message is sent when a menu is active and the user presses 
	// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);
 
	// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
		return 0;
	}

	return DefWindowProc(mhMainWnd, msg, wParam, lParam);
}


void D3DApp::initMainWindow()
{
	
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mhAppInst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	mhMainWnd = CreateWindow("D3DWndClassName", WINDOW_NAME, 
		WS_OVERLAPPEDWINDOW, WINDOW_LEFT, WINDOW_TOP, mClientWidth, mClientHeight, 0, 0, mhAppInst, this); 
	if( !mhMainWnd )
	{
		MessageBox(0, "CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);
}