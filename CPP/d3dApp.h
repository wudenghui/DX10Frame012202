//=======================================================================================
// d3dApp.h by Frank Luna (C) 2008 All Rights Reserved.
//
// Simple Direct3D demo application class.  
// Make sure you link: d3d10.lib d3dx10d.lib dxerr.lib dxguid.lib.
// Link d3dx10.lib for release mode builds instead of d3dx10d.lib.
//=======================================================================================


#ifndef D3DAPP_H
#define D3DAPP_H

#include"Directx10.h"
#include"Music.h"
#include"Scene.h"
#include"Keyboard.h"



class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	WNDCLASS& GetWinClass(){	return wc;	}

	int run();
	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.
	virtual void initApp();
	virtual void updateScene(float dt);
	virtual void drawScene(); 
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	void initMainWindow();
	
protected:
	WNDCLASS wc;
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;
};




#endif // D3DAPP_H