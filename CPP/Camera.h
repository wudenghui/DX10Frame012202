/*
	自创相机
	操作键：
	W、S、A、D、空格
	鼠标左键、右键切换相机模式
*/

#ifndef CAMERA_H
#define CAMERA_H

#include"Mouse.h"
#include"Keyboard.h"

// 自由相机模式开关
extern bool enableArcCamera;

// 相机参数
#define CAMERA_ROTATE_SPEED_X	0.0087266f	// 单位（弧度）——相机旋转速度
#define CAMERA_ROTATE_SPEED_Y	0.0087266f	// 单位（弧度）——相机旋转速度
#define CAMERA_SPEED_Z	2.0f		// 景深速度

#define CAMERA_WALK_SPEED	0.287266f	// 单位（弧度）——相机旋转速度
#define CAMERA_STRAFE_SPEED	0.287266f	// 单位（弧度）——相机旋转速度
#define CAMERA_UP_SPEED		0.287266f	// 单位（弧度）——相机旋转速度

#define DEF_CAMERA_DISTANT	0.0f
#define MIN_CAMERA_DISTANT	0.0f
#define MAX_CAMERA_DISTANT	0.0f


const D3DXVECTOR3 DEF_TARGET = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
const D3DXVECTOR3 DEF_CAMERA_DIRECTION = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
const D3DXVECTOR3 DEF_CAMERA_UP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
const D3DXVECTOR3 DEF_CAMERA_RIGHT = D3DXVECTOR3(1.0f, 0.0f, 0.0f);	// 相机方向，必须是单位向量

// Simple camera class that lets the viewer explore the 3D scene.
//   -It keeps track of the camera coordinate system relative to the world space
//    so that the view matrix can be obtained.  
//   -It keeps track of the viewing frustum of the camera so that the projection
//    matrix can be obtained.
class Camera : public GameObj
{
public:
	friend Camera& GetCamera();

	D3DXVECTOR3& position(){	return *mPosition;	}

	void ActionUpdate(float dt);


	void pitch(float angle);
	void rotateY(float angle);

	void rebuildView();

	// Arc相关
//	D3DXVECTOR3& GetTarget(){	return mTarget;	}

//	void SetDistance( float distance, float minDistance, float maxDistance );
//	void SetTarget(float x, float y, float z);
//	void ApplyZoom( float zoomDelta );

	void SetPosition(float x, float y, float z){	mPosition->x = x, mPosition->y = y, mPosition->z = z;	}
	D3DXVECTOR3& GetLook(){	return *mLook;	}
	D3DXVECTOR3& GetRight(){	return *mRight;	}
	D3DXVECTOR3& GetUp(){	return *mUp;	}

public:
	// 摄像机锁定某一个物体的位置
	void Lock(D3DXVECTOR3* position, D3DXVECTOR3* look, D3DXVECTOR3* right, D3DXVECTOR3* up);

private:
	Camera();
	~Camera();

private:
	ID3D10Effect* mFX;

	bool objLock;	// 是否锁定某物体
	D3DXVECTOR3* mPosition;
	D3DXVECTOR3* mLook;	// 摄像机视向
	D3DXVECTOR3* mRight;
	D3DXVECTOR3* mUp;
	
//	D3DXVECTOR3 mZoom;	// 摄像机景深
	// zoom计算公式：
	// zoom = mPosition - mTarget
	// zoom = -mDistant * mLook;
	// 为了简化运算，不必每一步都做负运算，公式改为如下
	// zoom = mTarget - mPosition
	// zoom = mDistant * mLook

	// Arc相关
//	D3DXVECTOR3 mTarget;
//	float mDistance, minDistance, maxDistance;
	float xRotation, yRotation, yRotationMin, yRotationMax;
};

Camera& GetCamera();

#endif // DEMO_CAMERA_H