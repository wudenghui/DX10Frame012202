#ifndef CRASH_LIST
#define CRASH_LIST

#include"GameObj.h"

class CrashList : public GameObj
{
public:
	CrashList();
	~CrashList();

	virtual pDemoPoint AddObj(GameObj* content);
	virtual void Delete(pDemoPoint pPoint);
	
	pDemoPoint GetHeadPoint(){	return pHead;	}
	pDemoPoint GetTailPoint(){	return pTail;	}

	// ��ײ���
	bool Crash(GameObj* obj);

protected:
	// 2Dͼ������
	pDemoPoint pHead;
	pDemoPoint pTail;
	UINT numCrashObj;
};

extern CrashList* gCrashList;	// ��ײ����

#endif