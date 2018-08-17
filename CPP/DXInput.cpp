#include "DxInput.h"

DXInput& GetDXInput()
{
	static DXInput input;
	return input;
}

DXInput::DXInput()
{
	// 输入设备初始化
	HRESULT result = DirectInput8Create( mhAppInst, DIRECTINPUT_VERSION, IID_IDirectInput8, ( void** )&directInput_, 0 );

	if( FAILED( result ) )
	{ 
		MessageBox(0,"DXInput初始化失败",0,0);
		return;
	}
}
DXInput::~DXInput()
{
	ReleaseCOM(directInput_);
}