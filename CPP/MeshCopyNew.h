#ifndef MESH_COPY_NEW
#define MESH_COPY_NEW

/*
	这个拷贝是独立拷贝，与母类完全独立
*/

#include"Mesh.h"


class MeshCopyNew : public Mesh
{
public:
	MeshCopyNew();
	~MeshCopyNew();

	void Copy(Mesh& mesh);
};


#endif