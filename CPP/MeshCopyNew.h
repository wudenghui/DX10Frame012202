#ifndef MESH_COPY_NEW
#define MESH_COPY_NEW

/*
	��������Ƕ�����������ĸ����ȫ����
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