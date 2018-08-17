#include"BulletObj.h"


BulletObj::BulletObj()
{
	maxCrashTime = BULLET_CRASH_TIME;		// 子弹的最大穿透次数
	maxLivingTime = BULLET_LIVEING_TIME;	// 最大存活时间

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

	// 物理系统***************************
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
		// 只有主动检测碰撞的物体才会旋转
		Rotate(rotateSpeedX, rotateSpeedY, rotateSpeedZ);
	}

	// 计算当前位置
	// 待补充
	if(mPosition.y + mSpeed.y + halfY >=  SKY_TOP)
		SetSpeed(0.0f, 0.0f, 0.0f);
	else if(mPosition.y + mSpeed.y - halfY <= HORIZON)
		SetSpeed(0.0f, 0.0f, 0.0f);
		
	Translate( mSpeed );

	if(Action)
		Action(dt);
	
}