//=======================================================================================
// PeaksAndValleys.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "PeaksAndValleys.h"


PeaksAndValleys::PeaksAndValleys()
: mNumRows(0), mNumCols(0)
{

}

PeaksAndValleys::~PeaksAndValleys()
{
}

float PeaksAndValleys::getHeight(float x, float z)const
{
	return 0.3f*( z*sinf(0.1f*x) + x*cosf(0.1f*z) );
}

void PeaksAndValleys::init( DWORD m, DWORD n, float dx)
{
	mNumRows  = m;
	mNumCols  = n;

	mNumVertices = m*n;
	mNumFaces    = (m-1)*(n-1)*2;

	stride = sizeof(Vertex_3D);
	offset = 0;

	// �����������Ҫ�޸�
	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);

	// Create the geometry and fill the vertex buffer. 

	std::vector<Vertex_3D> vertices(mNumVertices);
	float halfWidth = (n-1)*dx*0.5f;
	float halfDepth = (m-1)*dx*0.5f;
	for(DWORD i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dx;
		for(DWORD j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			// Graph of this function looks like a mountain range.
			float y = getHeight(x,z);

			vertices[i*n+j].pos = D3DXVECTOR3(x, y, z);

			if( y < -10.0f )
			{
				vertices[i*n+j].texC.x = du*j;
				vertices[i*n+j].texC.y = dv*i;
			}
			else if( y < 5.0f )
			{
				vertices[i*n+j].texC.x = du*j;
				vertices[i*n+j].texC.y = dv*i;
			}
			else if( y < 12.0f )
			{
				vertices[i*n+j].texC.x = du*j;
				vertices[i*n+j].texC.y = dv*i;
			}
			else if( y < 20.0f )
			{
				vertices[i*n+j].texC.x = du*j;
				vertices[i*n+j].texC.y = dv*i;
			}
			else
			{
				vertices[i*n+j].texC.x = du*j;
				vertices[i*n+j].texC.y = dv*i;
			}
			// n = (-df/dx, 1, -df/dz)
			D3DXVECTOR3 normal;
			normal.x = -0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z);
			normal.y = 1.0f;
			normal.z = -0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z);
			D3DXVec3Normalize(&vertices[i*n+j].normal, &normal);
		}
	}
 
    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex_3D) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer.  The index buffer is fixed, so we only 
	// need to create and set once.

	std::vector<DWORD> indices(mNumFaces*3); // 3 indices per face

	// Iterate over each quad and compute indices.
	int k = 0;
	for(DWORD i = 0; i < m-1; ++i)
	{
		for(DWORD j = 0; j < n-1; ++j)
		{
			indices[k]   = i*n+j;
			indices[k+1] = i*n+j+1;
			indices[k+2] = (i+1)*n+j;

			indices[k+3] = (i+1)*n+j;
			indices[k+4] = i*n+j+1;
			indices[k+5] = (i+1)*n+j+1;

			k += 6; // next quad
		}
	}

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

void PeaksAndValleys::update(float dt)
{
}

void PeaksAndValleys::draw()
{
	md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
    md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}