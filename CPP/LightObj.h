#ifndef LIGHT_OBJ
#define LIGHT_OBJ

#include"Light.h"
#include"DrawableTex2D.h"


class LightObj
{
public:
	LightObj();
	~LightObj();

	void Init( int type, ID3D10Effect* fx );

	Light* GetContent(){	return &content;	}
	D3DXVECTOR3 GetPos(){	return content.pos;	}
	D3DXVECTOR3 GetDir(){	return content.dir;	}

	ID3D10EffectVariable*	GetMfxLightVar(){	return mfxLightVar;	}

	D3DXVECTOR3 GetLookAt(){	return mLookAt;	}
	int GetType(){	return mType;	}
	void OpenLight();
	void CloseLight();
	void OpenShadow();
	void CloseShadow();
	void UpdateLightBatch();
	void SetLight(Light* light);
	void SetPos(float x, float y, float z);
	void SetPos(D3DXVECTOR3 pos);
	void SetPosBatch(D3DXVECTOR3 pos);
	void SetDirection(float x, float y, float z);
	void SetDirection(D3DXVECTOR3 dir);
	void SetDirectionBatch(D3DXVECTOR3 dir);
	void SetAmbient(D3DXCOLOR ambient);
	void SetAmbient(float r, float g, float b, float a);
	void SetAmbientBatch(D3DXCOLOR ambient);
	void SetDiffuse(D3DXCOLOR diffuse);
	void SetDiffuse(float r, float g, float b, float a);
	void SetDiffuseBatch(D3DXCOLOR diffuse);
	void SetSpecular(D3DXCOLOR spec);
	void SetSpecular(float r, float g, float b, float a);
	void SetSpecularBatch(D3DXCOLOR spec);
	void SetAttribute(D3DXVECTOR3 att);	// 衰减系数
	void SetAttribute(float x, float y, float z);	// 衰减系数
	void SetAttributeBatch(D3DXVECTOR3 att);	// 衰减系数
	void SetSpotPower(float power);
	void SetSpotPowerBatch(float power);
	void SetRange(float range);
	void SetRangeBatch(float range);

protected:
	Light content;
	D3DXVECTOR3 mLookAt;

	int mType;
	ID3D10Effect* mFX;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectScalarVariable* mfxLightShadowEnable;
	ID3D10EffectScalarVariable* mfxLightAlive;
};


#endif