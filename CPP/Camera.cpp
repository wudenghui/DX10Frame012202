#include "Camera.h"

bool enableArcCamera = false;

Camera& GetCamera()
{
	static Camera camera;
	return camera;
}

void Camera::ActionUpdate(float dt)
{
	if(objLock);
	else
	{
	// 相机深度
//	if(GetMouse().GetMouseWheel() < 0.0f)
	//	ApplyZoom(CAMERA_SPEED_Z);
//	else if(GetMouse().GetMouseWheel() > 0.0f)
	//	ApplyZoom(-CAMERA_SPEED_Z);
	
	static float pY;
	pY = GetMouse().GetDeltaY();
	if(pY != 0.0f)
		pitch( pY * CAMERA_ROTATE_SPEED_X );
	static float rY;
	rY = GetMouse().GetDeltaX();
	if(rY != 0.0f)
		rotateY( rY * CAMERA_ROTATE_SPEED_Y );

	// 参考公式：
	// zoom = mTarget - mPosition
	// zoom = mDistant * mLook
//	mZoom = mDistance * mLook;
//	mPosition = mTarget - mZoom;
	}

	rebuildView();
}

Camera::Camera()
{
	objLock = false;
	mPosition = new D3DXVECTOR3(DEF_TARGET);
	xRotation = yRotation = 0.0f;
	mRight    = new D3DXVECTOR3(DEF_CAMERA_RIGHT);
	mUp       = new D3DXVECTOR3(DEF_CAMERA_UP);
	mLook     = new D3DXVECTOR3(DEF_CAMERA_DIRECTION);

	// mTarget = DEF_TARGET;
	// SetDistance( DEF_CAMERA_DISTANT, MIN_CAMERA_DISTANT, MAX_CAMERA_DISTANT );
	// mZoom = DEF_CAMERA_DISTANT*DEF_CAMERA_DIRECTION;
	// 参考公式：
	// zoom = mTarget - mPosition
	// zoom = mDistant * mLook
	// mPosition = mTarget - mZoom;
	
	

	D3DXMatrixIdentity(&mView);
	rebuildView();
}

Camera::~Camera()
{
}

void Camera::Lock(D3DXVECTOR3* position, D3DXVECTOR3* look, D3DXVECTOR3* right, D3DXVECTOR3* up)	// 摄像机锁定某一个物体
{
	if(objLock)
		 mPosition = position, mLook = look, mRight = right, mUp = up;
	else
	{
		objLock = true;
		delete(mPosition), mPosition = position;
		delete(mLook), mLook = look;
		delete(mRight), mRight = right;
		delete(mUp), mUp = up;
	}
}

void Camera::pitch(float angle)
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
		D3DXMatrixRotationAxis(&R, mRight, angle);

		D3DXVec3TransformNormal(mUp, mUp, &R);
		D3DXVec3TransformNormal(mLook, mLook, &R);
	}
}

void Camera::rotateY(float angle)
{
	static D3DXMATRIX R;
	
	static float t;
	t = yRotation + angle;

	if( t > PI )
	{
		yRotation = -PI + atan( mLook->x / mLook->z );

	}
	else if(t < -PI )
	{
		float tan = atan( mLook->x / mLook->z );
		yRotation = PI - atan( mLook->x / mLook->z );
	}
	else
	{
		yRotation = t;
	}
	
	D3DXMatrixRotationY(&R, angle);

	D3DXVec3TransformNormal(mRight, mRight, &R);
	D3DXVec3TransformNormal(mUp, mUp, &R);
	D3DXVec3TransformNormal(mLook, mLook, &R);
	
}

void Camera::rebuildView()
{
	// Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(mLook, mLook);

	D3DXVec3Cross(mUp, mLook, mRight);
	D3DXVec3Normalize(mUp, mUp);

	D3DXVec3Cross(mRight, mUp, mLook);
	D3DXVec3Normalize(mRight, mRight);

	// Fill in the view matrix entries.
	static float x;
	x = -D3DXVec3Dot(mPosition, mRight);
	static float y;
	y = -D3DXVec3Dot(mPosition, mUp);
	static float z;
	z = -D3DXVec3Dot(mPosition, mLook);

	mView(0,0) = mRight->x; 
	mView(1,0) = mRight->y; 
	mView(2,0) = mRight->z; 
	mView(3,0) = x;   

	mView(0,1) = mUp->x;
	mView(1,1) = mUp->y;
	mView(2,1) = mUp->z;
	mView(3,1) = y;  

	mView(0,2) = mLook->x; 
	mView(1,2) = mLook->y; 
	mView(2,2) = mLook->z; 
	mView(3,2) = z;   

	mView(0,3) = 0.0f;
	mView(1,3) = 0.0f;
	mView(2,3) = 0.0f;
	mView(3,3) = 1.0f;
}

/*
void Camera::SetDistance( float distance, float min, float max )
{
	mDistance = distance;
    minDistance = min;
    maxDistance = max;

    if( mDistance < minDistance ) mDistance = minDistance;
    if( mDistance > maxDistance ) mDistance = maxDistance;
}
void Camera::SetTarget(float x, float y, float z)
{
	mTarget = D3DXVECTOR3(x, y, z);
}
void Camera::ApplyZoom( float zoomDelta )
{
	 mDistance += zoomDelta;

    if( mDistance < minDistance ) mDistance = minDistance;
    if( mDistance > maxDistance ) mDistance = maxDistance;
}
*/