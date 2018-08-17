#include "Mesh.h"


using namespace std;


Mesh::Mesh()
: mMeshData(0), mNumSubsets(0), mReflectMtrls(0), mDiffuseTextures(0), mSpecTextures(0), mNormalTextures(0), mNumVertices(0), mNumTriangles(0),
mShapeType(0),	// ��ײ��������Ĭ�������ε�
mCrashMtr(1.0f),	// ��ײ����Ĭ���ǲ�ɢʧ����
frictionAccSpeed(0.0f),	// Ħ�������ٶȣ�Ĭ����Ħ����
mMass(1.0f),		// Ĭ�������� 1.0f������������Ϊ����������̶�������������Ĭ��ֵ���Ա������Ϊ0
rotateSpeedX(0), rotateSpeedY(0), rotateSpeedZ(0),
mSupported(1),	// Ĭ���ǹ̶�������
mEnableJump(0), jumpSpeed(CAMERA_UP_SPEED)
{
	// �Ƿ���������
	enableExtern = false;
	// �Ƿ���Ӧʰȡ
	mEnablePick = false;
	// ��ײ���
	enableCrash = false;

	// Ϊ�˷���ɾ���ڵ㣬���Ӹñ���������ID
	mList = listID = NULL;

	visible = true;
	D3DXMatrixIdentity(&mCreateWorld);
	D3DXMatrixIdentity(&mTexMtx);
	D3DXMatrixIdentity(&mWVP);

	mPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	mRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMatrixIdentity(&mPositionMtx);
	D3DXMatrixIdentity(&mScaleMtx);
	D3DXMatrixIdentity(&mRotationMtx);

	positionXTex = 0.0f, positionYTex = 0.0f;	// �������ĵ����꣨�ֲ�����ϵ��
	scaleXTex = 1.0f, scaleYTex = 1.0f;	// �������Ų���
	rotationTex = 0.0f;	// ������ת����

	mSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	mAccSpeed = D3DXVECTOR3(0.0f, GRAVITY_ACC_SPEED, 0.0f);
}

Mesh::~Mesh()
{
	if(mList && listID)
		((DemoList*)mList)->Delete( (pDemoPoint)listID );
	mList = listID = NULL;

	if(mCrashList && mCrashListID)
		((CrashList*)mCrashList)->Delete( (pDemoPoint)mCrashListID );
	
	
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

void Mesh::OpenBoxExtern(D3DXCOLOR& color)
{
	
	enableExtern = true;

	mNumExternVertices = 24;
	mNumExternTriangles = 12;

	mExternTech = ColorVertexFX->GetTechniqueByName("ColorVertexTech");
	mfxExternWVPVar            = ColorVertexFX->GetVariableByName("gWVP")->AsMatrix();

	// ���벼��
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};
	// ����Mesh
	HR(D3DX10CreateMesh(md3dDevice, vertexDesc, 2,
			vertexDesc[0].SemanticName, mNumExternVertices, 
			mNumExternTriangles, D3DX10_MESH_32_BIT, &mExternMeshData));

	// Create vertex buffer
    Vertex_Color v[24];

	// Fill in the front face vertex data.
 	v[0] = Vertex_Color(
		mMin.x, mMin.y, mMin.z,
		color);
	v[1] = Vertex_Color(
		mMin.x, mMax.y, mMin.z,
		color);
	v[2] = Vertex_Color(
		mMax.x, mMax.y, mMin.z,
		color);
	v[3] = Vertex_Color(
		mMax.x, mMin.y, mMin.z,
		color);

	// Fill in the back face vertex data.
	v[4] = Vertex_Color(
		mMin.x, mMin.y, mMax.z,
		color);
	v[5] = Vertex_Color(
		mMax.x, mMin.y, mMax.z,
		color);
	v[6] = Vertex_Color(
		mMax.x, mMax.y, mMax.z,
		color);
	v[7] = Vertex_Color(
		mMin.x, mMax.y, mMax.z,
		color);

	// Fill in the top face vertex data.
	v[8]  = Vertex_Color(
		mMin.x, mMax.y, mMin.z,
		color);
	v[9]  = Vertex_Color(
		mMin.x, mMax.y, mMax.z,
		color);
	v[10] = Vertex_Color(
		mMax.x, mMax.y, mMax.z,
		color);
	v[11] = Vertex_Color(
		mMax.x, mMax.y, mMin.z,
		color);

	// Fill in the bottom face vertex data.
	v[12] = Vertex_Color(
		mMin.x, mMin.y, mMin.z,
		color);
	v[13] = Vertex_Color(
		mMax.x, mMin.y, mMin.z,
		color);
	v[14] = Vertex_Color(
		mMax.x, mMin.y, mMax.z,
		color);
	v[15] = Vertex_Color(
		mMin.x, mMin.y, mMax.z,
		color);

	// Fill in the left face vertex data.
	v[16] = Vertex_Color(
		mMin.x, mMin.y, mMax.z,
		color);
	v[17] = Vertex_Color(
		mMin.x, mMax.y, mMax.z,
		color);
	v[18] = Vertex_Color(
		mMin.x, mMax.y, mMin.z,
		color);
	v[19] = Vertex_Color(
		mMin.x, mMin.y, mMin.z,
		color);

	// Fill in the right face vertex data.
	v[20] = Vertex_Color(
		mMax.x, mMin.y, mMin.z,
		color);
	v[21] = Vertex_Color(
		mMax.x, mMax.y, mMin.z,
		color);
	v[22] = Vertex_Color(
		mMax.x, mMax.y, mMax.z,
		color);
	v[23] = Vertex_Color(
		mMax.x, mMin.y, mMax.z,
		color);


	HR(mExternMeshData->SetVertexData(0, v));

	// Create the index buffer

	UINT atrs[12];
	for(UINT index = 0; index<12; index++)
	{
		atrs[index] = 0;
	}

	DWORD indices[36];
	
	// Fill in the front face index data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Fill in the back face index data
	indices[6] = 4; indices[7]  = 5; indices[8]  = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// Fill in the top face index data
	indices[12] = 8; indices[13] =  9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// Fill in the bottom face index data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// Fill in the left face index data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// Fill in the right face index data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	HR(mExternMeshData->SetIndexData(indices, 36));
	HR(mExternMeshData->SetAttributeData(atrs));

	HR(mExternMeshData->GenerateAdjacencyAndPointReps(0.001f));
	HR(mExternMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0));
	HR(mExternMeshData->CommitToDevice());
}

void Mesh::DrawExtern()	// �������
{
	mfxExternWVPVar->SetMatrix((float*)&mWVP);

	ID3D10EffectPass* pass = mExternTech->GetPassByIndex(0);

	md3dDevice->IASetInputLayout(PosColor);
	//for(UINT subsetID = 0; subsetID < 6; ++subsetID)
	{
		pass->Apply(0);
		mExternMeshData->DrawSubset(0);
	}
	
}

void Mesh::GetAABB(D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
{
	ID3DX10MeshBuffer* vb = 0;
	HR(mMeshData->GetVertexBuffer(0, &vb));
	
	MeshVertex* vertices = 0;
	SIZE_T size;
	HR(vb->Map((void**)&vertices, &size));

	vMin = D3DXVECTOR3(+INFINITY, +INFINITY, +INFINITY);
	vMax = D3DXVECTOR3(-INFINITY, -INFINITY, -INFINITY);

	for(UINT i = 0; i < mMeshData->GetVertexCount(); ++i)
	{
		D3DXVec3Minimize(&vMin, &vMin, &vertices[i].pos);
		D3DXVec3Maximize(&vMax, &vMax, &vertices[i].pos);
	}
	
	HR(vb->Unmap());

	ReleaseCOM(vb);

	halfX = (vMax.x - vMin.x) / 2.0f;
	halfY = (vMax.y - vMin.y) / 2.0f;
	halfZ = (vMax.z - vMin.z) / 2.0f;

}

void Mesh::SetSupported(bool b)
{
	mSupported = b;
}

bool Mesh::Crash(GameObj* obj)	// ��ײ���
{
	// �ú��������ж�����������û����ײ��������������ٶ�
	Mesh* mesh = (Mesh*)obj;
	if(mesh == this)
		// �ų����Լ�
		return false;
	
	// �ų��������Զ�ĵ�
	static D3DXVECTOR3 position;
	position = mesh->GetPosition();

	// ��Ҫ�����ֵ�����Ӵ�����Ӧ��
	static float dty, dtx, dtz;
	dtx = mPosition.x - position.x;
	dty = mPosition.y - position.y;
	dtz = mPosition.z - position.z;


	if( dtx > MAX_DISTANT_OF_TWO_OBJ || dtx < -MAX_DISTANT_OF_TWO_OBJ)
		return false;
	if( dty > MAX_DISTANT_OF_TWO_OBJ || dty < -MAX_DISTANT_OF_TWO_OBJ)
		return false;
	if( dtz > MAX_DISTANT_OF_TWO_OBJ || dtz < -MAX_DISTANT_OF_TWO_OBJ)
		return false;

	static int shapeType;
	shapeType = mesh->GetShapeType();

	static D3DXVECTOR3 pos;
	static D3DXVECTOR3 norm;	// ���ߣ����������ײ����ٶȷ���
	static D3DXVECTOR3 verh;	// ��ֱ���ߵĵ�λ����

	static D3DXMATRIX normMtx;	// ���߱任����
	normMtx = mesh->GetWorldMtx();

	static D3DXMATRIX inverseW;
	D3DXMatrixInverse(&inverseW, 0, &normMtx);

	norm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	static float hx, hy, hz, dx, dy, dz, disX;
	static float dt1, dt2, dt3, dist;
	static float radius2;
	hy = mesh->GetHalfY();
	hx = mesh->GetHalfX(), hz = mesh->GetHalfZ();
	dy = hy + halfY;
	dx = halfX + hx, dz = halfZ + hz;

	// ����Ҫ���⼸�������
	// 1.����������
	// 2.������Բ׶����
	// 3.�����볤����
	// 4.�������볤����
	// 5.��������Բ׶����
	// 6.Բ׶������Բ׶����
	// ����ֻ�������������Σ����Σ�������ײ�����볤����
	// �������͵���ײ�����Ҫ�����ϵͳ���淨�������
	// �ȼ�����ײ�淨�ߣ�Ȼ����ݷ��߼�����ײ�ٶȣ������ֲ����˵�ִ����ˮ������Խ���

	// ���㷨��*************************************************
	if(shapeType == BOX )
	{
		// �Է��ǳ�����
		// 
		// X��Y��Z����� ����ϸ���� �ж϶���Ҫת����������ľֲ�����ϵ��
		// 
		// ת����ʼ***************************

		D3DXVec3TransformCoord(&pos, &mPosition, &inverseW);

		// ת�����****************************
		// 
		//
		// �����ֵ��������
		dtx = fabs(pos.x);
		dty = fabs(pos.y);
		dtz = fabs(pos.z);

		// ���������ײ
		if( dtx > dx+CRASH_GAP )
			return false;
		if( dty > dy+CRASH_GAP )
			return false;
		if( dtz > dz+CRASH_GAP )
			return false;


		// Y���Ӧ������ƽ��
		if( dtx < hx && dtz < hz )
		{
			// ��XZ������ײ����
			if( pos.y > 0.0f )	// �ϱ���
				norm = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			else	// �±���
				norm = D3DXVECTOR3(0.0f, -1.0f, 0.0f);;
		}
		else if( dty < hy && dtz < hz )
		{
			// ��YZ������ײ����
			if( pos.x > 0.0f )	// �ұ���
				norm = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			else	// �����
				norm = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		}
		else if( dty < hy && dtx < hx )
		{
			// ��XY������ײ����
			if( pos.z > 0.0f )	// �����
				norm = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			else	// ǰ����
				norm = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		
		// ����Ƿ���ײ���ߣ���������ƽ�漴�ɣ�ѡXZƽ��
		// ʾ��ͼ��
		//         +z
		//     ___________
		//    |           |
		//    |           _
		//-x  |      �� ( o )  +x
		//    |           -
		//    |           |
		//    -------------
		//         -z

		else if( (dtx > hx) && (dtx < hx + halfX) )
		{
			// +x
			// ��Ҫ�ж��Ƿ���ײ
			

			dt1 = dtx - hx, dt2 = dty - hy; 
			disX = dt1*dt1 + dt2*dt2;
			
			radius2 = halfY*halfY;	// ����İ뾶 = halfX = halfY = halfZ
			if(disX > radius2+CRASH_GAP)
				return false;
			else
			{
				// ��������
				if(pos.x > 0.0f)
				{
					//������+x
					if(pos.y > 0.0f)
						// +y������
						norm = D3DXVECTOR3(0.7071f, 0.7071f, 0.0f);
					else
						// -y����
						norm = D3DXVECTOR3(0.7071f, -0.7071f, 0.0f);
				}
				else
				{
					//������-x
					if(pos.y > 0.0f)
						// +y����
						norm = D3DXVECTOR3(-0.7071f, 0.7071f, 0.0f);
					else
						// -y����
						norm = D3DXVECTOR3(-0.7071f, -0.7071f, 0.0f);
				}
			}
		}
		else if( (dtz > hz) && (dtz < hz + halfZ) )
		{
			// +z
			// ��Ҫ�ж��Ƿ���ײ
			
			dt1 = dtz - hz, dt2 = dty - hy; 
			disX = dt1*dt1 + dt2*dt2;

			radius2 = halfY*halfY;	// ����İ뾶 = halfX = halfY = halfZ
			if(disX > radius2 + CRASH_GAP)
				return false;
			else
			{
				if(pos.z > 0.0f)
				{
					//������+z
					if(pos.y > 0.0f)
						// +y������
						norm = D3DXVECTOR3(0.0f, 0.7071f, 0.7071f);
					else
						// -y����
						norm = D3DXVECTOR3(0.0f, -0.7071f, 0.7071f);
				}
				else
				{
					//������-z
					if(pos.y > 0.0f)
						// +y������
						norm = D3DXVECTOR3(0.0f, 0.7071f, -0.7071f);
					else
						// -y����
						norm = D3DXVECTOR3(0.0f, -0.7071f, -0.7071f);
				}
			}
		}
		// ����Ƿ�������
		// 
		else
		{
			dt1 = dtz - hz, dt2 = dty - hy, dt3 = dtx - hx;
			disX = dt1*dt1 + dt2*dt2 + dt3*dt3;
			radius2 = halfY*halfY;	// ����İ뾶 radius = halfX = halfY = halfZ

			if(disX >  radius2)
				return false;
			else
			{
				// ��������
				if(pos.y > 0.0f )
				{
					if(pos.x>0.0f)
					{
						if(pos.z > 0.0f)
							// ����( 1, 1, 1)��
							norm = D3DXVECTOR3(0.57735f, 0.57735f, 0.57735f);
						else
							// ����(1, 1, -1)
							norm = D3DXVECTOR3(0.57735f, 0.57735f, -0.57735f);
					}
					else
					{
						if(pos.z>0.0f)
							// ����( -1, 1, 1)��
							norm = D3DXVECTOR3(-0.57735f, 0.57735f, 0.57735f);
						else
							// ����(-1, 1, -1)
							norm = D3DXVECTOR3(-0.57735f, 0.57735f, -0.57735f);
					}
				}
				else
				{
					if(pos.x>0.0f)
					{
						if(pos.z>0.0f)
							// ����( 1, -1, 1)��
							norm = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
						else
							// ����(1, -1, -1)
							norm = D3DXVECTOR3(0.57735f, -0.57735f, -0.57735f);
					}
					else
					{
						if(pos.z>0.0f)
							// ����( -1, 1, 1)��
							norm = D3DXVECTOR3(-0.57735f, 0.57735f, 0.57735f);
						else
							// ����(-1, 1, -1)
							norm = D3DXVECTOR3(-0.57735f, 0.57735f, -0.57735f);
					}
				}
			}
		}

		// ��normת������������ϵ��
		D3DXVec3TransformNormal(&norm, &norm, &normMtx);
		
		// ��λ�����������򻯼���
		D3DXVec3Normalize( &norm, &norm);

	}	
	else if(shapeType == CYLINDER)
	{
		// �Է���Բ׶���壬������
		return false;
	}
	else if(shapeType == QUAD)
	{
		// �Է���ƽ�棬ƽ��Ҳ��������ת�Ƕȣ�Ҳ��Ҫת�����ֲ�����ϵ���ж�������ײ
		D3DXVec3TransformCoord(&pos, &mPosition, &inverseW);
		// ת�����****************************

		// 
		//
		// �����ֵ��������

		// �趨ֻ�����������ײ������ǿ��ת�Ƶ�����
		// �ж�������ײ
		if( pos.x > dx+CRASH_GAP || pos.x < -dx-CRASH_GAP)
			return false;
		if( pos.y > dy+CRASH_GAP || pos.y < 0.0f )
			return false;
		if( pos.z > dz+CRASH_GAP || pos.z < -dz-CRASH_GAP )
			return false;

		// ����norm
		norm = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXVec3TransformNormal(&norm, &norm, &normMtx);
		// ��λ�����������򻯼���
		D3DXVec3Normalize( &norm, &norm);
		
	}
	else	// �����ڳ������Բ׶�������״ȫ��תΪĬ�ϡ�������
	{
		// �Է�����

		// ����
		norm = mPosition - position;
		
		// �ж�������ײ
		dist = D3DXVec3Length( &norm);

		if(dist > halfX + hx + CRASH_GAP)
		{
			// û����������
			return false;
		}
		// ��λ�����������򻯼���
		D3DXVec3Normalize( &norm, &norm);

	}
	// ɸѡ����*************************************************
	static float mass, crashMtr;
	mass = mesh->GetMass();
	crashMtr = mesh->GetCrashMtr();

	// ����
	static D3DXVECTOR3 oriSpeed1,	// ԭ�ٶ�
		nSpeed1,	// norm�������ϵ��ٶ�
		vSpeed1;	// ��ֱ��norm������ٶȷ���
	oriSpeed1 = mSpeed;

	static float len;
	len = D3DXVec3Length( &oriSpeed1 );
	// �ٶ�̫С����0
	if( len <= MIN_SPEED )
	{
		len = 0.0f;
		mSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		return true;
	}
	

	// �ж������Ƿ���Ϸ�������
	static float r;
	r = D3DXVec3Dot( &oriSpeed1, &norm );
	if( r > 0.0f )
		return true;

	// �����ٶ�*************************************************
	// �Է�
	static D3DXVECTOR3 oriSpeed2,
		nSpeed2,
		vSpeed2;
	static D3DXVECTOR3 destSpeed1, rotateSpeed;

	oriSpeed2 = mesh->GetSpeed();

	static float mtr;
	mtr = mesh->GetCrashMtr();
	// 
	// �����ٶ�

	// ����
	// norm����
	static float valueNSpeed1;
	valueNSpeed1 = D3DXVec3Dot(&oriSpeed1, &norm);

	nSpeed1 = valueNSpeed1 * norm;
	// ��ֱnorm�ķ���
	vSpeed1 = oriSpeed1 - nSpeed1;
	
	static float valueVSpeed1;
	valueVSpeed1 = D3DXVec3Length( &vSpeed1 );

	static float fricMtr;	// Ħ��������
	fricMtr = mtr * mCrashMtr;
	
	if(fricMtr == 0.0f)
	{
		// ��������ײ���ܣ���ȫĦ��
		destSpeed1 = ComputeVVecLineToNorm( &oriSpeed1, &norm );
		mSpeed = destSpeed1;
		return true;
	}

	if(mass <= 0.001f )
	{
		// �Է������޴�����ʱ

		// ���㷴������
		// ������ײ
		destSpeed1 = ComputeReflectLineToLine( &oriSpeed1, &norm);
		mSpeed = destSpeed1 * fricMtr;
		
		rotateSpeed = destSpeed1 + oriSpeed1;

		// �ٶ�ϵ��
		

		rotateSpeedZ = -rotateSpeed.x;
		rotateSpeedX = -rotateSpeed.y;
		rotateSpeedY = -rotateSpeed.z;

		return true;
	}
	else
	{
		// ��������ϵ��
		// ע�⣬��������˳�����һ��Ҫ�������Ϊ0�����
		static float m1, m2, m3, m4, mt;
		mt = (mMass + mass);

		if(mt < 0.0001f && mt > -0.0001f)
			// ����Ϊ0��ֱ�ӷ���
			return false;
		m1 = (mMass - mass) / mt;
		m2 = mass * 2.0f / mt;
		m3 = -m1;
		m4 = mMass * 2.0f / mt;



		// �����ٶ���norm�����ϵķ���
		// ȡ������Ϊnorm����
		
		// �Է�
		// norm����
		static float valueNSpeed2;
		valueNSpeed2 = D3DXVec3Dot(&oriSpeed2, &norm);
		nSpeed2 = valueNSpeed2 * norm;
		// ��ֱnorm����
		vSpeed2 = oriSpeed2 - nSpeed2;
		static float valueVSpeed2;
		valueVSpeed2 = D3DXVec3Length( &vSpeed2 );
		D3DXVec3Normalize( &verh, &vSpeed2);

		// ��ײ����ٶ�ֵ
		static float valueNSpeed3;
		valueNSpeed3 = (m1*valueNSpeed1 + m2*valueNSpeed2);	// ����
		static float valueNSpeed4;
		valueNSpeed4 = (m3*valueNSpeed2 + m4*valueNSpeed1);	// �Է�

		static float valueVSpeed3;
		valueVSpeed3 = (m1*valueVSpeed1 + m2*valueVSpeed2);	// ����
		static float valueVSpeed4;
		valueVSpeed4 = (m3*valueVSpeed2 + m4*valueVSpeed1);	// �Է�

		destSpeed1 = valueNSpeed3*norm + valueVSpeed3*verh;

		rotateSpeed = destSpeed1 + oriSpeed1;
		rotateSpeedZ = -rotateSpeed.x;
		rotateSpeedX = -rotateSpeed.y;
		rotateSpeedY = -rotateSpeed.z;

		oriSpeed1 = (destSpeed1) *mtr * mCrashMtr;	// ����
		oriSpeed2 = (valueNSpeed4*norm + valueVSpeed4*verh) *mtr * mCrashMtr;	// �Է�
	}

	SetSpeed(destSpeed1);
	mesh->SetSpeed(oriSpeed2);
	return true;
}

bool Mesh::Crashed(GameObj* obj)
{
	// �ú��������ж�����������û����ײ��������ײ���ٶ�
	
	Mesh* mesh = (Mesh*)obj;
	if(mesh == this)
		// �ų����Լ�
		return false;

	// �ų��������Զ�ĵ�
	D3DXVECTOR3 position;
	position = mesh->GetPosition();

	// ��Ҫ�����ֵ�����Ӵ�����Ӧ��
	float dty, dtx, dtz;
	dtx = mPosition.x - position.x;
	dty = mPosition.y - position.y;
	dtz = mPosition.z - position.z;


	if( dtx > MAX_DISTANT_OF_TWO_OBJ || dtx < -MAX_DISTANT_OF_TWO_OBJ)
		return false;
	if( dty > MAX_DISTANT_OF_TWO_OBJ || dty < -MAX_DISTANT_OF_TWO_OBJ)
		return false;
	if( dtz > MAX_DISTANT_OF_TWO_OBJ || dtz < -MAX_DISTANT_OF_TWO_OBJ)
		return false;

	int shapeType;
	shapeType = mesh->GetShapeType();

	D3DXMATRIX inverseW;
	D3DXMatrixInverse(&inverseW, 0, &mesh->GetWorldMtx());

	float hx, hy, hz, dx, dy, dz, disX;
	hy = mesh->GetHalfY();
	hx = mesh->GetHalfX(), hz = mesh->GetHalfZ();
	dy = hy + halfY;
	dx = halfX + hx, dz = halfZ + hz;

	if(shapeType == BOX )
	{
		// �Է��ǳ�����
		// 
		// X��Y��Z����� ����ϸ���� �ж϶���Ҫת����������ľֲ�����ϵ��
		// 
		// ת����ʼ***************************

		D3DXVECTOR3 pos;

		D3DXVec3TransformCoord(&pos, &mPosition, &inverseW);

		// ת�����****************************
		// 
		//
		// �����ֵ��������
		dtx = fabs(pos.x);
		dty = fabs(pos.y);
		dtz = fabs(pos.z);

		// ���������ײ
		if( dtx > dx+CRASH_GAP )
			return false;
		if( dty > dy+CRASH_GAP )
			return false;
		if( dtz > dz+CRASH_GAP )
			return false;

		if( (dtx > hx) && (dtx < hx + halfX) )
		{
			// +x
			float dt1 = dtx - hx, dt2 = dty - hy;
			disX = dt1*dt1 + dt2*dt2;
			float radius2 = halfY*halfY;
			if(disX > radius2+CRASH_GAP)
				return false;
		}
		else if( (dtz > hz) && (dtz < hz + halfZ) )
		{
			// +z
			float dt1 = dtz - hz, dt2 = dty - hy;
			disX = dt1*dt1 + dt2*dt2;
			float radius2 = halfY*halfY;
			if(disX > radius2 + CRASH_GAP)
				return false;
		}
		// ����Ƿ�������
		// 
		else
		{
			float dt1 = dtz - hz, dt2 = dty - hy, dt3 = dtx - hx;
			disX = dt1*dt1 + dt2*dt2 + dt3*dt3;
			float radius2 = halfY*halfY;

			if(disX >  radius2)
				return false;
		}
	}	
	else if(shapeType == CYLINDER)
	{
		// �Է���Բ׶���壬������
		return false;
	}
	else if(shapeType == QUAD)
	{
		// �Է���ƽ�棬ƽ��Ҳ��������ת�Ƕȣ�Ҳ��Ҫת�����ֲ�����ϵ���ж�������ײ
		D3DXVECTOR3 pos;
		D3DXVec3TransformCoord(&pos, &mPosition, &inverseW);
		// ת�����****************************

		// �ж�������ײ
		if( pos.x > dx+CRASH_GAP || pos.x < -dx-CRASH_GAP )
			return false;
		if( pos.y > dy+CRASH_GAP || pos.y < -dy-CRASH_GAP )
			return false;
		if( pos.z > dz+CRASH_GAP || pos.z < -dz-CRASH_GAP )
			return false;
		
	}
	else	// �����ڳ������Բ׶�������״ȫ��תΪĬ�ϡ�������
	{
		// �Է�����

		// ����
		D3DXVECTOR3 norm = mPosition - position;
		
		// �ж�������ײ
		float dist;
		dist = D3DXVec3Length( &norm);

		if(dist > halfX + hx + CRASH_GAP)
		{
			// û����������
			return false;
		}
	}
	return true;
}

void Mesh::ActionUpdate(float dt)
{
	// ����ϵͳ***************************
	// ���㵱ǰ�ٶ�
	if(mSupported)
		// λ�ù̶�������
		;
	else
	{
		// ��ײ���Ĺ����б���������Ĵ��ڣ����򲻷�����Ȼ����

		// ���������ײ������
		if(gCrashList->Crash(this));
		else
		{
			if(mSpeed.y + mAccSpeed.y > MAX_SPEED)
				mSpeed.y = MAX_SPEED;
			if(mSpeed.y + mAccSpeed.y < -MAX_SPEED)
				mSpeed.y = -MAX_SPEED;
			else
				mSpeed.y += mAccSpeed.y;
			// ֻ�����������ײ������Ż���ת
			Rotate(rotateSpeedX, rotateSpeedY, rotateSpeedZ);
		}

		// ���㵱ǰλ��
		// ������
		if(mPosition.y + mSpeed.y + halfY >=  SKY_TOP)
			SetSpeed(0.0f, 0.0f, 0.0f);
		else if(mPosition.y + mSpeed.y - halfY <= HORIZON)
			SetSpeed(0.0f, 0.0f, 0.0f);
	}
	Translate( mSpeed );

	if(mEnablePick)
	{
		if(Picked())
			// ��ʰȡ����
			GetMouse().CatchObj();
	}
}

void Mesh::DrawContent()
{
	if(visible == false)
		return ;
	mWVP = mCreateWorld*mView*mProj_3D;

	mfxWVPVar->SetMatrix((float*)&mWVP);
	mfxWorldVar->SetMatrix((float*)& mCreateWorld);
	mfxTexMtxVar->SetMatrix((float*)&mTexMtx);

	for(UINT subsetID = 0; subsetID < mNumSubsets; ++subsetID)
	{
		mfxReflectMtrlVar->SetRawValue((void*)&mReflectMtrls[subsetID], 0, sizeof(D3DXVECTOR3));
		mfxDiffuseMapVar->SetResource(mDiffuseTextures[subsetID]);
		mfxSpecMapVar->SetResource(mSpecTextures[subsetID]);
		mfxNormalMapVar->SetResource(mNormalTextures[subsetID]);

		pass->Apply(0);
		mMeshData->DrawSubset(subsetID);
	}

	// ������壬������
	if(enableExtern)
		DrawExtern();
		
}

void Mesh::DrawContentShadow(ID3D10EffectMatrixVariable* mfxLightWVPVar, D3DXMATRIX lightVP)
{
	
	if(visible == false)
		return ;
	
	mWVP = mCreateWorld*mView*mProj_3D;

	mfxWVPVar->SetMatrix((float*)&mWVP);
	mfxWorldVar->SetMatrix((float*)& mCreateWorld);
	mfxTexMtxVar->SetMatrix((float*)&mTexMtx);

	mfxLightWVPVar->SetMatrix(mCreateWorld*lightVP);

	

	for(UINT subsetID = 0; subsetID < mNumSubsets; ++subsetID)
	{
		mfxReflectMtrlVar->SetRawValue((void*)&mReflectMtrls[subsetID], 0, sizeof(D3DXVECTOR3));
		mfxDiffuseMapVar->SetResource(mDiffuseTextures[subsetID]);
		mfxSpecMapVar->SetResource(mSpecTextures[subsetID]);
		mfxNormalMapVar->SetResource(mNormalTextures[subsetID]);

		shadowPass->Apply(0);
		mMeshData->DrawSubset(subsetID);
	}
	// ������壬������
	if(enableExtern)
		DrawExtern();
		
}

void Mesh::DrawToShadowMap(
		ID3D10EffectMatrixVariable* mfxBuildShadowMapLightWVPVar,
		D3DXMATRIX lightVP,
		ID3D10EffectShaderResourceVariable* diffuseMapVar,
		ID3D10EffectPass* pass)
{
	
	if(visible == false)
		return ;

	shadowLightWVP = mCreateWorld*lightVP;
	mfxBuildShadowMapLightWVPVar->SetMatrix((float*)&shadowLightWVP);

	// We only need diffuse map for drawing into shadow map.
	for(UINT subsetID = 0; subsetID < mNumSubsets; ++subsetID)
	{
		diffuseMapVar->SetResource(mDiffuseTextures[subsetID]);
		pass->Apply(0);
		mMeshData->DrawSubset(subsetID);
	}
	
}

void Mesh::getFXHandles()
{
	pass = mTech->GetPassByIndex(0);
	shadowPass = mShadowTech->GetPassByIndex(0);

	mfxWVPVar            = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar          = mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxReflectMtrlVar    = mFX->GetVariableByName("gReflectMtrl")->AsVector();
	mfxDiffuseMapVar     = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar        = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxNormalMapVar      = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();
}

void Mesh::LoadMap(
		UINT numSubsets,
		ID3D10ShaderResourceView** diffuseMapRVArray,
		ID3D10ShaderResourceView** specMapRVArray,
		ID3D10ShaderResourceView** normMapRVArray,
		D3DXVECTOR3* reflectMtrArray	// �������
		)
{
	mDiffuseTextures = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*numSubsets);
	mSpecTextures = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*numSubsets);
	mNormalTextures = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*numSubsets);
	mReflectMtrls = (D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3)*numSubsets);

	for(UINT i=0; i<numSubsets; i++)
	{
		if(diffuseMapRVArray[i] == NULL)
			mDiffuseTextures[i] = GetTextureMgr().GetTexRVByName("PIC/blackdiffuse.dds");
		else
			mDiffuseTextures[i] = diffuseMapRVArray[i];
		if(specMapRVArray[i] == NULL)
			mSpecTextures[i] = GetTextureMgr().GetTexRVByName("PIC/defaultspec.dds");
		else
			mSpecTextures[i] = specMapRVArray[i];
		if(normMapRVArray[i] == NULL)
			mNormalTextures[i] = GetTextureMgr().GetTexRVByName("PIC/defaultnormal.dds");
		else
			mNormalTextures[i] = normMapRVArray[i];
		if(reflectMtrArray[i] == NULL)
			mReflectMtrls[i] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		else
			mReflectMtrls[i] = reflectMtrArray[i];
	}
}

void Mesh::Init( float pX, float pY, float pZ,	// ����
		ID3D10Effect* fx,					// Ч��
		char* techName, char* shadowTechName,
		char* meshFileName			// ģ������
		)
{
	mPosition.x = pX, mPosition.y = pY, mPosition.z = pZ;
	D3DXMatrixTranslation( &mPositionMtx, pX, pY, pZ );

	mScale.x = 1.0f, mScale.y = 1.0f, mScale.z = 1.0f;
	D3DXMatrixScaling(&mScaleMtx, mScale.x, mScale.y, mScale.z);

	mRotation.x = 0.0f, mRotation.y = 0.0f, mRotation.z = 0.0f;
	D3DXMATRIX rM;
	D3DXMatrixRotationX( &mRotationMtx, mRotation.x );
	D3DXMatrixRotationY( &rM, mRotation.y );
	mRotationMtx *= rM;
	D3DXMatrixRotationZ( &rM, mRotation.z );
	mRotationMtx *= rM;

	mCreateWorld = mRotationMtx * mScaleMtx * mPositionMtx;	// position�������
	mWVP = mCreateWorld * mView * mProj_3D;

	mFX = fx;
	if(mFX == 0)
		return ;
	mTech = mFX->GetTechniqueByName(techName);
	mShadowTech = mFX->GetTechniqueByName(shadowTechName);
	getFXHandles();

	if(meshFileName)
	{
		// ֱ�Ӽ���ģ��
		LoadMesh(meshFileName);

		GetAABB(mMin, mMax);
	}
	return ;
}

void Mesh::LoadMesh(char* fileName)
{
	FILE* hist = fopen(fileName, "r");
	
	char c[1024] = {};
	char ct[1024] = {};
	if( hist )
	{
		// file header text
		if(fgets(c, 1024, hist) == NULL)
			return ;
		// #Subsets
		if(fgets(c, 1024, hist) == NULL)
			return ;
		else
		{
			int i=0, j=0;
			while(c[i] != ' ')
				i++;
			i++;
			while(c[i] != '\n')
			{
				ct[j] = c[i];
				i++;
				j++;
			}
			ct[j] = '\0';
			mNumSubsets = atoi(ct);
		}
		// #Vertices
		if(fgets(c, 1024, hist) == NULL)
			return ;
		else
		{
			int i=0, j=0;
			while(c[i] != ' ')
				i++;
			i++;
			while(c[i] != '\n')
			{
				ct[j] = c[i];
				i++;
				j++;
			}
			ct[j] = '\0';
			mNumVertices = atoi(ct);
		}
		// #Triangles
		if(fgets(c, 1024, hist) == NULL)
			return ;
		else
		{
			int i=0, j=0;
			while(c[i] != ' ')
				i++;
			i++;
			while(c[i] != '\n')
			{
				ct[j] = c[i];
				i++;
				j++;
			}
			ct[j] = '\0';
			mNumTriangles = atoi(ct);
		}
		// ���벼��
		D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
		};
		// ����Mesh
		HR(D3DX10CreateMesh(md3dDevice, vertexDesc, 4, 
			vertexDesc[0].SemanticName, mNumVertices, 
			mNumTriangles, D3DX10_MESH_32_BIT, &mMeshData));

		// ����
		if(fgets(c, 1024, hist) == NULL)
			return ;

		// ��ͷ
		if(fgets(c, 1024, hist) == NULL)
			return ;

		// ����
		mDiffuseTextures = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*mNumSubsets);
		mSpecTextures = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*mNumSubsets);
		mNormalTextures = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*mNumSubsets);
		mReflectMtrls = (D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3)*mNumSubsets);

		for(UINT index = 0; index < mNumSubsets; ++index)
		{
			// ����
			char diffuseMapFilename[1024];
			char specMapFilename[1024];
			char normalMapFilename[1024];

			// diffuseMap
			if(fgets(c, 1024, hist) == NULL)
				return ;
			else
			{
				int j=0;
				while(c[j] != '\n')
				{
					j++;
				}
				c[j] = '\0';
				strcpy(diffuseMapFilename, c);
			}
			// specMap
			if(fgets(c, 1024, hist) == NULL)
				return ;
			else
			{
				int j=0;
				while(c[j] != '\n')
				{
					j++;
				}
				c[j] = '\0';
				strcpy(specMapFilename, c);
			}
			// normMap
			if(fgets(c, 1024, hist) == NULL)
				return ;
			else
			{
				int j=0;
				while(c[j] != '\n')
				{
					j++;
				}
				c[j] = '\0';
				strcpy(normalMapFilename, c);
			}
			// ��������
			
			mDiffuseTextures[index] = GetTextureMgr().CreateTex(diffuseMapFilename);
			mSpecTextures[index] = GetTextureMgr().CreateTex(specMapFilename);
			mNormalTextures[index] = GetTextureMgr().CreateTex(normalMapFilename);
			

			// �������
			D3DXVECTOR3 reflectivity;
			if(fgets(c, 1024, hist) == NULL)
				return ;
			else
			{
				int i=0, j=0;
				// ע��
				while(c[i] != ' ')
					i++;
				i++;
				// ��һ����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				reflectivity.x = (float)atof(ct);
				// �ڶ�����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				reflectivity.y = (float)atof(ct);
				// ��������ֵ
				while(c[i] != '\n')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				reflectivity.z = (float)atof(ct);
			}
			// ����
			if(fgets(c, 1024, hist) == NULL)
				return ;
			
			mReflectMtrls[index] = reflectivity;
			
		}
		// vertices header text
		if(fgets(c, 1024, hist) == NULL)
			return ;
		// ���㻺��
		MeshVertex* verts = new MeshVertex[mNumVertices];

		for(UINT index = 0; index < mNumVertices; ++index)
		{
			// Position:
			if(fgets(c, 1024, hist) == NULL)
				return ;
			else
			{
				int i=0, j=0;
				// ע��
				while(c[i] != ' ')
					i++;
				i++;
				// ��һ����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				verts[index].pos.x = (float)atof(ct);
				// �ڶ�����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				verts[index].pos.y = (float)atof(ct);
				// ��������ֵ
				while(c[i] != '\n')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				verts[index].pos.z = (float)atof(ct);
			}
			// Tangent:
			if(fgets(c, 1024, hist) == NULL)
				return ;
			else
			{
				int i=0, j=0;
				// ע��
				while(c[i] != ' ')
					i++;
				i++;
				// ��һ����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				verts[index].tangent.x = (float)atof(ct);
				// �ڶ�����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				verts[index].tangent.y = (float)atof(ct);
				// ��������ֵ
				while(c[i] != '\n')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				verts[index].tangent.z = (float)atof(ct);
			}
			// Normal:
			if(fgets(c, 1024, hist) == NULL)
				return ;
			else
			{
				int i=0, j=0;
				// ע��
				while(c[i] != ' ')
					i++;
				i++;
				// ��һ����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				verts[index].normal.x = (float)atof(ct);
				// �ڶ�����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				verts[index].normal.y = (float)atof(ct);
				// ��������ֵ
				while(c[i] != '\n')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				verts[index].normal.z = (float)atof(ct);
			}
			// Tex-Coords:
			if(fgets(c, 1024, hist) == NULL)
				return ;
			else
			{
				int i=0, j=0;
				// ע��
				while(c[i] != ' ')
					i++;
				i++;
				// ��һ����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				verts[index].texC.x = (float)atof(ct);
				// �ڶ�����ֵ
				while(c[i] != '\n')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				verts[index].texC.y = (float)atof(ct);
			}
			// ����
			if(fgets(c, 1024, hist) == NULL)
				return ;
		}
		HR(mMeshData->SetVertexData(0, verts));
		delete[] verts;

		// indices
		DWORD* indices = new DWORD[mNumTriangles*3];
		UINT* attributeBuffer = new UINT[mNumTriangles];
		// triangles header text
		if(fgets(c, 1024, hist) == NULL)
				return ;
		for(UINT index = 0; index < mNumTriangles; ++index)
		{
			if( fgets(c, 1024, hist) == NULL )
				return ;
			else
			{
				// û��ע�Ͳ��֣�ֱ�������ݣ���Ҫע��
				int i=0, j=0;
				// ��һ����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				indices[index*3+0] = atoi(ct);
				// �ڶ�����ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				indices[index*3+1] = atoi(ct);
				// ��������ֵ
				while(c[i] != ' ')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				i++;
				j=0;
				indices[index*3+2] = atoi(ct);
				// ���ĸ���ֵ
				while(c[i] != '\n')
				{
					ct[j] = c[i];
					i++;
					j++;
				}
				ct[j] = '\0';
				attributeBuffer[index] = atoi(ct);
			}
		}
		HR(mMeshData->SetIndexData(indices, mNumTriangles*3));
		HR(mMeshData->SetAttributeData(attributeBuffer));

		delete[] indices;
		delete[] attributeBuffer;

		HR(mMeshData->GenerateAdjacencyAndPointReps(0.001f));
		HR(mMeshData->Optimize(D3DX10_MESHOPT_ATTR_SORT|D3DX10_MESHOPT_VERTEX_CACHE,0,0));
		HR(mMeshData->CommitToDevice());
	}
	fclose(hist);
	
}

void Mesh::SetPosition( D3DXVECTOR3& position )
{
	mPosition.x = position.x, mPosition.y = position.y, mPosition.z = position.z;
	D3DXMatrixTranslation( &mPositionMtx, mPosition.x, mPosition.y, mPosition.z );

	mCreateWorld = mScaleMtx * mRotationMtx * mPositionMtx;	// position�������
}

void Mesh::SetPosition( float pX, float pY, float pZ )
{
	mPosition.x = pX, mPosition.y = pY, mPosition.z = pZ;

	D3DXMatrixTranslation( &mPositionMtx, pX, pY, pZ );

	mCreateWorld = mScaleMtx * mRotationMtx * mPositionMtx;	// position�������
}
void Mesh::SetScale( float sX, float sY, float sZ )
{
	mScale.x = sX, mScale.y = sY, mScale.z = sZ;

	D3DXMatrixScaling(&mScaleMtx, sX, sY, sZ);

	halfX = (mMax.x - mMin.x) / 2.0f;
	halfY = (mMax.y - mMin.y) / 2.0f;
	halfZ = (mMax.z - mMin.z) / 2.0f;

	halfX *= sX, halfY *= sY, halfZ *= sZ;

	mCreateWorld = mScaleMtx * mRotationMtx * mPositionMtx;	// position�������
}
void Mesh::SetRotation( float rX, float rY, float rZ )
{
	mRotation.x = rX, mRotation.y = rY, mRotation.z = rZ;

	static D3DXMATRIX rM;
	D3DXMatrixRotationX( &mRotationMtx, rX );
	D3DXMatrixRotationY( &rM, rY );
	mRotationMtx *= rM;
	D3DXMatrixRotationZ( &rM, rZ );
	mRotationMtx *= rM;

	mCreateWorld = mScaleMtx * mRotationMtx * mPositionMtx;	// position�������
}

void Mesh::Translate( D3DXVECTOR3& d )
{
	mPosition += d;

	static D3DXMATRIX transMtx;
	D3DXMatrixTranslation( &transMtx, d.x, d.y, d.z );
	mPositionMtx *= transMtx;
	mCreateWorld = mCreateWorld * transMtx;	// position�������
}
void Mesh::Translate( float x, float y, float z )
{
	mPosition.x += x, mPosition.y += y, mPosition.z += z;

	static D3DXMATRIX transMtx;
	D3DXMatrixTranslation( &transMtx, x, y, z );
	mPositionMtx *= transMtx;
	mCreateWorld = mCreateWorld * transMtx;	// position�������
}
void Mesh::Scale( float sX, float sY, float sZ )
{
	halfX*=sX, halfY *= sY, halfZ *= sZ;

	mScale.x *= sX, mScale.y *= sY, mScale.z *= sZ;
	static D3DXMATRIX scaleMtx;
	D3DXMatrixScaling(&scaleMtx, sX, sY, sZ);
	mScaleMtx *= scaleMtx;
	mCreateWorld = scaleMtx * mCreateWorld;	// position�������
}

void Mesh::RotateX( float rX )
{
	// Rotation
	mRotation.x += rX;

	static D3DXMATRIX rM;
	D3DXMatrixRotationX( &rM, rX );
	mRotationMtx *= rM;

	mCreateWorld = mScaleMtx * mRotationMtx * mPositionMtx;	// position�������
}
void Mesh::RotateY( float rY )
{
	// Rotation
	mRotation.y += rY;
	static D3DXMATRIX rM;
	D3DXMatrixRotationY( &rM, rY );
	mRotationMtx *= rM;

	mCreateWorld = mScaleMtx * mRotationMtx * mPositionMtx;	// position�������
}
void Mesh::RotateZ( float rZ )
{
	// Rotation
	mRotation.z += rZ;

	static D3DXMATRIX rM;
	D3DXMatrixRotationZ( &rM, rZ );
	mRotationMtx *= rM;

	mCreateWorld = mScaleMtx * mRotationMtx * mPositionMtx;	// position�������
}
void Mesh::Rotate(float rX, float rY, float rZ)
{
	// Rotation
	mRotation.x += rX, mRotation.y += rY, mRotation.z += rZ;

	static D3DXMATRIX rM;
	D3DXMatrixRotationX( &mRotationMtx, mRotation.x );

	D3DXMatrixRotationY( &rM, mRotation.y );
	mRotationMtx *= rM;
	D3DXMatrixRotationZ( &rM, mRotation.z );
	mRotationMtx *= rM;

	mCreateWorld = mScaleMtx * mRotationMtx * mPositionMtx;	// position�������
}

void Mesh::SetPositionTex(float x, float y)
{
	positionXTex = x, positionYTex = y;

	static D3DXMATRIX mPositionTex;
	D3DXMatrixTranslation( &mPositionTex, x, y, 0.0f );

	static D3DXMATRIX mScaleTex;
	D3DXMatrixScaling(&mScaleTex, scaleXTex, scaleYTex, 0.0f);

	static D3DXMATRIX mRotationTex;
	D3DXMatrixRotationZ( &mRotationTex, rotationTex );

	mTexMtx = mScaleTex * mRotationTex * mPositionTex;	// position�������
}
void Mesh::SetScaleTex( float sX, float sY )
{
	scaleXTex = sX, scaleYTex = sY;

	static D3DXMATRIX mPositionTex;
	D3DXMatrixTranslation( &mPositionTex, positionXTex, positionYTex, 0.0f );

	static D3DXMATRIX mScaleTex;
	D3DXMatrixScaling( &mScaleTex, sX, sY, 0.0f );

	static D3DXMATRIX mRotationTex;
	D3DXMatrixRotationZ( &mRotationTex, rotationTex );

	mTexMtx = mScaleTex * mRotationTex * mPositionTex;	// position�������
}
void Mesh::SetRotationTex( float rotation )
{
	rotationTex = rotation;

	static D3DXMATRIX mPositionTex;
	D3DXMatrixTranslation( &mPositionTex, positionXTex, positionYTex, 0.0f );

	static D3DXMATRIX mScaleTex;
	D3DXMatrixScaling( &mScaleTex, scaleXTex, scaleYTex, 0.0f );

	static D3DXMATRIX mRotationTex;
	D3DXMatrixRotationX( &mRotationTex, rotation );

	mTexMtx = mScaleTex * mRotationTex * mPositionTex;	// position�������
}

void Mesh::TranslationTex( float x, float y )
{
	positionXTex += x, positionYTex += y;

	static D3DXMATRIX transMtr;
	D3DXMatrixTranslation( &transMtr, x, y, 0.0f );
	mTexMtx = mTexMtx * transMtr;	// position�������
}
void Mesh::ScaleTex( float sX, float sY )
{
	scaleXTex *= sX, scaleYTex *= sY;
	static D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, sX, sY, 0.0f );
	mTexMtx = mScale * mTexMtx;	// position�������
}
void Mesh::RotateTex( float rotation )
{
	rotationTex += rotation;

	static D3DXMATRIX mPositionTex;
	D3DXMatrixTranslation( &mPositionTex, positionXTex, positionYTex, 0.0f );

	static D3DXMATRIX mScaleTex;
	D3DXMatrixScaling( &mScaleTex, scaleXTex, scaleYTex, 0.0f );

	static D3DXMATRIX mRotationTex;
	D3DXMatrixRotationX( &mRotationTex, rotation );

	mTexMtx = mScaleTex * mRotationTex * mPositionTex;	// position�������
}

BOOL Mesh::Picked()
{
	RayLine rayLine = mPickRayLine;
	static D3DXMATRIX inverseW;
	D3DXMatrixInverse(&inverseW, 0, &mCreateWorld);

	D3DXVec3TransformCoord(&rayLine.pos, &rayLine.pos, &inverseW);
	D3DXVec3TransformNormal(&rayLine.dir, &rayLine.dir, &inverseW);

	return D3DXBoxBoundProbe( &mMin, &mMax, &rayLine.pos, &rayLine.dir);
}