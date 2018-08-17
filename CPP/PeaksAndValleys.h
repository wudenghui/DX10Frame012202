//=======================================================================================
// PeaksAndValleys.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef PEAKSANDVALLEYS_H
#define PEAKSANDVALLEYS_H

#include "MyMesh.h"

class PeaksAndValleys :  public MyMesh
{
public:
	PeaksAndValleys();
	~PeaksAndValleys();

	float getHeight(float x, float z)const;		// �߶����ɹ�ʽ
 
	void init( DWORD m, DWORD n,	// ����������
		float dx);	// ����
	void update(float dt);
	void draw();

private:
	DWORD mNumRows;
	DWORD mNumCols;
};



#endif // PEAKSANDVALLEYS_H