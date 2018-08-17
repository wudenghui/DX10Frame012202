#include"MeshCopyNew.h"

MeshCopyNew::MeshCopyNew()
{
	
}

MeshCopyNew::~MeshCopyNew()
{
	ReleaseCOM(mMeshData);
	// 这种写法，对应谁声明谁释放原则，否则内存泄露
	// 原理：子类释放掉后，指针变为野指针
	if(mReflectMtrls)
		free(mReflectMtrls), mReflectMtrls = 0;
	if(mDiffuseTextures)
		free(mDiffuseTextures), mDiffuseTextures = 0;
	if(mSpecTextures)
		free(mSpecTextures), mSpecTextures = 0;
	if(mNormalTextures)
		free(mNormalTextures), mNormalTextures = 0;
}

void MeshCopyNew::Copy(Mesh& mesh)
{
	mNumVertices = mesh.GetNumVertices();
	mNumTriangles = mesh.GetNumTriangles();

	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};
	// 克隆Mesh
	mesh.d3dxMesh()->CloneMesh(D3DX10_MESH_32_BIT, "POSITION", vertexDesc, 4, &mMeshData);
	HR(mMeshData->GenerateAdjacencyAndPointReps(0.001f));
	HR(mMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0));
	HR(mMeshData->CommitToDevice());

	mNumSubsets = mesh.GetNumSubsets();

	mDiffuseTextures	=	(ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*mNumSubsets);
	mSpecTextures		=	(ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*mNumSubsets);
	mNormalTextures		=	(ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*mNumSubsets);
	mReflectMtrls		=	(D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3)*mNumSubsets);

	memcpy(mReflectMtrls, mesh.GetReflectMtr(), sizeof(D3DXVECTOR3)*mNumSubsets);
	memcpy(mDiffuseTextures, mesh.GetDiffuseMap(), sizeof(ID3D10ShaderResourceView*)*mNumSubsets);
	memcpy(mSpecTextures, mesh.GetSpecMap(), sizeof(ID3D10ShaderResourceView*)*mNumSubsets);
	memcpy(mNormalTextures, mesh.GetNormMap(), sizeof(ID3D10ShaderResourceView*)*mNumSubsets);

	visible = mesh.GetVisible();
	mCreateWorld = mesh.GetWorldMtx();
	mTexMtx = mesh.GetTexMtx();
	mWVP = mesh.GetWVPMtx();

	mPosition = mesh.GetPosition();
	mScale = mesh.GetScale();
	mRotation = mesh.GetRotation();

	positionXTex = 0.0f, positionYTex = 0.0f;	// 纹理中心点坐标（局部坐标系）
	scaleXTex = 1.0f, scaleYTex = 1.0f;	// 文理缩放参数
	rotationTex = 0.0f;	// 纹理旋转参数

	// FX
	mFX = mesh.GetFX();
	if(mFX == 0)
		return ;
	mTech = mesh.GetTech();
	mShadowTech = mesh.GetShadowTech();

	getFXHandles();

	mExternMeshData = mesh.GetExternMesh();

	mMin = mesh.GetMin(), mMax = mesh.GetMax();
	enableExtern = false;
	mEnablePick = false;

	halfX = mesh.GetHalfX();	// 半x轴宽，为了方便计算重力响应设此变量
	halfY = mesh.GetHalfY();	// 半y轴宽
	halfZ = mesh.GetHalfZ();	// 半Z轴宽

	mShapeType = mesh.GetShapeType();		//形状
	mCrashMtr = mesh.GetCrashMtr();	// 碰撞材质，0.0f表示完全非弹性碰撞，撞到的物体与自身“粘滞”在一起，1.0f表示弹性碰撞，无能量外泄
	mMass = mesh.GetMass();		// 物体的质量（物理学质量）如果质量为负，表示无限大质量
	mSupported = false;	// 此变量说明物体是否受重力影响

	enableCrash = false;	// 碰撞检测开关
}