#include"Vertex.h"

void ComputeNorm_Vertex( Vertex_Norm* destNorm,	// Ŀ�귨������
	Vertex_Pos* srcPos, DWORD numVertices,	// ����λ�����顢��������
	DWORD* srcIB, DWORD numFaces )			// �����������顢������������
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

}	// ����ComputeNorm()



D3DXVECTOR3 ComputeNorm_Face(D3DXVECTOR3* A, D3DXVECTOR3* B, D3DXVECTOR3* C)	// ���������εķ�������
{
	D3DXVECTOR3 e0 = (*A) - (*B);
	D3DXVECTOR3 e1 = (*B) - (*C);

	D3DXVECTOR3 faceNormal;
	D3DXVec3Cross(&faceNormal, &e0, &e1);

	return faceNormal;
}	// ����ComputeNorm()