#ifndef CIRCLE_2D
#define CIRCLE_3D

#include"Sprite.h"
#include"Camera.h"

class Circle_2D : public Sprite
{
public:
	Circle_2D();
	~Circle_2D();

	void Init(
		float r, int slices,
		float posX, float posY,
		float sX, float sY,
		float rot,
		ID3D10ShaderResourceView** pa, int np,
		ID3D10Effect* fx,					// 效果
		char* techName
		);

	void DrawContent();

	typedef std::vector<Vertex_2D> VertexList_2D;
	typedef std::vector<DWORD> IndexList;

	// 画圆、正多边形，30个切片时大致为圆
	void CreateCircleFace(VertexList_2D& vertices, IndexList& indices, float r, int slices);

private:
	float mRadius;
	int mSlices;
};

#endif