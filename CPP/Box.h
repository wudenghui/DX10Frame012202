//=======================================================================================
// Box.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef BOX_H
#define BOX_H

#include"Mesh.h"

class Box : public Mesh
{
public:

	Box();
	~Box();
	void CreateMesh(  float length, float width, float height, UINT numSubsets);
	void Init( 
		float length, float width, float height,
		float pX, float pY, float pZ,		// 坐标
		ID3D10Effect* fx,					// 效果
		char* techName, char* shadowTechName,
		UINT numSubsets,
		ID3D10ShaderResourceView** diffuseMapRVArray,
		ID3D10ShaderResourceView** specMapRVArray,
		ID3D10ShaderResourceView** normMapRVArray,
		D3DXVECTOR3* reflectMtrArray	// 反射材质
		);
	float GetLength(){	return mLength;	}
	float GetWidth(){	return mWidth;	}
	float GetHeight(){	return mHeight;	}
protected:
	float mLength, mWidth, mHeight;
};


#endif // BOX_H
