#ifndef BULLET
#define BULLET

#include"BulletObj.h"
#include"MeshCopyNew.h"

#define BULLET_TIME_GAP 0.1f
#define MAX_BULLET_TYPE	7
#define BULLET_LIFE_TIME	1.0f
#define MAX_BULLET_NUM	80	// 最多80发子弹


class Bullet : public GameObj
{
public:
	Bullet();
	~Bullet();

	virtual void ActionUpdate(float dt);

	void SetBulletList(DemoList* list){	mBulletList = list;	}

	bool AddBulletMesh(Mesh& mesh);
	int SetBulletMesh(Mesh& mesh, int index);	// 设置子弹的种类
	void SetBulletMeshIndex(int index){	curBulletTypeIndex = index<numBulletType ?index : numBulletType;	}	// 设置子弹的种类
	void SetSpeed(float speed, int index){	mSpeed[index] = speed;	}
	void CreateOneBullet(D3DXVECTOR3& position, D3DXVECTOR3& direction);	// 创建一个子弹

protected:
	// 子弹种类数组
	MeshCopyNew* bulletTypeArray[MAX_BULLET_TYPE];
	float mSpeed[MAX_BULLET_TYPE];
	int maxBulletNum[MAX_BULLET_TYPE];
	int numBulletNum[MAX_BULLET_TYPE];

	int curBulletTypeIndex;
	int maxBulletType;
	int numBulletType;

	// 子弹速度
	DemoList* mBulletList;

	// 简易的PV操作模拟
	bool existBullet;	// 已存在子弹，PV操作的控制变量
};


#endif
