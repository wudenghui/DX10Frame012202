#ifndef SKYDOME_H
#define SKYDOME_H

#include "Directx10.h"
#include "TextureMgr.h"
#include"Camera.h"

class Sky
{
public:
	Sky();
	~Sky();

	void init(ID3D10ShaderResourceView* cubemap, float radius);
	
	void draw();

	void show(){	visible = true;	}

	void SetMap(ID3D10ShaderResourceView* map){	mCubeMap = map;	}
	ID3D10ShaderResourceView* GetCubeMap(){	return mCubeMap;	}

private:
	Sky(const Sky& rhs);
	Sky& operator=(const Sky& rhs);

private:
	bool visible;

	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	UINT stride;
    UINT offset;

	ID3D10ShaderResourceView* mCubeMap;

	UINT mNumIndices;
 
	ID3D10EffectTechnique* mTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectShaderResourceVariable* mfxCubeMapVar;
};

#endif // SKY_H