#include"MyMesh.h"

MyMesh::MyMesh() : 
mNumVertices(0), mNumFaces(0), mVB(0), mIB(0)
{
	
}

MyMesh::~MyMesh()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}
