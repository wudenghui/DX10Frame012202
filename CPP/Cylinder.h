#ifndef CYLINDER_H
#define CYLINDER_H

#include"Mesh.h"

class Cylinder :  public Mesh
{
public:
	Cylinder();
	~Cylinder();

	void Init( 
		float topRadius, float bottomRadius, 
		float height, UINT numSlices, UINT numStacks,
		float pX, float pY, float pZ,		// 坐标
		ID3D10Effect* fx,					// 效果
		char* techName, char* shadowTechName,
		int numSubsets,
		ID3D10ShaderResourceView** diffuseMapRVArray,
		ID3D10ShaderResourceView** specMapRVArray,
		ID3D10ShaderResourceView** normMapRVArray,
		D3DXVECTOR3* reflectMtrArray	// 反射材质
		);

	void CreateMesh(float topRadius, float bottomRadius, 
		float height, UINT numSlices, UINT numStacks, int numSubsets);

private:
	typedef std::vector<Vertex_3D> VertexList;
	typedef std::vector<DWORD> IndexList;
	typedef std::vector<UINT> AtrList;

	void buildStacks(VertexList& vertices, IndexList& indices, AtrList& atrs);
	void buildBottomCap(VertexList& vertices, IndexList& indices, AtrList& atrs);
	void buildTopCap(VertexList& vertices, IndexList& indices, AtrList& atrs);
private:
	float mTopRadius;
	float mBottomRadius;
	float mHeight;
	UINT  mNumSlices;
	UINT  mNumStacks;
};

#endif // CYLINDER_H