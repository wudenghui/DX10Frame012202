#ifndef MESH_COPY
#define MESH_COPY

#include"Mesh.h"

class MeshCopy : public GameObj
{
public:
	MeshCopy();
	~MeshCopy();

	void Copy(Mesh& mesh);
	ID3DX10Mesh*				d3dxMesh(){	return mMeshData;	}
	ID3D10Effect*				GetFX(){	return mFX;	}
	ID3D10EffectTechnique*		GetTech(){	return mTech;	}
	ID3D10EffectTechnique*		GetShadowTech(){	return	mShadowTech;	}
	UINT						GetNumSubsets(){	return mNumSubsets;	}
	UINT						GetNumVertices(){	return mNumVertices;	}
	UINT						GetNumTriangles(){	return mNumTriangles;	}
	D3DXVECTOR3*				GetReflectMtr(){	return mReflectMtrls;	}
	ID3D10ShaderResourceView**	GetDiffuseMap(){	return mDiffuseTextures;	}
	ID3D10ShaderResourceView**	GetSpecMap(){	return mSpecTextures;	}
	ID3D10ShaderResourceView**	GetNormMap(){	return mNormalTextures;	}
	D3DXMATRIX								GetWVPMtx(){	return mWVP;	}
	D3DXMATRIX								GetTexMtx(){	return mTexMtx;	}
	D3DXMATRIX								GetWorldMtx(){	return  mCreateWorld;	}
	bool									GetVisible(){	return visible;	}
	float	GetX(){	return mPosition.x;	}
	float	GetY(){	return mPosition.y;	}
	float	GetZ(){	return mPosition.z;	}
	float	GetRotationX(){	return mRotation.x;	}
	float	GetRotationY(){	return mRotation.y;	}
	float	GetRotationZ(){	return mRotation.z;	}
	D3DXVECTOR3	GetPosition(){	return mPosition;	}
	D3DXVECTOR3	GetScale(){	return mScale;	}
	D3DXVECTOR3	GetRotation(){	return mRotation;	}
	D3DXVECTOR2	GetPositionTex(){	return D3DXVECTOR2(positionXTex, positionYTex);	}
	D3DXVECTOR2	GetScaleTex(){	return D3DXVECTOR2(scaleXTex, scaleYTex);	}
	float		GetRotationTex(){	return rotationTex;	}

	D3DXVECTOR3 GetAccSpeed(){	return mAccSpeed;	}
public:

	virtual void ActionUpdate(float dt);
	void DrawContent();
	
	void DrawContentShadow(ID3D10EffectMatrixVariable* mfxLightWVPVar, D3DXMATRIX lightVP);
	void DrawToShadowMap(
		ID3D10EffectMatrixVariable* mfxBuildShadowMapLightWVPVar,
		D3DXMATRIX lightVP,
		ID3D10EffectShaderResourceVariable* diffuseMapVar,
		ID3D10EffectPass* pass);
	
public:
	void Show(){	visible = true;	}
	void Hide(){	visible = false; }

	// ����������
	void SetPosition( D3DXVECTOR3& position );
	void SetPosition( float pX, float pY, float pZ );
	void SetScale( float sX, float sY, float sZ );
	void SetRotation( float rX, float rY, float rZ );

	void Translate( D3DXVECTOR3& d );
	void Translate( float x, float y, float z );
	void Scale( float sX, float sY, float sZ );
	void RotateX( float rX );
	void RotateY( float rY );
	void RotateZ( float rZ );
	void Rotate(float rX, float rY, float rZ);

public:
	// ����������
	void SetTexMatrix(D3DXMATRIX texMatrix){	mTexMtx = texMatrix;	}
	void SetPositionTex(float x, float y);
	void SetScaleTex( float sX, float sY );
	void SetRotationTex( float rotation );
	void TranslationTex( float x, float y );
	void ScaleTex( float sX, float sY );
	void RotateTex( float rotation );

protected:
	void getFXHandles();
protected:
	ID3DX10Mesh* mMeshData;
	UINT mNumVertices;
	UINT mNumTriangles;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10EffectTechnique* mShadowTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVectorVariable* mfxReflectMtrlVar;
	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectShaderResourceVariable* mfxNormalMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;

	UINT mNumSubsets;
	D3DXVECTOR3* mReflectMtrls;
	ID3D10ShaderResourceView** mDiffuseTextures;
	ID3D10ShaderResourceView** mSpecTextures;
	ID3D10ShaderResourceView** mNormalTextures;

protected:
	D3DXMATRIX shadowLightWVP;
	
	D3DXMATRIX mWVP;
	D3DXMATRIX mTexMtx;
	D3DXMATRIX mCreateWorld;

	D3DXMATRIX mPositionMtx;
	D3DXMATRIX mScaleMtx;
	D3DXMATRIX mRotationMtx;

protected:
	bool visible;
	// ģ����ر���
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mScale;
	D3DXVECTOR3 mRotation;

	// ������ر���
	float positionXTex, positionYTex;	// �������ĵ����꣨�ֲ�����ϵ��
	float scaleXTex, scaleYTex;	// �������Ų���
	float rotationTex;	// ������ת����

	// �����
public:
	BOOL Picked();
	ID3DX10Mesh* GetExternMesh(){	return mExternMeshData;	}
	UINT GetNumExternVertices(){	return  mNumExternVertices;	}
	UINT GetNumExternTriangles(){	return mNumExternTriangles;	}
	void DrawExtern();	// �������
	void OpenBoxExtern(D3DXCOLOR& color);
	D3DXVECTOR3 GetMin(){	return mMin;	}
	D3DXVECTOR3 GetMax(){	return mMax;	}
	void OpenPick(){	mEnablePick = true;	}
protected:
	ID3DX10Mesh* mExternMeshData;
	UINT mNumExternVertices;
	UINT mNumExternTriangles;

	D3DXVECTOR3 mMin, mMax;
	bool enableExtern;
	bool mEnablePick;

	ID3D10EffectPass* pass;
	ID3D10EffectPass* shadowPass;
	ID3D10EffectTechnique* mExternTech;
	ID3D10EffectMatrixVariable* mfxExternWVPVar;

	// ����ϵͳ���
public:
	// �������ٶȲ���
	bool GetSupported(){	return mSupported;	}
	void SetSupported(bool b);
	D3DXVECTOR3 GetSpeed(){	return mSpeed;	}

	// �������޸��ٶȺ�������Ч
	void SetSpeed(D3DXVECTOR3& v){	mSpeed = v;	}
	void SetSpeed(float x, float y, float z){	mSpeed.x = x, mSpeed.y = y, mSpeed.z = z;	}
	void SetAccSpeed(float x, float y, float z){	mAccSpeed.x = x, mAccSpeed.y = y, mAccSpeed.z = z;	}

	// ��ײ��ⲿ��
	float GetMass(){	return mMass;	}
	void SetMass(float m){	mMass = m;	}
		
	int GetShapeType(){	return mShapeType;	}
	
	float GetCrashMtr(){	return mCrashMtr;	}
	// ��ײ������������ 0.0f ~ 1.0f ֮��
	void SetCrashMtr(float mtr){	mCrashMtr = mtr>1.0f ? 1.0f : mtr;	}
	bool Crash(GameObj* mesh);	// ��ײ���
	virtual bool Crashed(GameObj* mesh);
	void OpenCrash(){	enableCrash = true;	}	// ����ײ����
	void CloseCrash(){	enableCrash = false;	}

	float GetHalfX(){	return halfX;	}
	float GetHalfY(){	return halfY;	}
	float GetHalfZ(){	return halfZ;	}

	void AddCrash( GameObj* cl,	// ������ײ��
		bool active );	// �Ƿ����������ײ

	// Ħ����ϵͳ
	void SetFrictionAccSpeed(float acc){	frictionAccSpeed = acc;	}
	float GetFrictionAccSpeed(){	return frictionAccSpeed;	}

protected:
	D3DXVECTOR3 mSpeed, mAccSpeed;

	float rotateSpeedX, rotateSpeedY, rotateSpeedZ,
		// ������ת�ٶ�
		increAccSpeedX, increAccSpeedY, increAccSpeedZ,
		// ���ٶ�����
		// ��������ʱ�����޸Ĵ��������������ܼ��ٶ��Ǵ���������������ٶ�֮������
		frictionAccSpeed;
		// Ħ�������ٶ�

	float halfX;	// ��x���Ϊ�˷������������Ӧ��˱���
	float halfY;	// ��y���
	float halfZ;	// ��Z���

	int mShapeType;		//��״
	float mCrashMtr;	// ��ײ���ʣ�0.0f��ʾ��ȫ�ǵ�����ײ��ײ��������������ճ�͡���һ��1.0f��ʾ������ײ����������й
	float mMass;		// ���������������ѧ�������������Ϊ������ʾ���޴�����
	// 
	bool mSupported;	// �˱���˵�������Ƿ�������Ӱ��
	bool mEnableJump;	// ��Ծ���Ʒ�
	float jumpSpeed;
	bool enableCrash;	// ��ײ��⿪��
};

#endif