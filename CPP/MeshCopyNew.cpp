#include"MeshCopyNew.h"

MeshCopyNew::MeshCopyNew()
{
	
}

MeshCopyNew::~MeshCopyNew()
{
	ReleaseCOM(mMeshData);
	// ����д������Ӧ˭����˭�ͷ�ԭ�򣬷����ڴ�й¶
	// ԭ�������ͷŵ���ָ���ΪҰָ��
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
	// ��¡Mesh
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

	positionXTex = 0.0f, positionYTex = 0.0f;	// �������ĵ����꣨�ֲ�����ϵ��
	scaleXTex = 1.0f, scaleYTex = 1.0f;	// �������Ų���
	rotationTex = 0.0f;	// ������ת����

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

	halfX = mesh.GetHalfX();	// ��x���Ϊ�˷������������Ӧ��˱���
	halfY = mesh.GetHalfY();	// ��y���
	halfZ = mesh.GetHalfZ();	// ��Z���

	mShapeType = mesh.GetShapeType();		//��״
	mCrashMtr = mesh.GetCrashMtr();	// ��ײ���ʣ�0.0f��ʾ��ȫ�ǵ�����ײ��ײ��������������ճ�͡���һ��1.0f��ʾ������ײ����������й
	mMass = mesh.GetMass();		// ���������������ѧ�������������Ϊ������ʾ���޴�����
	mSupported = false;	// �˱���˵�������Ƿ�������Ӱ��

	enableCrash = false;	// ��ײ��⿪��
}