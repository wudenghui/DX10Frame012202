#ifndef SCENE
#define SCENE

#include"Camera.h"
#include"DemoList.h"
#include"Sky.h"
#include"LightObj.h"
#include"Mesh.h"

// 阴影视窗大小
#define LIGHT_VIEW_WIDTH	100.0f
#define LIGHT_VIEW_HEIGHT	100.0f

// 阴影分辨率
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

	void Init(ID3D10Effect* fx,					// 效果
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
	// 阴影相关
	void CloseShadow(){	enableShadow = false;	}

protected:	
	Scene* nextScene;

	DemoList pActionList;		// 纯动作响应，无绘图操作
	DemoList pNoCullList;		// 无背面消隐链表
	DemoList pAlphaObjectList3D;	// 3D透明物体链表
	DemoList pObjectList3D;	// 3D物体链表
	DemoList pReflectMapList3D;	// 反射环境
	DemoList pUI;

	// 文字部分
	char* textContent;
	int textLen;
	int maxTextLen;

	RECT textErea;;
	D3DXCOLOR fontColor;
	ID3DX10Font* mFont;

private:
	// Light相关
	Light* mShadowLight;

	D3DXMATRIX mLightView;
	D3DXMATRIX mLightVolume;

protected:
	// 环境
	Sky mSky;

protected:

	// RS光栅化状态
	ID3D10RasterizerState* mRS;

protected:
 
	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10EffectTechnique* mShadowTech;
	ID3D10EffectVariable* mfxShadowLightVar;

	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectScalarVariable* mfxCubeMapEnabledVar;

	// 环境贴图
	ID3D10EffectShaderResourceVariable* mfxCubeMapVar;

	// 阴影相关，不可删除
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