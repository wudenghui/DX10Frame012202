#ifndef MY_MESH
#define MY_MESH

#include "Directx10.h"
#include "Vertex.h"

class MyMesh
{
public:
	MyMesh();
	~MyMesh();

	virtual void update(float dt){		}
	virtual void draw(){		}

protected:
	DWORD mNumVertices;
	DWORD mNumFaces;
	
	UINT stride;
    UINT offset;

	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
};

#endif