#ifndef BULLET_OBJ
#define BULLET_OBJ

#include"MeshCopy.h"

#define BULLET_LIVEING_TIME	5.0f
#define BULLET_CRASH_TIME	3
#define BULLET_SPEED	0.5f

class BulletObj : public MeshCopy
{
public:
	BulletObj();
	~BulletObj();

	void ActionUpdate(float dt);

	void SetLivingTime(float time){	maxLivingTime = time;	}
	void SetCrashTime(int time){	maxCrashTime = time;	}

private:
	
	int maxCrashTime;		// 子弹的最大穿透次数
	int numCrashTime;

	float maxLivingTime;	// 最大存活时间
	float mCreateTime;
};


#endif