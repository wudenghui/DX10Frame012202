#include"Keyboard.h"


Keyboard& GetKeyboard()
{
	static Keyboard keyboard;
	return keyboard;
}

void Keyboard::ActionUpdate(float dt)
{
	// ����
	memcpy( prevKeyboardKeys_, keyboardKeys_, sizeof( keyboardKeys_ ) );	// ����ǰ������Ϊ֮ǰ���������̣�
	static HRESULT hr;
	hr = keyboardDevice_->GetDeviceState( sizeof( keyboardKeys_ ), ( LPVOID )&keyboardKeys_ );	// ����	

	if( FAILED( hr ) )
	{
		// ��ʰ�����豸�����̣�
		hr = keyboardDevice_->Acquire( );
	}
}

bool Keyboard::KeyPressed( int key )
{
	if( (keyboardKeys_[key] & 0x80) && (prevKeyboardKeys_[key] & 0x80) )
	{
		return true;
	}
	return false;
}

bool Keyboard::KeyClicked( int key )
{
	if( !(keyboardKeys_[key] & 0x80) && (prevKeyboardKeys_[key] & 0x80) )
	{
		return true;
	}
	return false;
}

Keyboard::Keyboard()
{
	Init();
}

void Keyboard::Init()
{
	ZeroMemory( keyboardKeys_, sizeof( keyboardKeys_ ) );
	ZeroMemory( prevKeyboardKeys_, sizeof( prevKeyboardKeys_ ) );

	HRESULT result = GetDXInput().GetDevice()->CreateDevice( GUID_SysKeyboard, &keyboardDevice_, 0 );

	if( FAILED( result ) )
	{ 
		MessageBox(0,"��ʼ�������豸ʧ��",0,0);
		return ;
	}

	result = keyboardDevice_->SetDataFormat( &c_dfDIKeyboard );

	if( FAILED( result ) )
	{
		MessageBox(0,"���ü������ݸ�ʽʧ��",0,0);
		return;
	}

	if(HARD_EXCLUSE)
		result = keyboardDevice_->SetCooperativeLevel( mhMainWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );	// ǰ̨��ռģʽ
	else
		result = keyboardDevice_->SetCooperativeLevel( mhMainWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );	// ǰ̨��ռģʽ

	if( FAILED( result ) )
	{
		MessageBox(0,"���ü���Э��ģʽʧ��",0,0);
		return;
	}

	result = keyboardDevice_->Acquire( );

	if( FAILED( result ) )
	{ 
		MessageBox(0,"��ȡ��������ʧ��",0,0);
		return ;
	}
}
void Keyboard::Release()
{
	ReleaseCOM(keyboardDevice_);
}

Keyboard::~Keyboard()
{
	Release();
}