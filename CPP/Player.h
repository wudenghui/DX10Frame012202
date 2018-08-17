#ifndef PLAYER
#define PLAYER

#include"MeshCopy.h"
#include"Bullet.h"

class Player : public MeshCopyNew
{
public:
	Player();
	~Player();

	void ActionUpdate(float dt);

	void pitch(float angle);
	void rotateY(float angle);

	void walk();
	void jump();
private:
	D3DXVECTOR3 walkSpeed;

	// ���ר��
	D3DXVECTOR3 mRight;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mLook;	// ���������

	float xRotation, yRotation, yRotationMin, yRotationMax;

public:
	// ��������������Լ�����
	void LockCamera(){	GetCamera().Lock(&mPosition, &mLook, &mRight, &mUp);	}
	void CameraUpdate();

public:
	Bullet& GetBullet(){	return mBullet;	}

private:
	Bullet mBullet;	// ��ҵ��ӵ�

};

#endif