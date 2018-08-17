#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"

typedef D3DXVECTOR3 Vertex_Pos;
typedef D3DXVECTOR3 Vertex_Norm;
typedef D3DXCOLOR Vertex_Diffuse;
typedef D3DXCOLOR Vertex_Spec;
typedef D3DXVECTOR2 Vertex_Tex;

const UINT numSlot = 1;

struct Vertex_2D
{
	Vertex_2D(){}
	Vertex_2D(
		float x, float y, float z,
		float u, float v
		) : pos(x,y,z), texC(u,v)
	{ }

	D3DXVECTOR3 pos;
	D3DXVECTOR2 texC;
};

struct Vertex_Color
{
	Vertex_Color(){};
	Vertex_Color(
		float x, float y, float z,
		D3DXCOLOR& c
		) : pos(x,y,z), color(c)
	{}
	Vertex_Color(
		float x, float y, float z,
		float r, float g, float b, float a 
		) : pos(x, y, z), color(r, g, b, a)
	{}

	D3DXVECTOR3 pos;
	D3DXCOLOR color;
};

struct Vertex_3D
{
	Vertex_3D(){}
	Vertex_3D(float x, float y, float z, 
		float tx, float ty, float tz,
		float nx, float ny, float nz, 
		float u, float v )
		: pos(x,y,z),
		tangent(tx, ty, tz),
		normal(nx,ny,nz), texC(u,v)
	{}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
};

// ���㷨����
// ���㶥�㷨����
// �����淨����
void ComputeNorm_Vertex( Vertex_Norm* destNorm,	// Ŀ�귨������
	Vertex_Pos* srcPos, DWORD numVertices,	// ����λ�����顢��������
	DWORD* srcIB, DWORD numFaces );			// �����������顢������������

D3DXVECTOR3 ComputeNorm_Face(D3DXVECTOR3* A, D3DXVECTOR3* B, D3DXVECTOR3* C);	// ���������εķ�������

#endif // VERTEX_H