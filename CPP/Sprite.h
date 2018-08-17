#ifndef SPRITE
#define SPRITE

#include"Vertex.h"
#include"Directx10.h"
#include"GameObj.h"

class Sprite : public GameObj
{
public:
	Sprite();
	~Sprite();

	void Init(
		float w, float h,
		float posX, float posY,
		float sX, float sY,
		float r,
		ID3D10ShaderResourceView** pa, int np,
		ID3D10Effect* fx,					// 效果
		char* techName
		);

	virtual void AtionUpdate(float dt){	}
	virtual void DrawContent();

	void CreateVB(float w, float h);

public:
	bool GetVisible(){	return visible;	}
	void Show(){	visible = true;	}
	void Hide(){	visible = false; }

	void SetWidth(float w);
	void SetHeight(float h);
	void SetPicIndex(int index){	picIndex = index;	}
	void SetPic(ID3D10ShaderResourceView* pic, int index);

	// 世界矩阵相关
	D3DXMATRIX World(){	return  mCreateWorld;	}
	void SetPosition( float pX, float pY );
	void SetScale( float sX, float sY );
	void SetRotation( float r );

	void Translation( float x, float y );
	void Scale( float sX, float sY );
	void Rotate( float r );
protected:
	DWORD mNumVertices;
	DWORD mNumFaces;
	
	UINT stride;
    UINT offset;

	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;

	int picIndex;
	int numPic;
	ID3D10ShaderResourceView** picArray;

protected:
	D3DXMATRIX mWVP;
	D3DXMATRIX mCreateWorld;

protected:
	bool visible;
	// 模型相关变量
	float positionX, positionY;
	float width, height;
	float scaleX, scaleY;
	float rotation;

protected:
	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectShaderResourceVariable* mfxMapVar;
};


#endif