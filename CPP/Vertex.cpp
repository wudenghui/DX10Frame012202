#include"Vertex.h"

void ComputeNorm_Vertex( Vertex_Norm* destNorm,	// 目标法线数组
	Vertex_Pos* srcPos, DWORD numVertices,	// 顶点位置数组、顶点容量
	DWORD* srcIB, DWORD numFaces )			// 顶点索引数组、顶点索引容量
{
	DWORD i = 0;
	DWORD t1, t2, t3;

	Vertex_Norm vertexNorm[3];

	for(i=0; i<numFaces; i++)
	{
		t1 = srcIB[i*3];
		t2 = srcIB[i*3+1];
		t3 = srcIB[i*3+2];

		Vertex_Norm faceNorm = ComputeNorm_Face( srcPos+t1, srcPos+t2, srcPos+t3);

		destNorm[t1] += faceNorm;
		destNorm[t2] += faceNorm;
		destNorm[t3] += faceNorm;
	}
	
	for(i=0; i<numVertices; i++)
	{
		D3DXVec3Normalize( destNorm+i, destNorm+i);
	}

}	// 结束ComputeNorm()



D3DXVECTOR3 ComputeNorm_Face(D3DXVECTOR3* A, D3DXVECTOR3* B, D3DXVECTOR3* C)	// 计算三角形的法线向量
{
	D3DXVECTOR3 e0 = (*A) - (*B);
	D3DXVECTOR3 e1 = (*B) - (*C);

	D3DXVECTOR3 faceNormal;
	D3DXVec3Cross(&faceNormal, &e0, &e1);

	return faceNormal;
}	// 结束ComputeNorm()