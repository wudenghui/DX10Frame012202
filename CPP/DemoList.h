#ifndef LIST
#define LIST

#include"GameObj.h"

// SceneҲ���Դ����ʵ��



class DemoList : public GameObj
{
public:
	DemoList();
	~DemoList();

	virtual pDemoPoint AddObj(GameObj* content);
	virtual void Delete(pDemoPoint pPoint);
	virtual void DrawContent();
	virtual void ActionUpdate(float dt);

	// ��ӰShadow
	void DrawContentShadow(ID3D10EffectMatrixVariable* mfxLightWVPVar, D3DXMATRIX lightVP);
	void DrawToShadowMap(
		ID3D10EffectMatrixVariable* mfxBuildShadowMapLightWVPVar,
		D3DXMATRIX lightVP,
		ID3D10EffectShaderResourceVariable* diffuseMapVar,
		ID3D10EffectPass* pass);

protected:
	// 2Dͼ������
	pDemoPoint pHead;
	pDemoPoint pTail;
};


#endif