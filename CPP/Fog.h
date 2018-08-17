#ifndef FOG
#define FOG

#include "Directx10.h"

class Fog
{
public:
	Fog();
	~Fog();

	void Init( ID3D10Effect* fx );

	void OpenFog();
	void CloseFog();
	void UpdateFogBatch();

	void SetFX( ID3D10Effect* fx ){	mFX = fx;	}
	void SetFogRange(float range);
	void SetFogRangeBatch(float range);
	void SetFogStart(float start);
	void SetFogStartBatch(float start);
	void SetFogColor(float r, float g, float b);
	void SetFogColorBatch(float r, float g, float b);
	void SetFogColor(D3DXVECTOR3 color);
	void SetFogColorBatch(D3DXVECTOR3 color);
private:
	ID3D10Effect* mFX;

	ID3D10EffectVariable* mfxEnableFog;
	ID3D10EffectVariable* mfxFogStart;
	ID3D10EffectVariable* mfxFogRange;
	ID3D10EffectVariable* mfxFogColor;

	bool fogEnable;
	float fogStart;
	float fogRange;
	D3DXVECTOR3 fogColor;

};

#endif