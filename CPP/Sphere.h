#ifndef SPHERE_H
#define SPHERE_H

#include"Mesh.h"

class Sphere : public Mesh
{
public:
	Sphere();
	~Sphere();

	void Init(float radius, UINT numSlices, UINT numStacks,
		float pX, float pY, float pZ,		// 坐标
		ID3D10Effect* fx,					// 效果
		char* techName, char* shadowTechName,
		UINT numSubsets,
		ID3D10ShaderResourceView** diffuseMapRVArray,
		ID3D10ShaderResourceView** specMapRVArray,
		ID3D10ShaderResourceView** normMapRVArray,
		D3DXVECTOR3* reflectMtrArray	// 反射材质
		);

	void CreateMesh(float radius, UINT numSlices, UINT numStacks, UINT numSubsets);

	float GetRadius(){	return mRadius;	}
	UINT GetSlices(){	return mNumSlices;	}
	UINT GetStacks(){	return mNumStacks;	}

protected:
	typedef std::vector<Vertex_3D> VertexList;
	typedef std::vector<DWORD> IndexList;

	void buildStacks(VertexList& vertices, IndexList& indices);

protected:
	float mRadius;
	UINT  mNumSlices;
	UINT  mNumStacks;
};

#endif // SPHERE_H