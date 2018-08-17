#include"Bullet.h"

Bullet::Bullet() : curBulletTypeIndex(0), numBulletType(0), maxBulletType(MAX_BULLET_TYPE),
	existBullet(false)	// Ĭ�ϲ������κ��ӵ�
{
	for(int i=0; i<MAX_BULLET_TYPE; i++)
	{
		bulletTypeArray[i] = (MeshCopyNew*)malloc(sizeof(MeshCopyNew));
		mSpeed[i] = BULLET_SPEED;
		maxBulletNum[i] = MAX_BULLET_NUM;
		numBulletNum[i] = MAX_BULLET_NUM;
	}

	return;
}

Bullet::~Bullet()
{
	for(int i=0; i<MAX_BULLET_TYPE; i++)
		free(bulletTypeArray[i]);
}

bool Bullet::AddBulletMesh(Mesh& mesh)
{
	if(numBulletType >= maxBulletType)
		return false;
	bulletTypeArray[numBulletType]->Copy(mesh);
	numBulletType++;
	return true;
}

int Bullet::SetBulletMesh(Mesh& mesh, int index)	// �����ӵ�������
{
	int ind;
	if(index >= MAX_BULLET_TYPE)
		return -1;
	if(index >= numBulletType)
		ind = numBulletType;
	else
		ind = index;

	bulletTypeArray[ind]->Copy(mesh);
	return ind;
}

void Bullet::CreateOneBullet(D3DXVECTOR3& position, D3DXVECTOR3& direction)	// ����һ���ӵ�
{
	if(numBulletType == 0)
		return;
	if(numBulletNum[curBulletTypeIndex] == 0)
	{
		// û�ӵ���
		static int index1 = GetMusic().GetSoundIndexByName("WAV/empty.wav");
		GetMusic().PlaySound(index1);
		return;;
	}
	static BulletObj* pNew;
	pNew = new BulletObj();
	pNew->Copy( *bulletTypeArray[curBulletTypeIndex] );
	pNew->SetPosition(position);
	pNew->SetSpeed(direction*mSpeed[curBulletTypeIndex]);

	mBulletList->AddObj(pNew);

	static int index = GetMusic().GetSoundIndexByName("WAV/pulse.wav");

	GetMusic().PlaySound(index);
	numBulletNum[curBulletTypeIndex] = numBulletNum[curBulletTypeIndex]-1;
}

void Bullet::ActionUpdate(float dt)
{
	// ������
	if(GetKeyboard().KeyPressed(DIK_R))
	{
		if(numBulletNum[curBulletTypeIndex] < maxBulletNum[curBulletTypeIndex])
		{
			static int index1 = GetMusic().GetSoundIndexByName("WAV/empty.wav");
			numBulletNum[curBulletTypeIndex] = maxBulletNum[curBulletTypeIndex];
			GetMusic().PlaySound(index1);
		}
	}
	if(numBulletNum[curBulletTypeIndex] == 0)
		sprintf(DXInfo, "�ӵ�����ˣ�����R�������ӵ�");
	else
		sprintf(DXInfo, "��ǰ�ӵ���:%d\n", numBulletNum[curBulletTypeIndex]);
}