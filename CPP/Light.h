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

	D3DXVECTOR3 pos;		// ��Դλ��
	float pad1;      // not used
	D3DXVECTOR3 dir;		// ���߷���
	float pad2;      // not used
	D3DXCOLOR ambient;	// ������ǿ��
	D3DXCOLOR diffuse;	// �������
	D3DXCOLOR specular;// �߹�ǿ��
	D3DXVECTOR3 att;	// ˥��ϵ��
	float spotPow;		// �۹�Ʒ�Χ����
	float range;		// ���շ�Χ
	
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

#define PARALLEL	0	// ƽ�й�
#define POINT		1	// ���
#define SPOT		2	// �۹��


#endif // LIGHT_H