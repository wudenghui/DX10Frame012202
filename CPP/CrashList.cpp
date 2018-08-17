#include"CrashList.h"

CrashList* gCrashList;	// 碰撞链表

CrashList::CrashList()
{
	// 头节点
	pDemoPoint pNew = (pDemoPoint)malloc( sizeof(DemoPoint));
	if(pNew == NULL)
	{
		MessageBox(NULL, "分配内存失败", "List2D错误——头", 50);
		return;
	}
	
	pNew->pPrev = NULL;
	pNew->pContent = NULL;
	
	pHead = pNew;
	
	// 尾节点
	pNew = (pDemoPoint)malloc( sizeof(DemoPoint));
	if(pNew == NULL)
	{
		MessageBox(NULL, "分配内存失败", "List2D错误——尾", 50);
		return;
	}
	pNew->pNext = NULL;
	pNew->pContent = NULL;
	pTail = pNew;
	
	pHead->pNext = pTail;
	pTail->pPrev = pHead;

	numCrashObj = 0;

	return;
}	// 结束DemoList2D()

CrashList::~CrashList()
{
	pDemoPoint pT = pHead->pNext;
	pDemoPoint pP = NULL;
	while(pT != pTail)
	{
		pP = pT->pNext;
		pT->pContent->CrashListReleased();
		pT->pContent = NULL;	// 这里并没有真正释放掉图像内存，需要物体对象自己释放掉自己的图像内存
		free(pT);
		pT = pP;
	}
	pT = pHead = pTail =NULL;
}	// 结束ReleaseDemoList2D()

pDemoPoint CrashList::AddObj(GameObj* content)
{
	static pDemoPoint pNew;
	pNew = (pDemoPoint)malloc( sizeof(DemoPoint));
	pNew->pContent = content;

	pNew->pPrev = pTail->pPrev;
	pNew->pNext = pTail;

	pTail->pPrev->pNext = pNew;
	pTail->pPrev = pNew;
	
	content->SetCrashListID(pNew);
	content->SetCrashList(this);

	return pNew;
}	// 结束Insert

bool CrashList::Crash(GameObj* obj)
{
	static bool result;
	result = false;
	if(pTail->pPrev == pHead)
		return result;
	pDemoPoint pNew = pHead->pNext;
	while(pNew != pTail)
	{
		if(pNew->pContent != obj)
			// 只要有一项为真，就返回真
			result |= obj->Crash(pNew->pContent);
		pNew = pNew->pNext;
	}
	return result;

}

void CrashList::Delete(pDemoPoint pPoint)
{
	if(pTail->pPrev == pHead)
		return;
	pPoint->pPrev->pNext = pPoint->pNext;
	pPoint->pNext->pPrev = pPoint->pPrev;

	pPoint->pContent->SetCrashList(NULL);
	pPoint->pContent->SetCrashListID(NULL);

	pPoint->pContent = NULL;
	free(pPoint);
}