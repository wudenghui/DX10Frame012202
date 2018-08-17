/*
	�Դ����
	��������
	W��S��A��D���ո�
	���������Ҽ��л����ģʽ
*/

#ifndef CAMERA_H
#define CAMERA_H

#include"Mouse.h"
#include"Keyboard.h"

// �������ģʽ����
extern bool enableArcCamera;

// �������
#define CAMERA_ROTATE_SPEED_X	0.0087266f	// ��λ�����ȣ����������ת�ٶ�
#define CAMERA_ROTATE_SPEED_Y	0.0087266f	// ��λ�����ȣ����������ת�ٶ�
#define CAMERA_SPEED_Z	2.0f		// �����ٶ�

#define CAMERA_WALK_SPEED	0.287266f	// ��λ�����ȣ����������ת�ٶ�
#define CAMERA_STRAFE_SPEED	0.287266f	// ��λ�����ȣ����������ת�ٶ�
#define CAMERA_UP_SPEED		0.287266f	// ��λ�����ȣ����������ת�ٶ�

#define DEF_CAMERA_DISTANT	0.0f
#define MIN_CAMERA_DISTANT	0.0f
#define MAX_CAMERA_DISTANT	0.0f


const D3DXVECTOR3 DEF_TARGET = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
const D3DXVECTOR3 DEF_CAMERA_DIRECTION = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
const D3DXVECTOR3 DEF_CAMERA_UP = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
const D3DXVECTOR3 DEF_CAMERA_RIGHT = D3DXVECTOR3(1.0f, 0.0f, 0.0f);	// ������򣬱����ǵ�λ����

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

	// Arc���
//	D3DXVECTOR3& GetTarget(){	return mTarget;	}

//	void SetDistance( float distance, float minDistance, float maxDistance );
//	void SetTarget(float x, float y, float z);
//	void ApplyZoom( float zoomDelta );

	void SetPosition(float x, float y, float z){	mPosition->x = x, mPosition->y = y, mPosition->z = z;	}
	D3DXVECTOR3& GetLook(){	return *mLook;	}
	D3DXVECTOR3& GetRight(){	return *mRight;	}
	D3DXVECTOR3& GetUp(){	return *mUp;	}

public:
	// ���������ĳһ�������λ��
	void Lock(D3DXVECTOR3* position, D3DXVECTOR3* look, D3DXVECTOR3* right, D3DXVECTOR3* up);

private:
	Camera();
	~Camera();

private:
	ID3D10Effect* mFX;

	bool objLock;	// �Ƿ�����ĳ����
	D3DXVECTOR3* mPosition;
	D3DXVECTOR3* mLook;	// ���������
	D3DXVECTOR3* mRight;
	D3DXVECTOR3* mUp;
	
//	D3DXVECTOR3 mZoom;	// ���������
	// zoom���㹫ʽ��
	// zoom = mPosition - mTarget
	// zoom = -mDistant * mLook;
	// Ϊ�˼����㣬����ÿһ�����������㣬��ʽ��Ϊ����
	// zoom = mTarget - mPosition
	// zoom = mDistant * mLook

	// Arc���
//	D3DXVECTOR3 mTarget;
//	float mDistance, minDistance, maxDistance;
	float xRotation, yRotation, yRotationMin, yRotationMax;
};

Camera& GetCamera();

#endif // DEMO_CAMERA_H