#ifndef QUAD_H
#define QUAD_H

#include "Mesh.h"

// Builds a quad in xz-plane.
class Quad :  public Mesh
{
public:
	Quad();
	~Quad();

	void Init(
		DWORD m, DWORD n, float dx,
		float pX, float pY, float pZ,		// 坐标
		ID3D10Effect* fx,					// 效果
		char* techName, char* shadowTechName,
		UINT numSubsets,
		ID3D10ShaderResourceView** diffuseMapRVArray,
		ID3D10ShaderResourceView** specMapRVArray,
		ID3D10ShaderResourceView** normMapRVArray,
		D3DXVECTOR3* reflectMtrArray	// 反射材质
		);

	void CreateMesh(DWORD m, DWORD n, float dx, UINT numSubsets);

	DWORD GetRow()	{	return mNumRows;	}
	DWORD GetCols()	{	return mNumCols;	}
	float GetDx()	{	return mDx;	}

private:
	DWORD mNumRows;
	DWORD mNumCols;
	float mDx;
};

#endif // QUAD_H
