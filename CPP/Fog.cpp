#include"Fog.h"


Fog::Fog() : fogEnable( false ), fogStart( 10.0f ), fogRange( 140.0f ), fogColor( D3DXVECTOR3( 0.7f, 0.7f, 0.7f) )
{
	
}

Fog::~Fog()
{
	
}

void Fog::Init( ID3D10Effect* fx )
{
	mFX = fx;
	mfxEnableFog	= mFX->GetVariableByName("enableFog");
	mfxFogStart		= mFX->GetVariableByName("gFogStart");
	mfxFogRange		= mFX->GetVariableByName("gFogRange");
	mfxFogColor		= mFX->GetVariableByName("gFogColor");
}

void Fog::OpenFog()
{
	fogEnable = true;
	mfxEnableFog->SetRawValue(&fogEnable, 0, sizeof(bool));
	mfxFogStart->SetRawValue(&fogStart, 0, sizeof(float));
	mfxFogRange->SetRawValue(&fogRange, 0, sizeof(float));
	mfxFogColor->SetRawValue(&fogColor, 0, sizeof(D3DXVECTOR3));
}
void Fog::CloseFog()
{
	fogEnable = false;
	mfxEnableFog->SetRawValue(&fogEnable, 0, sizeof(bool));
}
void Fog::UpdateFogBatch()
{
	mfxFogStart->SetRawValue(&fogStart, 0, sizeof(float));
	mfxFogRange->SetRawValue(&fogRange, 0, sizeof(float));
	mfxFogColor->SetRawValue(&fogColor, 0, sizeof(D3DXVECTOR3));
}

void Fog::SetFogRange(float range)
{
	fogRange = range;
	mfxFogStart->SetRawValue(&fogStart, 0, sizeof(float));
	mfxFogRange->SetRawValue(&fogRange, 0, sizeof(float));
	mfxFogColor->SetRawValue(&fogColor, 0, sizeof(D3DXVECTOR3));
}

void Fog::SetFogRangeBatch(float range)
{
	fogRange = range;
}

void Fog::SetFogStart(float start)
{
	fogStart = start;
	mfxFogStart->SetRawValue(&fogStart, 0, sizeof(float));
	mfxFogRange->SetRawValue(&fogRange, 0, sizeof(float));
	mfxFogColor->SetRawValue(&fogColor, 0, sizeof(D3DXVECTOR3));
}

void Fog::SetFogStartBatch(float start)
{
	fogStart = start;
}

void Fog::SetFogColor(float r, float g, float b)
{
	fogColor = D3DXVECTOR3(r, g, b);
	mfxFogStart->SetRawValue(&fogStart, 0, sizeof(float));
	mfxFogRange->SetRawValue(&fogRange, 0, sizeof(float));
	mfxFogColor->SetRawValue(&fogColor, 0, sizeof(D3DXVECTOR3));
}

void Fog::SetFogColorBatch(float r, float g, float b)
{
	fogColor = D3DXVECTOR3(r, g, b);
}

void Fog::SetFogColor(D3DXVECTOR3 color)
{
	fogColor = color;
	mfxFogColor->SetRawValue(&fogColor, 0, sizeof(D3DXVECTOR3));
}

void Fog::SetFogColorBatch(D3DXVECTOR3 color)
{
	fogColor = color;
}