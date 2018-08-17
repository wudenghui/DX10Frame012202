#include"Keyboard.h"


Keyboard& GetKeyboard()
{
	static Keyboard keyboard;
	return keyboard;
}

void Keyboard::ActionUpdate(float dt)
{
	// 键盘
	memcpy( prevKeyboardKeys_, keyboardKeys_, sizeof( keyboardKeys_ ) );	// 将当前按键置为之前按键（键盘）
	static HRESULT hr;
	hr = keyboardDevice_->GetDeviceState( sizeof( keyboardKeys_ ), ( LPVOID )&keyboardKeys_ );	// 键盘	

	if( FAILED( hr ) )
	{
		// 重拾输入设备（键盘）
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
		MessageBox(0,"初始化键盘设备失败",0,0);
		return ;
	}

	result = keyboardDevice_->SetDataFormat( &c_dfDIKeyboard );

	if( FAILED( result ) )
	{
		MessageBox(0,"设置键盘数据格式失败",0,0);
		return;
	}

	if(HARD_EXCLUSE)
		result = keyboardDevice_->SetCooperativeLevel( mhMainWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );	// 前台独占模式
	else
		result = keyboardDevice_->SetCooperativeLevel( mhMainWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );	// 前台独占模式

	if( FAILED( result ) )
	{
		MessageBox(0,"设置键盘协作模式失败",0,0);
		return;
	}

	result = keyboardDevice_->Acquire( );

	if( FAILED( result ) )
	{ 
		MessageBox(0,"获取键盘数据失败",0,0);
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