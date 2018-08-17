#ifndef DIRECTX_10
#define DIRECTX_10

#include "d3dUtil.h"

extern CRITICAL_SECTION context_cs;	// �ٽ�����������Ⱦ������
extern ID3D10Device*    md3dDevice;
extern IDXGISwapChain*  mSwapChain;
extern ID3D10Texture2D* mDepthStencilBuffer;
extern D3D10_VIEWPORT mViewPort;
extern ID3D10RenderTargetView* mRenderTargetView;
extern ID3D10DepthStencilView* mDepthStencilView;
extern D3D10_DRIVER_TYPE md3dDriverType;
extern D3DXCOLOR mClearColor;
extern IDXGIAdapter* mAdapterArray[2];	// �Կ��ӿ�����
extern UINT numAdapter;		// �Կ�����

extern bool enableMultSamp;
extern unsigned int multSampQuality;

extern D3DXMATRIX mProj_3D;
extern D3DXMATRIX mProj_2D;
extern D3DXMATRIX mView;

extern 	char* DXInfo;

void InitDX();
void DestroyDX();
void ResetOMTargetsAndViewport();
void OnResize();
void setLens(float fovY, float aspect, float zn, float zf);

// Ч��
extern ID3D10Effect* SkyFX;
extern ID3D10Effect* BuildShadowMapFX;

extern ID3D10Effect* CubeMapFX;
extern ID3D10Effect* SpriteFX;
extern ID3D10Effect* ColorVertexFX;


void InitAllFX();
void DestroyAllFX();


// InputLayout���벼��
extern ID3D10InputLayout* Pos;
extern ID3D10InputLayout* PosColor;
extern ID3D10InputLayout* PosTex;
extern ID3D10InputLayout* PosTanNormalTex;

void InitAllInputLayout();
void DestroyAllInputLayout();

void InitAll();
void DestroyAll();

#endif