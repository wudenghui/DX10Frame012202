#include"Sprite.h"

Sprite::Sprite()
{
	visible = true;
	positionX = positionY = width = height = scaleX = scaleY = rotation = 0.0f;


	mNumVertices = mNumFaces = 0;

	D3DXMatrixIdentity(&mCreateWorld);
	D3DXMatrixIdentity(&mWVP);

	stride = sizeof(Vertex_2D);
    offset = 0;

	picIndex = 0;
}

Sprite::~Sprite()
{
	if(mVB)
		ReleaseCOM(mVB);
}

void Sprite::CreateVB(float w, float h)
{
	float tw = w/2.0f, th = h/2.0f;
	Vertex_2D v[6];
	v[0] = Vertex_2D(-tw, -th, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex_2D(-tw, th, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex_2D(tw,  th, 0.0f, 1.0f, 0.0f);

	v[3] = Vertex_2D(-tw, -th, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex_2D(tw,  th, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex_2D(tw, -th, 0.0f, 1.0f, 1.0f);

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex_2D) * 6;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = v;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}

void Sprite::Init(
	float w, float h,
	float posX, float posY,
	float sX, float sY,
	float r,
	ID3D10ShaderResourceView** pa, int np,
	ID3D10Effect* fx,					// 效果
	char* techName
	)
{
	width = w, height = h;
	CreateVB(w, h);

	positionX = posX, positionY = posY;
	D3DXMATRIX mPosition;
	D3DXMatrixTranslation( &mPosition, posX, posY, 0.0f );

	scaleX = sX, scaleY =sY;
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, sX, sY, 1.0f);

	rotation = r;
	D3DXMATRIX rM;
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

void Sprite::DrawContent()
{
	if(visible == false)
		return ;
	mWVP = mCreateWorld * mProj_2D;
	mfxWVPVar->SetMatrix((float*)&mWVP);

    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	mfxMapVar->SetResource(picArray[picIndex]);

	ID3D10EffectPass* pass = mTech->GetPassByIndex(0);
	pass->Apply(0);
	md3dDevice->Draw(6, 0);
}

void Sprite::SetPic(ID3D10ShaderResourceView* pic, int index)
{
	if(index>numPic)
	{
		MessageBox(0,"图片索引超限","Sprite",0);
		return;
	}
	picArray[index] = pic;
}


// 世界矩阵相关
void Sprite::SetPosition( float pX, float pY )
{
	positionX = pX, positionY = pY;
	static D3DXMATRIX mPosition;
	D3DXMatrixTranslation( &mPosition, positionX, positionY, 0.0f );

	static D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scaleX, scaleY, 1.0f);

	static D3DXMATRIX rM;
	D3DXMatrixRotationZ( &rM, rotation );

	mCreateWorld = mScale * rM * mPosition;	// position放在最后
}
void Sprite::SetScale( float sX, float sY )
{
	static D3DXMATRIX mPosition;
	D3DXMatrixTranslation( &mPosition, positionX, positionY, 0.0f );

	scaleX = sX, scaleY = sY;
	static D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scaleX, scaleY, 1.0f);

	static D3DXMATRIX rM;
	D3DXMatrixRotationZ( &rM, rotation );

	mCreateWorld = mScale * rM * mPosition;	// position放在最后
}

void Sprite::SetRotation( float r )
{
	static D3DXMATRIX mPosition;
	D3DXMatrixTranslation( &mPosition, positionX, positionY, 0.0f );

	static D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scaleX, scaleY, 1.0f);

	rotation = r;
	static D3DXMATRIX rM;
	D3DXMatrixRotationZ( &rM, rotation );

	mCreateWorld = mScale * rM * mPosition;	// position放在最后
}

void Sprite::Translation( float x, float y )
{
	positionX += x, positionY += y;
	static D3DXMATRIX translation;
	D3DXMatrixTranslation( &translation, x, y, 0.0f );
	mCreateWorld *= translation;
}
void Sprite::Scale( float sX, float sY )
{
	scaleX *= sX, scaleY *= sY;
	static D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scaleX, scaleY, 1.0f);
	mCreateWorld = mScale * mCreateWorld;
}
void Sprite::Rotate( float r )
{
	static D3DXMATRIX mPosition;
	D3DXMatrixTranslation( &mPosition, positionX, positionY, 0.0f );

	static D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scaleX, scaleY, 1.0f);

	rotation += r;
	static D3DXMATRIX rM;
	D3DXMatrixRotationZ( &rM, rotation );

	mCreateWorld = mScale * rM * mPosition;	// position放在最后
}