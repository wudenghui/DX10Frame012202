#include"Player.h"

Player::Player()
{
	xRotation = yRotation = 0.0f;
	mRight    = DEF_CAMERA_RIGHT;
	mUp       = DEF_CAMERA_UP;
	mLook     = DEF_CAMERA_DIRECTION;

	walkSpeed = mLook * CAMERA_WALK_SPEED;
}

Player::~Player()
{
	
}

void Player::ActionUpdate(float dt)
{
	CameraUpdate();
	mBullet.ActionUpdate(dt);

	static float t_base_bullet = 0.0f, t_cur_bullet = 0.0f;
	if(GetMouse().GetState() == LKEY_PRESSED)
	{
		t_cur_bullet = GetTime().getGameTime();
		if( (t_cur_bullet - t_base_bullet) >= BULLET_TIME_GAP)
		{
			mBullet.CreateOneBullet(mPosition, mLook);

			t_base_bullet = t_cur_bullet;
		}
	}

	static float pY, rY, valueN;
	static D3DXVECTOR3 earthNorm = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	static D3DXVECTOR3 look, right, upright, upleft;

	if(enableArcCamera)
	{
		// 自由相机

	}
	else
	{
		right = GetCamera().GetRight() * CAMERA_STRAFE_SPEED;

		look = GetCamera().GetLook();
		valueN = D3DXVec3Dot( &earthNorm, &look);
		walkSpeed = look - valueN*look;
		D3DXVec3Normalize(&walkSpeed, &walkSpeed);
		walkSpeed *= CAMERA_WALK_SPEED;
	}
	
	
	// 物理系统***************************
	// 计算当前速度
	if(mSupported)
		// 位置固定的物体
		;
	else
	{
		// 碰撞检测的过程中必须忽视力的存在，否则不符合自然规律

		// 移动
		if(GetKeyboard().KeyPressed(DIK_W))
		{
			mSpeed.x = walkSpeed.x;
			mSpeed.z = walkSpeed.z;
		}
		else if(GetKeyboard().KeyPressed(DIK_S))
		{
			mSpeed.x = -walkSpeed.x;
			mSpeed.z = -walkSpeed.z;
		}
		else if(GetKeyboard().KeyPressed(DIK_D))
		{
			mSpeed.x = right.x;
			mSpeed.z = right.z;
		}
		else if(GetKeyboard().KeyPressed(DIK_A))
		{
			mSpeed.x = -right.x;
			mSpeed.z = -right.z;
		}
		else
		{
			mSpeed.x = 0.0f;
			mSpeed.z = 0.0f;
		}
		// 跳跃
		if(mEnableJump)
		{
			if(GetKeyboard().KeyPressed(DIK_SPACE))
			{
				mSpeed.y = jumpSpeed;
				mEnableJump = false;
			}
		}

		// 主动检测碰撞的物体
		if(gCrashList->Crash(this));
		else
		{
			if(mSpeed.y + mAccSpeed.y > MAX_SPEED)
				mSpeed.y = MAX_SPEED;
			if(mSpeed.y + mAccSpeed.y < -MAX_SPEED)
				mSpeed.y = -MAX_SPEED;
			else
				mSpeed.y += mAccSpeed.y;
			// 只有主动检测碰撞的物体才会旋转
			Rotate(rotateSpeedX, rotateSpeedY, rotateSpeedZ);
		}

		// 计算当前位置
		// 待补充
		if(mPosition.y + mSpeed.y + halfY >=  SKY_TOP)
			SetSpeed(0.0f, 0.0f, 0.0f);
		else if(mPosition.y + mSpeed.y - halfY <= HORIZON)
			SetSpeed(0.0f, 0.0f, 0.0f);
	}

	
	Translate( mSpeed );
}

void Player::walk()
{
	
	// 应该获取地面法线
}

void Player::jump()
{
	
}

void Player::CameraUpdate()
{
	static float pY;
	pY = GetMouse().GetDeltaY();
	if(pY != 0.0f)
		pitch( pY * CAMERA_ROTATE_SPEED_X );
	static float rY;
	rY = GetMouse().GetDeltaX();
	if(rY != 0.0f)
		rotateY( rY * CAMERA_ROTATE_SPEED_Y );
}

void Player::pitch(float angle)
{
	static D3DXMATRIX R;
	
	static float t;
	t = xRotation + angle;

	if( t > PI_DIV2 || t < -PI_DIV2 )
	{
		// nothing
	}
	else
	{
		xRotation = t;
		D3DXMatrixRotationAxis(&R, &mRight, angle);

		D3DXVec3TransformNormal(&mUp, &mUp, &R);
		D3DXVec3TransformNormal(&mLook, &mLook, &R);
	}
}

void Player::rotateY(float angle)
{
	static D3DXMATRIX R;
	
	static float t;
	t = yRotation + angle;

	if( t > PI )
	{
		yRotation = -PI + atan( mLook.x / mLook.z );

	}
	else if(t < -PI )
	{
		float tan = atan( mLook.x / mLook.z );
		yRotation = PI - atan( mLook.x / mLook.z );
	}
	else
	{
		yRotation = t;
	}
	
	D3DXMatrixRotationY(&R, angle);

	D3DXVec3TransformNormal(&mRight, &mRight, &R);
	D3DXVec3TransformNormal(&mUp, &mUp, &R);
	D3DXVec3TransformNormal(&mLook, &mLook, &R);
	
}