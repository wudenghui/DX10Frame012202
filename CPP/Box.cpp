#include "Box.h"

Box::Box()
{
}
 
Box::~Box()
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

void Box::Init(
		float length, float width, float height,
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
	else if(numSubsets==4 || numSubsets==5)
		mNumSubsets = 3;
	else if(numSubsets > 6)
		mNumSubsets = 6;
	else
		mNumSubsets = numSubsets;

	CreateMesh(length, width, height, mNumSubsets);

	GetAABB(mMin, mMax);
	mShapeType = BOX;

	LoadMap(mNumSubsets, diffuseMapRVArray, specMapRVArray, normMapRVArray, reflectMtrArray);
}

void Box::CreateMesh(  float length, float width, float height, UINT numSubsets)
{
	UINT mNumVertices = 24;
	UINT mNumTriangles    = 12; // 2 per quad
	
	float L = length / 2.0f;
	float W = width / 2.0f;
	float H = height / 2.0f;

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

	// Create vertex buffer
    Vertex_3D v[24];

	// Fill in the front face vertex data.
 	v[0] = Vertex_3D(-L, -H, -W,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex_3D(-L,  H, -W,
		 1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex_3D( L,  H, -W,
		 1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[3] = Vertex_3D( L, -H, -W,
		 1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex_3D(-L, -H, W,
		 -1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[5] = Vertex_3D( L, -H, W,
		 -1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = Vertex_3D( L,  H, W,
		 -1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[7] = Vertex_3D(-L,  H, W,
		 -1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8]  = Vertex_3D(-L, H, -W,
		 1.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f);
	v[9]  = Vertex_3D(-L, H,  W,
		 1.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f);
	v[10] = Vertex_3D( L, H,  W,
		 1.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		1.0f, 0.0f);
	v[11] = Vertex_3D( L, H, -W,
		 1.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex_3D(-L, -H, -W,
		 -1.0f, 0.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		1.0f, 1.0f);
	v[13] = Vertex_3D( L, -H, -W,
		 -1.0f, 0.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		0.0f, 1.0f);
	v[14] = Vertex_3D( L, -H,  W,
		 -1.0f, 0.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f);
	v[15] = Vertex_3D(-L, -H,  W,
		 -1.0f, 0.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex_3D(-L, -H,  W,
		 0.0f, 0.0f, -1.0f,
		 -1.0f, 0.0f, 0.0f,
		0.0f, 1.0f);
	v[17] = Vertex_3D(-L,  H,  W,
		 0.0f, 0.0f, -1.0f,
		 -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f);
	v[18] = Vertex_3D(-L,  H, -W,
		 0.0f, 0.0f, -1.0f,
		 -1.0f, 0.0f, 0.0f,
		1.0f, 0.0f);
	v[19] = Vertex_3D(-L, -H, -W,
		 0.0f, 0.0f, -1.0f,
		 -1.0f, 0.0f, 0.0f,
		1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex_3D( L, -H, -W,
		 0.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f);
	v[21] = Vertex_3D( L,  H, -W,
		 0.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f);
	v[22] = Vertex_3D( L,  H,  W,
		 0.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f);
	v[23] = Vertex_3D( L, -H,  W,
		 0.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f);


	HR(mMeshData->SetVertexData(0, v));

	// Create the index buffer

	DWORD i[36];
	UINT a[12];

	
	int nGap = 6/numSubsets;

	for(int k=0; k<12; k++)
	{
		UINT t = k/2;
		a[k] = t/nGap;
	}
	
	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	HR(mMeshData->SetIndexData(i, 36));
	HR(mMeshData->SetAttributeData(a));

	HR(mMeshData->GenerateAdjacencyAndPointReps(0.001f));
	HR(mMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0));
	HR(mMeshData->CommitToDevice());
}

