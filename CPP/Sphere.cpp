#include "Sphere.h"
 
Sphere::Sphere()
: mRadius(0.0f), mNumSlices(0), mNumStacks(0)
{
}

Sphere::~Sphere()
{
}

void Sphere::Init(float radius, UINT numSlices, UINT numStacks,
		float pX, float pY, float pZ,		// 坐标
		ID3D10Effect* fx,					// 效果
		char* techName, char* shadowTechName,
		UINT numSubsets,
		ID3D10ShaderResourceView** diffuseMapRVArray,
		ID3D10ShaderResourceView** specMapRVArray,
		ID3D10ShaderResourceView** normMapRVArray,
		D3DXVECTOR3* reflectMtrArray	// 反射材质
		)
{
	mPosition.x = pX, mPosition.y = pY, mPosition.z = pZ;
	D3DXMatrixTranslation( &mPositionMtx, pX, pY, pZ );

	mScale.x = 1.0f, mScale.y = 1.0f, mScale.z = 1.0f;
	D3DXMatrixScaling(&mScaleMtx, mScale.x, mScale.y, mScale.z);

	mRotation.x = 0.0f, mRotation.y = 0.0f, mRotation.z = 0.0f;
	static D3DXMATRIX rM;
	D3DXMatrixRotationX( &mRotationMtx, mRotation.x );
	D3DXMatrixRotationY( &rM, mRotation.y );
	mRotationMtx *= rM;
	D3DXMatrixRotationZ( &rM, mRotation.z );
	mRotationMtx *= rM;

	mCreateWorld = mRotationMtx * mScaleMtx * mPositionMtx;	// position放在最后
	mWVP = mCreateWorld * mView * mProj_3D;

	mFX = fx;
	if(mFX == 0)
		return ;
	mTech = mFX->GetTechniqueByName(techName);
	mShadowTech = mFX->GetTechniqueByName(shadowTechName);
	getFXHandles();

	if(numSubsets<1)
		return ;
	else
		mNumSubsets = 1;

	CreateMesh(radius, numSlices, numStacks, mNumSubsets);

	GetAABB(mMin, mMax);

	LoadMap(mNumSubsets, diffuseMapRVArray, specMapRVArray, normMapRVArray, reflectMtrArray);

}

void Sphere::CreateMesh(float radius, UINT numSlices, UINT numStacks, UINT numSubsets)
{
	mRadius    = radius;

	mNumSlices = numSlices;
	mNumStacks = numStacks;

	std::vector<Vertex_3D> vertices;
	std::vector<DWORD> indices;
	std::vector<UINT> atrs;

	buildStacks(vertices, indices);

	mNumVertices = vertices.size();
	mNumTriangles = indices.size();
	
	for(UINT i=0; i<mNumTriangles; i++)
	{
		atrs.push_back(0);
	}

	// 输入布局
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};
		// 创建Mesh
	HR(D3DX10CreateMesh(md3dDevice, vertexDesc, 4,
			vertexDesc[0].SemanticName, mNumVertices, 
			mNumTriangles, D3DX10_MESH_32_BIT, &mMeshData));

	HR(mMeshData->SetVertexData(0, &vertices[0]));


	HR(mMeshData->SetIndexData(&indices[0], indices.size()));
	HR(mMeshData->SetAttributeData(&atrs[0]));

	HR(mMeshData->GenerateAdjacencyAndPointReps(0.001f));
	HR(mMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0));
	HR(mMeshData->CommitToDevice());
}

void Sphere::buildStacks(VertexList& vertices, IndexList& indices)
{
	float phiStep = PI/mNumStacks;

	// do not count the poles as rings
	UINT numRings = mNumStacks-1;

	// Compute vertices for each stack ring.
	for(UINT i = 1; i <= numRings; ++i)
	{
		float phi = i*phiStep;

		// vertices of ring
		float thetaStep = 2.0f*PI/mNumSlices;
		for(UINT j = 0; j <= mNumSlices; ++j)
		{
			float theta = j*thetaStep;

			Vertex_3D v;

			// spherical to cartesian
			v.pos.x = mRadius*sinf(phi)*cosf(theta);
			v.pos.y = mRadius*cosf(phi);
			v.pos.z = mRadius*sinf(phi)*sinf(theta);

			// partial derivative of P with respect to theta
			v.tangent.x = -mRadius*sinf(phi)*sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = mRadius*sinf(phi)*cosf(theta);

			D3DXVec3Normalize(&v.normal, &v.pos);

			v.texC.x = theta / (2.0f*PI);
			v.texC.y = phi / PI;

			vertices.push_back( v );
		}
	}

	// poles: note that there will be texture coordinate distortion
	vertices.push_back( Vertex_3D(0.0f, -mRadius, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f) );
	vertices.push_back( Vertex_3D(0.0f, mRadius, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f) );

	UINT northPoleIndex = (UINT)vertices.size()-1;
	UINT southPoleIndex = (UINT)vertices.size()-2;

	UINT numRingVertices = mNumSlices+1;

	// Compute indices for inner stacks (not connected to poles).
	for(UINT i = 0; i < mNumStacks-2; ++i)
	{
		for(UINT j = 0; j < mNumSlices; ++j)
		{
			indices.push_back(i*numRingVertices + j);
			indices.push_back(i*numRingVertices + j+1);
			indices.push_back((i+1)*numRingVertices + j);

			indices.push_back((i+1)*numRingVertices + j);
			indices.push_back(i*numRingVertices + j+1);
			indices.push_back((i+1)*numRingVertices + j+1);
		}
	}

	// Compute indices for top stack.  The top stack was written 
	// first to the vertex buffer.
	for(UINT i = 0; i < mNumSlices; ++i)
	{
		indices.push_back(northPoleIndex);
		indices.push_back(i+1);
		indices.push_back(i);
	}

	// Compute indices for bottom stack.  The bottom stack was written
	// last to the vertex buffer, so we need to offset to the index
	// of first vertex in the last ring.

	UINT baseIndex = (numRings-1)*numRingVertices;
	for(UINT i = 0; i < mNumSlices; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex+i);
		indices.push_back(baseIndex+i+1);
	}
}
 