//=======================================================================================
// d3dUtil.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef D3DUTIL_H
#define D3DUTIL_H

// Let VC++ know we are compiling for WinXP and up.  This let's us use
// API functions specific to WinXP (e.g., WM_INPUT API).
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0600 // Vista
#endif

 
// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) || defined(_DEBUG)
	#ifndef D3D_DEBUG_INFO
	#define D3D_DEBUG_INFO
	#endif
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include"GameTimer.h"
#include <d3dx10.h>
#include <dxerr.h>
#include <cassert>
#include <vector>
#include<io.h>
#include<cstdio>
#include<cstdlib>
#include<math.h>
#include<malloc.h>

//*****************************************************************************
// Simple d3d error checker for book demos.
//*****************************************************************************

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
		DXTrace(__FILE__, (DWORD)__LINE__, hr, #x, true); \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 


//*****************************************************************************
// Convenience macro for releasing COM objects.
//*****************************************************************************

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

//*****************************************************************************
// Convenience functions.
//*****************************************************************************

void BuildGeoSphere(
	UINT numSubdivisions,
	float radius,
	std::vector<D3DXVECTOR3>& vertices, 
	std::vector<DWORD>& indices);

// Returns the polar angle of the point (x,y) in [0, 2*PI).
float AngleFromXY(float x, float y); 

// Converts ARGB 32-bit color format to ABGR 32-bit color format.
D3DX10INLINE UINT ARGB2ABGR(UINT argb)
{
	BYTE A = (argb >> 24) & 0xff;
	BYTE R = (argb >> 16) & 0xff;
	BYTE G = (argb >>  8) & 0xff;
	BYTE B = (argb >>  0) & 0xff;

	return (A << 24) | (B << 16) | (G << 8) | (R << 0);
}

// Returns random float in [0, 1).
D3DX10INLINE float RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

// Returns random float in [a, b).
D3DX10INLINE float RandF(float a, float b)
{
	return a + RandF()*(b-a);
}

// Returns random vector on the unit sphere.
D3DX10INLINE D3DXVECTOR3 RandUnitVec3()
{
	D3DXVECTOR3 v(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f));
	D3DXVec3Normalize(&v, &v);
	return v;
}
 
template<typename T>
D3DX10INLINE T Min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
D3DX10INLINE T Max(const T& a, const T& b)
{
	return a > b ? a : b;
}
 
template<typename T>
D3DX10INLINE T Lerp(const T& a, const T& b, float t)
{
	return a + (b-a)*t;
}

template<typename T>
D3DX10INLINE T Clamp(const T& x, const T& low, const T& high)
{
	return x < low ? low : (x > high ? high : x); 
}


// ����
struct RayLine
{
	RayLine(){}
	RayLine(D3DXVECTOR3& p, D3DXVECTOR3 d)
		: pos(p), dir(d)
	{}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;
};


//*****************************************************************************
// Constants
//*****************************************************************************

const float INFINITY = FLT_MAX;
const float PI_MUL_2	 = 3.14159265358979323f+3.14159265358979323f;
const float PI       = 3.14159265358979323f;
const float PI_DIV2  = 3.14159265358979323f/2.0f;
const float MATH_EPS = 0.0001f;

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);

const D3DXCOLOR BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXCOLOR LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXCOLOR DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXCOLOR DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);
const D3DXVECTOR3 UP_DIR( 0.0f, 1.0f, 0.0f);

////////////
const D3DXCOLOR GREY(0.5f, 0.5f, 0.5f, 1.0f);

#define WINDOWED		1
#define WINDOW_NAME		"DX10��Ϸ���"
#define WINDOW_LEFT		100
#define WINDOW_TOP		50
#define CLEAR_COLOR		GREY
#define CLIENT_FONT_COLOR	RED

#define MULT_SAMP		1	// 
#define NUM_MULT_POINT_SAMP	4

// �����ʽ
#define SURFACE_FORM	DXGI_FORMAT_R8G8B8A8_UNORM

// ������Χ
#define SIGHT_EREA	0.25f*PI

// Ч����غ�
#define EFFECT_3D_NAME	"FX/tex.fx"
#define EFFECT_2D_NAME	""
#define EFFECT_SKY_NAME	"FX/sky.fx"
#define EFFECT_CUBE_MAP_NAME	"FX/cubemap.fx"

extern HINSTANCE mhAppInst;
extern HWND      mhMainWnd;

extern int mClientWidth;
extern int mClientHeight;


// Ӳ����ռ���Ʊ���
extern int HARD_EXCLUSE;


// 3Dģʽ����
extern bool MODE_3D;

// Ħ����ϵ��
extern float FricFactor;

// ʰȡ����
extern RayLine mPickRayLine;

// �����������Ϊȫ�ֵģ���������ͼƬ����
void GetPicSize(ID3D10ShaderResourceView* pic, float* width, float* height);

int NumFile(char* fileName, char* form);	// �����ļ���Ŀ�������ļ���Ŀ

void Swap( int* x1, int* x2);
float Min(float x, float y);
float Max(float x, float y);

// ����������ƽ�����ĵ���

// �������һ�������ԳƵ�������������С��
D3DXVECTOR3 ComputeReflectLineToLine(D3DXVECTOR3* pIn, D3DXVECTOR3* pNorm);
D3DXVECTOR3 ComputeCrashSpeed(D3DXVECTOR3* pIn, D3DXVECTOR3* pNorm);
D3DXVECTOR3 ComputeNormPointToLine(D3DXVECTOR3* pIn, D3DXVECTOR3* pDir);
D3DXVECTOR3 ComputeVVecLineToNorm(D3DXVECTOR3* pIn, D3DXVECTOR3* pNorm);

#endif // D3DUTIL_H