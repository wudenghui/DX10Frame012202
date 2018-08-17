#include"DemoList.h"

DemoList::DemoList()
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

	return;
}	// ����DemoList2D()

DemoList::~DemoList()
{
	
	pDemoPoint pT = pHead->pNext;
	pDemoPoint pP = NULL;
	while(pT != pTail)
	{
		pP = pT->pNext;
		pT->pContent->ListReleased();
		pT->pContent = NULL;	// ���ﲢû�������ͷŵ�ͼ���ڴ棬��Ҫ��������Լ��ͷŵ��Լ���ͼ���ڴ�
		free(pT);
		pT = pP;
	}
	pT = pHead = pTail =NULL;
}	// ����ReleaseDemoList2D()

pDemoPoint DemoList::AddObj(GameObj* content)
{
	static pDemoPoint pNew;
	pNew = (pDemoPoint)malloc( sizeof(DemoPoint));
	pNew->pContent = content;

	pNew->pPrev = pTail->pPrev;
	pNew->pNext = pTail;

	pTail->pPrev->pNext = pNew;
	pTail->pPrev = pNew;
	
	content->SetListID(pNew);
	content->SetList(this);

	return pNew;
}	// ����Insert

void DemoList::Delete(pDemoPoint  pPoint)
{
	
	if(pTail->pPrev == pHead)
		return;
	pPoint->pPrev->pNext = pPoint->pNext;
	pPoint->pNext->pPrev = pPoint->pPrev;
	
	pPoint->pContent->SetList(NULL);
	pPoint->pContent->SetListID(NULL);
	pPoint->pContent = NULL;
	free(pPoint);
	
}	// ����Delete

void DemoList::DrawContent()
{
	if(pTail->pPrev == pHead)
		return;
	static pDemoPoint pNew;
	pNew = pHead->pNext;
	while(pNew != pTail)
	{
		pNew->pContent->DrawContent();
		pNew = pNew->pNext;
	}
}	// ����Tranverse()


void DemoList::ActionUpdate(float dt)
{
	if(pTail->pPrev == pHead)
		return;
	static pDemoPoint pNew, pT;
	pNew = pHead->pNext;
	while(pNew != pTail)
	{
		pT = pNew->pNext;
		pNew->pContent->ActionUpdate(dt);
		pNew = pT;
	}
}	// ����DrawContent()

void DemoList::DrawContentShadow(ID3D10EffectMatrixVariable* mfxLightWVPVar, D3DXMATRIX lightVP)
{
	if(pTail->pPrev == pHead)
		return;
	static pDemoPoint pNew;
	pNew = pHead->pNext;
	while(pNew != pTail)
	{
		pNew->pContent->DrawContentShadow(mfxLightWVPVar, lightVP);
		pNew = pNew->pNext;
	}
}

void DemoList::DrawToShadowMap(
		ID3D10EffectMatrixVariable* mfxBuildShadowMapLightWVPVar,
		D3DXMATRIX lightVP,
		ID3D10EffectShaderResourceVariable* diffuseMapVar,
		ID3D10EffectPass* pass)
{
	if(pTail->pPrev == pHead)
		return;
	static pDemoPoint pNew;
	pNew = pHead->pNext;
	while(pNew != pTail)
	{
		pNew->pContent->DrawToShadowMap(mfxBuildShadowMapLightWVPVar, lightVP, diffuseMapVar, pass);
		pNew = pNew->pNext;
	}
}
