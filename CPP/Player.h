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

	// 相机专属
	D3DXVECTOR3 mRight;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mLook;	// 摄像机视向

	float xRotation, yRotation, yRotationMin, yRotationMax;

public:
	// 将摄像机锁定到自己身上
	void LockCamera(){	GetCamera().Lock(&mPosition, &mLook, &mRight, &mUp);	}
	void CameraUpdate();

public:
	Bullet& GetBullet(){	return mBullet;	}

private:
	Bullet mBullet;	// 玩家的子弹

};

#endif