#include "Cylinder.h"

Cylinder::Cylinder()
: mTopRadius(0.0f), mBottomRadius(0.0f), mHeight(0.0f), mNumSlices(0), mNumStacks(0)
{

}

Cylinder::~Cylinder()
{
	ReleaseCOM(mMeshData);
	if(mReflectMtrls)
		free(mReflectMtrls), mReflectMtrls = 0;
	if(mDiffuseTextures)
		free(mDiffuseTextures), mDiffuseTextures = 0;
	if(mSpecTextures)
		free(mSpecTextures), mSpecTextures = 0;
	if(mNormalTextures)
		free(mNormalTextures), mNormalTextures = 0;
}

void Cylinder::Init( 
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
	else if(numSubsets>3)
		mNumSubsets = 3;
	else
		mNumSubsets = numSubsets;
 	
	CreateMesh(topRadius, bottomRadius, 
		height, numSlices, numStacks, mNumSubsets);

	GetAABB(mMin, mMax);

	mShapeType = CYLINDER;

	LoadMap(mNumSubsets, diffuseMapRVArray, specMapRVArray, normMapRVArray, reflectMtrArray);
}

void Cylinder::CreateMesh(float topRadius, float bottomRadius, 
		float height, UINT numSlices, UINT numStacks, int numSubsets)
{
	mTopRadius    = topRadius;
	mBottomRadius = bottomRadius;
	mHeight       = height;
	mNumSlices    = numSlices;
	mNumStacks    = numStacks;

	std::vector<Vertex_3D> vertices;
	std::vector<DWORD> indices;
	std::vector<UINT> atrs;

	buildStacks(vertices, indices, atrs);
	buildTopCap(vertices, indices, atrs);
	buildBottomCap(vertices, indices, atrs);

	
	mNumVertices = (UINT)vertices.size();
	mNumTriangles    = (UINT)indices.size()/3;

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
	HR(mMeshData->SetVertexData(0,  &vertices[0]));
	
	HR(mMeshData->SetIndexData(&indices[0], indices.size()));
	HR(mMeshData->SetAttributeData(&atrs[0]));

	HR(mMeshData->GenerateAdjacencyAndPointReps(0.001f));
	HR(mMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0));
	HR(mMeshData->CommitToDevice());
}

void Cylinder::buildStacks(VertexList& vertices, IndexList& indices, AtrList& atrs)
{
	float stackHeight = mHeight / mNumStacks;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (mTopRadius - mBottomRadius) / mNumStacks;

	UINT numRings = mNumStacks+1;

	// Compute vertices for each stack ring.
	for(UINT i = 0; i < numRings; ++i)
	{
		float y = -0.5f*mHeight + i*stackHeight;
		float r = mBottomRadius + i*radiusStep;

		// Height and radius of next ring up.
		float y_next = -0.5f*mHeight + (i+1)*stackHeight;
		float r_next = mBottomRadius + (i+1)*radiusStep;
	 
		// vertices of ring
		float dTheta = 2.0f*PI/mNumSlices;
		for(UINT j = 0; j <= mNumSlices; ++j)
		{
			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);

			float u = (float)j/mNumSlices;
			float v = 1.0f - (float)i/mNumStacks;

			// Partial derivative in theta direction to get tangent vector (this is a unit vector).
			D3DXVECTOR3 T(-s, 0.0f, c);

			// Compute tangent vector down the slope of the cone (if the top/bottom 
			// radii differ then we get a cone and not a true cylinder).
			D3DXVECTOR3 P(r*c, y, r*s);
			D3DXVECTOR3 P_next(r_next*c, y_next, r_next*s);
			D3DXVECTOR3 B = P - P_next;
			D3DXVec3Normalize(&B, &B);

			D3DXVECTOR3 N;
			D3DXVec3Cross(&N, &T, &B);
			D3DXVec3Normalize(&N, &N);

			vertices.push_back( Vertex_3D(P.x, P.y, P.z, T.x, T.y, T.z, N.x, N.y, N.z, u, v) );
		}
	}

	UINT numRingVertices = mNumSlices+1;

	// Compute indices for each stack.
	for(UINT i = 0; i < mNumStacks; ++i)
	{
		for(UINT j = 0; j < mNumSlices; ++j)
		{
			indices.push_back(i*numRingVertices + j);
			indices.push_back((i+1)*numRingVertices + j);
			indices.push_back((i+1)*numRingVertices + j+1);
			
			indices.push_back(i*numRingVertices + j);
			indices.push_back((i+1)*numRingVertices + j+1);
			indices.push_back(i*numRingVertices + j+1);

			atrs.push_back(0);
			atrs.push_back(0);
		}
	}
}
 
void Cylinder::buildBottomCap(VertexList& vertices, IndexList& indices, AtrList& atrs)
{
	UINT baseIndex = (UINT)vertices.size();

	// Duplicate cap vertices because the texture coordinates and normals differ.
	float y = -0.5f*mHeight;

	// vertices of ring
	float dTheta = 2.0f*PI/mNumSlices;
	for(UINT i = 0; i <= mNumSlices; ++i)
	{
		float x = mBottomRadius*cosf(i*dTheta);
		float z = mBottomRadius*sinf(i*dTheta);

		// Map [-1,1]-->[0,1] for planar texture coordinates.
		float u = +0.5f*x/mBottomRadius + 0.5f;
		float v = -0.5f*z/mBottomRadius + 0.5f;

		vertices.push_back( Vertex_3D(x, y, z, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, u, v) );
	}

	// cap center vertex
	vertices.push_back( Vertex_3D(0.0f, y, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f) );

	// index of center vertex
	UINT centerIndex = (UINT)vertices.size()-1;

	UINT tAtr;
	if(mNumSubsets == 1)
		tAtr = 0;
	else
		tAtr = 1;

	for(UINT i = 0; i < mNumSlices; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i+1);
		
		atrs.push_back(tAtr);
	}
}

void Cylinder::buildTopCap(VertexList& vertices, IndexList& indices, AtrList& atrs)
{
	UINT baseIndex = (UINT)vertices.size();

	// Duplicate cap vertices because the texture coordinates and normals differ.
	float y = 0.5f*mHeight;

	// vertices of ring
	float dTheta = 2.0f*PI/mNumSlices;
	for(UINT i = 0; i <= mNumSlices; ++i)
	{
		float x = mTopRadius*cosf(i*dTheta);
		float z = mTopRadius*sinf(i*dTheta);

		// Map [-1,1]-->[0,1] for planar texture coordinates.
		float u = +0.5f*x/mTopRadius + 0.5f;
		float v = -0.5f*z/mTopRadius + 0.5f;

		vertices.push_back( Vertex_3D(x, y, z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, u, v) );
	}

	// cap center vertex
	vertices.push_back( Vertex_3D(0.0f, y, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f) );

	// index of center vertex
	UINT centerIndex = (UINT)vertices.size()-1;

	UINT tAtr;
	if(mNumSubsets == 1)
		tAtr = 0;
	else if(mNumSubsets == 2)
		tAtr = 1;
	else
		tAtr = 2;

	for(UINT i = 0; i < mNumSlices; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i+1);
		indices.push_back(baseIndex + i);
		
		atrs.push_back(tAtr);
	}
}