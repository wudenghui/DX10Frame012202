#include"LightObj.h"

LightObj::LightObj()
{
	mType = 0;
}

LightObj::~LightObj()
{
}

void LightObj::Init( int type, ID3D10Effect* fx )
{
	mFX = fx;
	mType = type;
	mLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if(type == PARALLEL)
	{
		mfxLightVar  = mFX->GetVariableByName("gParaLight");
		mfxLightAlive = mFX->GetVariableByName("gParaAlive")->AsScalar();;
		mfxLightShadowEnable = mFX->GetVariableByName("gParaShadowEnable")->AsScalar();

		// Parallel light.
		content.dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
		content.ambient  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		content.diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		content.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if(type == POINT)
	{
		mfxLightVar  = mFX->GetVariableByName("gPointLight");
		mfxLightAlive = mFX->GetVariableByName("gPointAlive")->AsScalar();;
		mfxLightShadowEnable = mFX->GetVariableByName("gPointShadowEnable")->AsScalar();

		// Pointlight--position is changed every frame to animate.
		content.ambient  = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		content.diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		content.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		content.att.x    = 0.0f;
		content.att.y    = 0.1f;
		content.att.z    = 0.0f;
		content.range    = 50.0f;
	}
	else if(type == SPOT)
	{
		mfxLightVar  = mFX->GetVariableByName("gSpotLight");
		mfxLightAlive = mFX->GetVariableByName("gSpotAlive")->AsScalar();;
		mfxLightShadowEnable = mFX->GetVariableByName("gSpotShadowEnable")->AsScalar();

		// Spotlight--position and direction changed every frame to animate.
		content.ambient  = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
		content.diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		content.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		content.att.x    = 1.0f;
		content.att.y    = 0.0f;
		content.att.z    = 0.0f;
		content.spotPow  = 32.0f;
		content.range    =  100000.0f;
		content.pos = D3DXVECTOR3(100.0f, 100.0f, 0.0f);;
		content.dir = D3DXVECTOR3(-10.0f, -10.0f, 0.0f);
		D3DXVec3Normalize(&content.dir, &content.dir);
	}
	else
		MessageBox(0,"灯光格式不支持",0,0);
	
}

void LightObj::OpenLight()
{
	mfxLightAlive->SetBool(true);
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::CloseLight()
{
	mfxLightAlive->SetBool(false);
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::OpenShadow()
{
	mfxLightShadowEnable->SetBool(true);
}
void LightObj::CloseShadow()
{
	mfxLightShadowEnable->SetBool(false);
}

void LightObj::UpdateLightBatch()
{
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}

void LightObj::SetLight(Light* light)
{
	memcpy(&content, light, sizeof(Light));
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}

void LightObj::SetPos(float x, float y, float z)
{
	content.pos = D3DXVECTOR3(x,y,z);
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetPos(D3DXVECTOR3 pos)
{
	content.pos = pos;
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetPosBatch(D3DXVECTOR3 pos)
{
	content.pos = pos;
}

void LightObj::SetDirection(float x, float y, float z)
{
	content.dir = D3DXVECTOR3(x, y,z);
	D3DXVec3Normalize(&content.dir, &content.dir);
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetDirection(D3DXVECTOR3 dir)
{
	content.dir = dir;
	D3DXVec3Normalize(&content.dir, &content.dir);
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetDirectionBatch(D3DXVECTOR3 dir)
{
	content.dir = dir;
	D3DXVec3Normalize(&content.dir, &content.dir);
}
void LightObj::SetAmbient(D3DXCOLOR ambient)
{
	content.ambient = ambient;
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetAmbient(float r, float g, float b, float a)
{
	content.ambient = D3DXCOLOR(r, g, b, a);
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetAmbientBatch(D3DXCOLOR ambient)
{
	content.ambient = ambient;
}

void LightObj::SetDiffuse(D3DXCOLOR diffuse)
{
	content.diffuse = diffuse;
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetDiffuse(float r, float g, float b, float a)
{
	content.diffuse = D3DXCOLOR(r, g, b, a);
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetDiffuseBatch(D3DXCOLOR diffuse)
{
	content.diffuse = diffuse;
}
void LightObj::SetSpecular(D3DXCOLOR spec)
{
	content.specular = spec;
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetSpecular(float r, float g, float b, float a)
{
	content.specular =  D3DXCOLOR(r, g, b, a);
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetSpecularBatch(D3DXCOLOR spec)
{
	content.specular = spec;
}
void LightObj::SetAttribute(D3DXVECTOR3 att)	// 衰减系数
{
	content.att = att;
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetAttribute(float x, float y, float z)	// 衰减系数
{
	content.att = D3DXVECTOR3(x,y,z);
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetAttributeBatch(D3DXVECTOR3 att)	// 衰减系数
{
	content.att = att;
}
void LightObj::SetSpotPower(float power)
{
	content.spotPow = power;
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetSpotPowerBatch(float power)
{
	content.spotPow = power;
}

void LightObj::SetRange(float range)
{
	content.range = range;
	mfxLightVar->SetRawValue( &content, 0, sizeof(Light));
}
void LightObj::SetRangeBatch(float range)
{
	content.range = range;
}
