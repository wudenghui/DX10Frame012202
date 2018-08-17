//=======================================================================================
// Light.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef LIGHT_H
#define LIGHT_H

#include "Directx10.h"

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	D3DXVECTOR3 pos;		// 光源位置
	float pad1;      // not used
	D3DXVECTOR3 dir;		// 光线方向
	float pad2;      // not used
	D3DXCOLOR ambient;	// 环境光强度
	D3DXCOLOR diffuse;	// 漫反射光
	D3DXCOLOR specular;// 高光强度
	D3DXVECTOR3 att;	// 衰减系数
	float spotPow;		// 聚光灯范围参数
	float range;		// 光照范围
	
	bool alive;
};

struct Material
{
	Material()
	{
		ZeroMemory(this, sizeof(Material));
	}

	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	float specPower;
};

#define PARALLEL	0	// 平行光
#define POINT		1	// 点光
#define SPOT		2	// 聚光灯


#endif // LIGHT_H