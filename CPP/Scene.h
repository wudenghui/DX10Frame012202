#ifndef SCENE
#define SCENE

#include"Camera.h"
#include"DemoList.h"
#include"Sky.h"
#include"LightObj.h"
#include"Mesh.h"

// ��Ӱ�Ӵ���С
#define LIGHT_VIEW_WIDTH	100.0f
#define LIGHT_VIEW_HEIGHT	100.0f

// ��Ӱ�ֱ���
#define SHADOW_MAP_WIDTH	1024
#define SHADOW_MAP_HEIGHT	1024

class Scene : public GameObj
{
public:
	Scene();
	~Scene();

	void ActionUpdate(float dt);
	void DrawContent(); 

	void drawSceneToShadowMap();

	ID3D10ShaderResourceView* GetShadowMap(){	return mShadowMap.depthMap();	}
	ID3D10ShaderResourceView* GetShadowMapColor(){	return mShadowMap.colorMap();	}

	void Init(ID3D10Effect* fx,					// Ч��
		char* techName, char* shadowTechName);

	Scene* GetNextScene(){	return nextScene;	}
	
	DemoList& GetActionList(){	return pActionList;	}
	DemoList& GetAlphaObjList(){	return pAlphaObjectList3D;	}
	DemoList& GetObjList(){	return pObjectList3D;	}
	DemoList& GetReflectMapList(){	return pReflectMapList3D;	}
	DemoList& GetUIList(){	return pUI;	}
	DemoList& GetNoCullList(){	return pNoCullList;	}

	pDemoPoint AddObj(GameObj* obj){	return pObjectList3D.AddObj(obj);	}
	pDemoPoint AddUI(GameObj* obj){	return 	pUI.AddObj(obj);}
	pDemoPoint AddAlphaObj(GameObj* obj){	return pAlphaObjectList3D.AddObj(obj);}
	pDemoPoint AddActionObj(GameObj* obj){	return pActionList.AddObj(obj);	}
	pDemoPoint AddReflectObj(GameObj* obj){	return pReflectMapList3D.AddObj(obj);	}
	pDemoPoint AddNoCullObj(GameObj* obj){	return pNoCullList.AddObj(obj);	}

	Sky& GetEnv(){	return mSky;	}

	void SetTextStr( char* text );
	void InitEnv(ID3D10ShaderResourceView* envMapRV, float radius){	mSky.init(envMapRV, radius);	}
	void SetEnv(ID3D10ShaderResourceView* envMapRV){	mSky.SetMap(envMapRV), mSky.show();	}
	void SetTextErea( RECT* rt ){	memcpy( &textErea, rt, sizeof(RECT));	}
	void SetNextScene( Scene* srcScene ){	nextScene = srcScene;	}
	void SetTexColor(D3DXCOLOR& color){	fontColor = color;	}

public:
	void OpenShadow(LightObj* light);
	// ��Ӱ���
	void CloseShadow(){	enableShadow = false;	}

protected:	
	Scene* nextScene;

	DemoList pActionList;		// ��������Ӧ���޻�ͼ����
	DemoList pNoCullList;		// �ޱ�����������
	DemoList pAlphaObjectList3D;	// 3D͸����������
	DemoList pObjectList3D;	// 3D��������
	DemoList pReflectMapList3D;	// ���价��
	DemoList pUI;

	// ���ֲ���
	char* textContent;
	int textLen;
	int maxTextLen;

	RECT textErea;;
	D3DXCOLOR fontColor;
	ID3DX10Font* mFont;

private:
	// Light���
	Light* mShadowLight;

	D3DXMATRIX mLightView;
	D3DXMATRIX mLightVolume;

protected:
	// ����
	Sky mSky;

protected:

	// RS��դ��״̬
	ID3D10RasterizerState* mRS;

protected:
 
	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10EffectTechnique* mShadowTech;
	ID3D10EffectVariable* mfxShadowLightVar;

	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectScalarVariable* mfxCubeMapEnabledVar;

	// ������ͼ
	ID3D10EffectShaderResourceVariable* mfxCubeMapVar;

	// ��Ӱ��أ�����ɾ��
	bool enableShadow;
	
	DrawableTex2D mShadowMap;

	ID3D10EffectMatrixVariable* mfxLightWVPVar;
	ID3D10EffectShaderResourceVariable* mfxShadowMapVar;

	ID3D10EffectTechnique* mBuildShadowMapTech;
	ID3D10EffectMatrixVariable* mfxBuildShadowMapLightWVPVar;
	ID3D10EffectShaderResourceVariable* mfxBuildShadowMapDiffuseMapVar;
};

extern Scene* curScene;

#endif