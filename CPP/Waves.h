//=======================================================================================
// Waves.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef WAVES_H
#define WAVES_H

#include"MyMesh.h"

class Waves :  public MyMesh
{
public:
	Waves();
	~Waves();

 
	void init(DWORD m, DWORD n, float dx, float dt, float speed, float damping);
	void update(float dt);
	void disturb(DWORD i, DWORD j,	// 扰动坐标
		float magnitude);			// 扰动强度
	void draw();

private:
	DWORD mNumRows;
	DWORD mNumCols;

	// Simulation constants we can precompute.
	float mK1;
	float mK2;
	float mK3;

	float mTimeStep;
	float mSpatialStep;

	D3DXVECTOR3* mPrevSolution;
	D3DXVECTOR3* mCurrSolution;
	D3DXVECTOR3* mNormals;
};


#endif // WAVES_H