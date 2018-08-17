#include "Quad.h"

Quad::Quad()
{
}

Quad::~Quad()
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

void Quad::Init(
		DWORD m, DWORD n, float dx,
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

	DWORD t = (m-1)*(n-1);
	mNumTriangles = t*2;
	
	if(numSubsets < 1)
		return;
	else if(numSubsets > t)
		mNumSubsets = t;
	else
		mNumSubsets = numSubsets;
	
	CreateMesh(m, n, dx, mNumSubsets);

	GetAABB(mMin, mMax);
	mShapeType = QUAD;
	mMass = -1.0f;	// 无限大质量

	LoadMap(mNumSubsets, diffuseMapRVArray, specMapRVArray, normMapRVArray, reflectMtrArray);
}

void Quad::CreateMesh(DWORD m, DWORD n, float dx, UINT numSubsets)
{
	mNumRows  = m;
	mNumCols  = n;

	mNumVertices	= m*n;
	mNumTriangles	= (m-1)*(n-1)*2;


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

	// Create the geometry and fill the vertex buffer. 

	std::vector<Vertex_3D> vertices(mNumVertices);
	float halfWidth = (n-1)*dx*0.5f;
	float halfDepth = (m-1)*dx*0.5f;

	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);
	for(DWORD i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dx;
		for(DWORD j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			vertices[i*n+j].pos     = D3DXVECTOR3(x, 0.0f, z);
			vertices[i*n+j].normal  = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			vertices[i*n+j].tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
			vertices[i*n+j].texC.x = j*du;
			vertices[i*n+j].texC.y = i*dv;
		}
	}
	HR(mMeshData->SetVertexData(0, &vertices[0]));

	// Create the index buffer. 

	std::vector<DWORD> indices(mNumTriangles*3); // 3 indices per face
	std::vector<UINT> atrs(mNumTriangles);

	// Iterate over each quad and compute indices.
	int k = 0;
	for(DWORD i = 0; i < m-1; ++i)
	{
		for(DWORD j = 0; j < n-1; ++j)
		{
			indices[k*6]   = i*n+j;
			indices[k*6+1] = i*n+j+1;
			indices[k*6+2] = (i+1)*n+j;

			indices[k*6+3] = (i+1)*n+j;
			indices[k*6+4] = i*n+j+1;
			indices[k*6+5] = (i+1)*n+j+1;

			atrs.push_back(k%numSubsets);
			atrs.push_back(k%numSubsets);

			k += 1; // next quad
		}
	}

	HR(mMeshData->SetIndexData(&indices[0], indices.size()));
	HR(mMeshData->SetAttributeData(&atrs[0]));

	HR(mMeshData->GenerateAdjacencyAndPointReps(0.001f));
	HR(mMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0));
	HR(mMeshData->CommitToDevice());
}