#ifndef BULLET
#define BULLET

#include"BulletObj.h"
#include"MeshCopyNew.h"

#define BULLET_TIME_GAP 0.1f
#define MAX_BULLET_TYPE	7
#define BULLET_LIFE_TIME	1.0f
#define MAX_BULLET_NUM	80	// ���80���ӵ�


class Bullet : public GameObj
{
public:
	Bullet();
	~Bullet();

	virtual void ActionUpdate(float dt);

	void SetBulletList(DemoList* list){	mBulletList = list;	}

	bool AddBulletMesh(Mesh& mesh);
	int SetBulletMesh(Mesh& mesh, int index);	// �����ӵ�������
	void SetBulletMeshIndex(int index){	curBulletTypeIndex = index<numBulletType ?index : numBulletType;	}	// �����ӵ�������
	void SetSpeed(float speed, int index){	mSpeed[index] = speed;	}
	void CreateOneBullet(D3DXVECTOR3& position, D3DXVECTOR3& direction);	// ����һ���ӵ�

protected:
	// �ӵ���������
	MeshCopyNew* bulletTypeArray[MAX_BULLET_TYPE];
	float mSpeed[MAX_BULLET_TYPE];
	int maxBulletNum[MAX_BULLET_TYPE];
	int numBulletNum[MAX_BULLET_TYPE];

	int curBulletTypeIndex;
	int maxBulletType;
	int numBulletType;

	// �ӵ��ٶ�
	DemoList* mBulletList;

	// ���׵�PV����ģ��
	bool existBullet;	// �Ѵ����ӵ���PV�����Ŀ��Ʊ���
};


#endif
