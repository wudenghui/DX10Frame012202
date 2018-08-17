#ifndef MIRROR
#define MIRROR
/*
#include "Light.h"
#include "Box.h"
#include"GameObj.h"


class Mirror : public GameObj
{
public:
	Mirror();
	~Mirror();

	void ActionUpdate(float dt);
	void DrawContent(); 

private:
	void buildFX();
	void buildVertexLayouts();

	void buildRoomGeometry();
	void drawRoom(ID3D10EffectPass* pass);
	void drawMirror(ID3D10EffectPass* pass);
 
private:

	ID3D10Buffer* mRoomVB;
	Box mCrateMesh;

	Light mParallelLight;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;

	ID3D10RasterizerState* mCullCWRS;
	ID3D10BlendState* mDrawReflectionBS;
	ID3D10DepthStencilState* mDrawMirrorDSS;
	ID3D10DepthStencilState* mDrawReflectionDSS;

	ID3D10ShaderResourceView* mCrateDiffuseMapRV;
	ID3D10ShaderResourceView* mFloorDiffuseMapRV;
	ID3D10ShaderResourceView* mWallDiffuseMapRV;
	ID3D10ShaderResourceView* mMirrorDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;

	D3DXMATRIX mCrateWorld;
	D3DXMATRIX mRoomWorld;

	D3DXMATRIX mIdentityTexMtx;

	D3DXMATRIX mView;
	D3DXMATRIX mWVP;

	D3DXVECTOR3 mEyePos;
	float mRadius;
	float mTheta;
	float mPhi;
};

*/
#endif