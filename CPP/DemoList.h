#ifndef LIST
#define LIST

#include"GameObj.h"

// Scene也是自此类的实体



class DemoList : public GameObj
{
public:
	DemoList();
	~DemoList();

	virtual pDemoPoint AddObj(GameObj* content);
	virtual void Delete(pDemoPoint pPoint);
	virtual void DrawContent();
	virtual void ActionUpdate(float dt);

	// 阴影Shadow
	void DrawContentShadow(ID3D10EffectMatrixVariable* mfxLightWVPVar, D3DXMATRIX lightVP);
	void DrawToShadowMap(
		ID3D10EffectMatrixVariable* mfxBuildShadowMapLightWVPVar,
		D3DXMATRIX lightVP,
		ID3D10EffectShaderResourceVariable* diffuseMapVar,
		ID3D10EffectPass* pass);

protected:
	// 2D图像链表
	pDemoPoint pHead;
	pDemoPoint pTail;
};


#endif