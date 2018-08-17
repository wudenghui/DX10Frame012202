#include"CrashList.h"

CrashList* gCrashList;	// ��ײ����

CrashList::CrashList()
{
	// ͷ�ڵ�
	pDemoPoint pNew = (pDemoPoint)malloc( sizeof(DemoPoint));
	if(pNew == NULL)
	{
		MessageBox(NULL, "�����ڴ�ʧ��", "List2D���󡪡�ͷ", 50);
		return;
	}
	
	pNew->pPrev = NULL;
	pNew->pContent = NULL;
	
	pHead = pNew;
	
	// β�ڵ�
	pNew = (pDemoPoint)malloc( sizeof(DemoPoint));
	if(pNew == NULL)
	{
		MessageBox(NULL, "�����ڴ�ʧ��", "List2D���󡪡�β", 50);
		return;
	}
	pNew->pNext = NULL;
	pNew->pContent = NULL;
	pTail = pNew;
	
	pHead->pNext = pTail;
	pTail->pPrev = pHead;

	numCrashObj = 0;

	return;
}	// ����DemoList2D()

CrashList::~CrashList()
{
	pDemoPoint pT = pHead->pNext;
	pDemoPoint pP = NULL;
	while(pT != pTail)
	{
		pP = pT->pNext;
		pT->pContent->CrashListReleased();
		pT->pContent = NULL;	// ���ﲢû�������ͷŵ�ͼ���ڴ棬��Ҫ��������Լ��ͷŵ��Լ���ͼ���ڴ�
		free(pT);
		pT = pP;
	}
	pT = pHead = pTail =NULL;
}	// ����ReleaseDemoList2D()

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
}	// ����Insert

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
			// ֻҪ��һ��Ϊ�棬�ͷ�����
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