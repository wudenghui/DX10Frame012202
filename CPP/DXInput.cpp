#include "DxInput.h"

DXInput& GetDXInput()
{
	static DXInput input;
	return input;
}

DXInput::DXInput()
{
	// �����豸��ʼ��
	HRESULT result = DirectInput8Create( mhAppInst, DIRECTINPUT_VERSION, IID_IDirectInput8, ( void** )&directInput_, 0 );

	if( FAILED( result ) )
	{ 
		MessageBox(0,"DXInput��ʼ��ʧ��",0,0);
		return;
	}
}
DXInput::~DXInput()
{
	ReleaseCOM(directInput_);
}