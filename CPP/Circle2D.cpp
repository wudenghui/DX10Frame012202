#include"Circle2D.h"

Circle_2D::Circle_2D()
{
	mRadius = 0;
	mSlices = 0;
}

Circle_2D::~Circle_2D()
{
	
}

void Circle_2D::Init(
	float r, int slices,
	float posX, float posY,
	float sX, float sY,
	float rot,
	ID3D10ShaderResourceView** pa, int np,
	ID3D10Effect* fx,					// 效果
	char* techName)
{
	VertexList_2D vertices;
	IndexList indices;

	mSlices = slices;
	width = height = mRadius = r;	// 用于碰撞检测
	CreateCircleFace(vertices, indices, r, slices);

	positionX = posX, positionY = posY;
	D3DXMATRIX mPosition;
	D3DXMatrixTranslation( &mPosition, posX, posY, 0.0f );

	scaleX = sX, scaleY =sY;
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, sX, sY, 1.0f);

	rotation = rot;
	static D3DXMATRIX rM;
	D3DXMatrixRotationZ( &rM, rotation );

	mCreateWorld = mScale * rM * mPosition;	// position放在最后
	mWVP = mCreateWorld;

	numPic = np;
	picArray = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*numPic);
	for(int i=0; i<numPic; i++)
	{
		picArray[i] = pa[i];
	}

	mFX = fx;
	if(mFX == 0)
		return ;
	mTech = mFX->GetTechniqueByName(techName);
	mfxWVPVar	= mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxMapVar	= mFX->GetVariableByName("gMap")->AsShaderResource();
}

void Circle_2D::DrawContent()
{
	if(visible == false)
		return ;
	mWVP = mCreateWorld * mProj_2D;

	md3dDevice->IASetInputLayout(PosTex);
	mfxWVPVar->SetMatrix((float*)&mWVP);
	mfxMapVar->SetResource(picArray[picIndex]);

	ID3D10EffectPass* pass = mTech->GetPassByIndex(0);
	pass->Apply(0);

	md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
    md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}

// 画圆、正多边形，30个切片时大致为圆
void Circle_2D::CreateCircleFace(VertexList_2D& vertices, IndexList& indices, float r, int slices)
{
	UINT baseIndex = (UINT)vertices.size();

	// vertices of ring
	float dTheta = 2.0f*PI/ (float)slices;
	for(UINT i = 0; i < (UINT)slices; ++i)
	{
		float x = r*cosf(i*dTheta);
		float y = r*sinf(i*dTheta);

		// Map [-1,1]-->[0,1] for planar texture coordinates.
		float u = +0.5f*x/r + 0.5f;
		float v = -0.5f*y/r + 0.5f;

		vertices.push_back( Vertex_2D(x, y, 0.0f, u, v) );

		mNumVertices++;
	}

	// cap center vertex
	vertices.push_back( Vertex_2D(0.0f, 0.0f, 0.0f, 0.5f, 0.5f) );
	mNumVertices++;

	// index of center vertex
	UINT centerIndex = (UINT)vertices.size()-1;

	for(UINT i = 0; i < (UINT)slices; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i+1);

		mNumFaces++;
	}

	indices.push_back(centerIndex);
	indices.push_back(baseIndex);
	indices.push_back(baseIndex+slices-1);
	mNumFaces++;

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex_2D) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}