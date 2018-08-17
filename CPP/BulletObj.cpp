#include"BulletObj.h"


BulletObj::BulletObj()
{
	maxCrashTime = BULLET_CRASH_TIME;		// �ӵ������͸����
	maxLivingTime = BULLET_LIVEING_TIME;	// �����ʱ��

	numCrashTime = 0;
	mCreateTime = GetTime().getGameTime();
	
}

BulletObj::~BulletObj()
{
	if(mList && listID )
	{
		((DemoList*)mList)->Delete( (pDemoPoint)listID );
	}
}


void BulletObj::ActionUpdate(float dt)
{
	static bool crashed;
	crashed = false;
	static float curTime;
	curTime = GetTime().getGameTime();
	if(curTime - mCreateTime >= maxLivingTime)
	{
		delete(this);
		return ;
	}

	if(numCrashTime == maxCrashTime)
	{
		delete(this);
		return ;
	}

	// ����ϵͳ***************************
	static pDemoPoint pHead, pTail;
	pHead = gCrashList->GetHeadPoint(),
		pTail = gCrashList->GetTailPoint();

	if(pTail->pPrev == pHead)
		return;

	static pDemoPoint pNew;
	pNew = pHead->pNext;
	while(pNew != pTail)
	{
		if(Crash(pNew->pContent))
		{
			crashed = true;
			numCrashTime++;
		}
		
		pNew = pNew->pNext;
	}

	if(crashed)
		;
	else
	{
		if(mSpeed.y + mAccSpeed.y > MAX_SPEED)
			mSpeed.y = MAX_SPEED;
		if(mSpeed.y + mAccSpeed.y < -MAX_SPEED)
			mSpeed.y = -MAX_SPEED;
		else
			mSpeed.y += mAccSpeed.y;
		// ֻ�����������ײ������Ż���ת
		Rotate(rotateSpeedX, rotateSpeedY, rotateSpeedZ);
	}

	// ���㵱ǰλ��
	// ������
	if(mPosition.y + mSpeed.y + halfY >=  SKY_TOP)
		SetSpeed(0.0f, 0.0f, 0.0f);
	else if(mPosition.y + mSpeed.y - halfY <= HORIZON)
		SetSpeed(0.0f, 0.0f, 0.0f);
		
	Translate( mSpeed );

	if(Action)
		Action(dt);
	
}