#include"Mouse.h"

char MousePos[512];

Mouse& GetMouse()
{
	static Mouse mouse;
	return mouse;
}

Mouse::Mouse()
{
	InitDevice();
}	// ����Mouse()

Mouse::~Mouse()
{
	Release();
}	// ����~Mouse()

void Mouse::Release()
{
	ReleaseCOM(mouseDevice_);
}

void Mouse::InitDevice()
{
	mCatchObj = false;

	speed = 1.0f;
	position_x = 0.0f;
	position_y = 0.0f;
	delta_x = delta_y = mouseWheel_ = 0.0f;

	state = NORMAL;

	mCursor.SetPosition(position_x, position_y);

	// ��겿��
	HRESULT result = GetDXInput().GetDevice()->CreateDevice( GUID_SysMouse, &mouseDevice_, 0 );

	if( FAILED( result ) )
	{ 
		MessageBox(0,"��ʼ�����ʧ��",0,0);
		return;
	}

	result = mouseDevice_->SetDataFormat( &c_dfDIMouse );

	if( FAILED( result ) )
	{ 
		MessageBox(0,"������������ʽʧ��",0,0);
		return;
	}
	if(HARD_EXCLUSE)
		result = mouseDevice_->SetCooperativeLevel( mhMainWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
	else
		result = mouseDevice_->SetCooperativeLevel( mhMainWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );

	if( FAILED( result ) )
	{ 
		MessageBox(0,"�������Э��ģʽʧ��",0,0);
		return;
	}

	result = mouseDevice_->Acquire( );

	if( FAILED( result ) )
	{ 
		MessageBox(0,"��ȡ�������ʧ��",0,0);
		return;
	}
}

void Mouse::Init(
		ID3D10ShaderResourceView** pa, int np,
		ID3D10Effect* fx,					// Ч��
		char* techName
		)
{

	float w, h;
	GetPicSize( pa[0], &w, &h);
	mCursor.Init( w, h,
		0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		pa, np,
		fx,
		techName);
}

void Mouse::DrawContent()
{
	mCursor.DrawContent();
}

void Mouse::ActionUpdate(float dt)
{

	// ���
	memcpy( &prevMouseState_, &mouseState_, sizeof( mouseState_ ) );		// ���
	static HRESULT hr;
	hr = mouseDevice_->GetDeviceState( sizeof ( mouseState_ ), ( LPVOID ) &mouseState_ );
	while( FAILED( hr ) )
	{
		// ��ʰ�����豸����꣩
		// ��һ����Ҫ�Ǹ���mouseState_������
		hr = mouseDevice_->Acquire( );
	}
	if( ( mouseState_.rgbButtons[0] & 0x80 ) && (prevMouseState_.rgbButtons[0] & 0x80) )
	{
		// �������������ŵ�ʱ����겻�����κ��ж�
		state = LKEY_PRESSED;
	}
	else if( !( mouseState_.rgbButtons[0] & 0x80 ) && (prevMouseState_.rgbButtons[0] & 0x80) )
	{
		state = LKEY_CLICKED;
	}
	else if( ( mouseState_.rgbButtons[1] & 0x80 ) && (prevMouseState_.rgbButtons[1] & 0x80) )
	{
		state = RKEY_PRESSED;
	}
	else if( !( mouseState_.rgbButtons[0] & 0x80 ) && (prevMouseState_.rgbButtons[1] & 0x80) ) 
	{
		state = RKEY_CLICKED;
	}
	else
	{
		state = NORMAL;
	}

	if(mCatchObj)
	{
		mCursor.SetPicIndex(1);
		mCatchObj = false;
	}
	else
		mCursor.SetPicIndex(0);

	if(speed == 1.0f)
	{
		// �Ż�
		delta_x = (float)mouseState_.lX;
		delta_y = (float)mouseState_.lY;
		mouseWheel_ =(float) mouseState_.lZ;
	}
	else
	{
		delta_x = (float)mouseState_.lX * speed;
		delta_y = (float)mouseState_.lY * speed;
		mouseWheel_ =(float) mouseState_.lZ * speed;
	}
	
	// ��ֹ���Խ��
	if(position_x + delta_x < -(mClientWidth/2.0f) )
		delta_x = 0.0f;
	else if(position_x + delta_x > (mClientWidth/2.0f) )
		delta_x = 0.0f;

	if(position_y - delta_y < -(mClientHeight/2.0f) )
		delta_y = 0.0f;
	else if(position_y - delta_y > (mClientHeight/2.0f) )
		delta_y = 0.0f;
	
	if(MODE_3D)
	{
		if(position_x != 0.0f || position_y != 0.0f)
		{
			position_x =0.0f;
			position_y =0.0f;

			mCursor.SetPosition( position_x, position_y);
		}
	}
	else
	{
		position_x += delta_x;
		position_y -= delta_y;

		mCursor.Translation(delta_x, -delta_y);
	}
	
	ComputePickRay(position_x, position_y);

}	// ����MouseUpdate()

void Mouse::SetSpeed(float spd)
{
	speed = spd;
}	// ����SetSpeed()

void Mouse::ComputePickRay(float sx, float sy)
{
	// Compute picking ray in view space.
	static float vx;
	vx = (2.0f*sx/mClientWidth)/mProj_3D(0,0);
	float vy;
	vy = (2.0f*sy/mClientHeight)/mProj_3D(1,1);


	mPickRayLine.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mPickRayLine.dir = D3DXVECTOR3(vx, vy, 1.0f);

	// Tranform to world space.
	static D3DXMATRIX V;

	static D3DXMATRIX inverseV;
	D3DXMatrixInverse(&inverseV, 0, &mView);

	D3DXVec3TransformCoord(&mPickRayLine.pos, &mPickRayLine.pos, &inverseV);
	D3DXVec3TransformNormal(&mPickRayLine.dir, &mPickRayLine.dir, &inverseV);
}