#include "Sky.h"


struct SkyVertex
{
	D3DXVECTOR3 pos;
};

Sky::Sky()
: mVB(0), mIB(0), mCubeMap(0), visible(false)
{
	mNumIndices = 0;

	stride = sizeof(SkyVertex);
    offset = 0;

	mTech         = SkyFX->GetTechniqueByName("SkyTech");
	mfxWVPVar     = SkyFX->GetVariableByName("gWVP")->AsMatrix();
	mfxCubeMapVar = SkyFX->GetVariableByName("gCubeMap")->AsShaderResource();
}

Sky::~Sky()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Sky::init(ID3D10ShaderResourceView* cubemap, float radius)
{
	mCubeMap   = cubemap;

	std::vector<D3DXVECTOR3> vertices;
	std::vector<DWORD> indices;

	BuildGeoSphere(2, radius, vertices, indices);

	std::vector<SkyVertex> skyVerts(vertices.size());
	for(size_t i = 0; i < vertices.size(); ++i)
	{
		// Scale on y-axis to turn into an ellipsoid to make a flatter Sky surface
		skyVerts[i].pos = 0.5f*vertices[i];
	}

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SkyVertex) * (UINT)skyVerts.size();
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &skyVerts[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	mNumIndices = (UINT)indices.size();

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumIndices;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Sky::draw()
{
	if(visible)
	{
		static D3DXVECTOR3 eyePos;
		eyePos = GetCamera().position();

		// center Sky about eye in world space
		static D3DXMATRIX W;
		D3DXMatrixTranslation(&W, eyePos.x, eyePos.y, eyePos.z);

		static D3DXMATRIX WVP;
		WVP = W*mView*mProj_3D;

		HR(mfxWVPVar->SetMatrix((float*)WVP));
		HR(mfxCubeMapVar->SetResource(mCubeMap));


		md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		md3dDevice->IASetInputLayout(Pos);
		md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	

		mTech->GetPassByIndex(0)->Apply(0);
		md3dDevice->DrawIndexed(mNumIndices, 0, 0);
	}
}
 