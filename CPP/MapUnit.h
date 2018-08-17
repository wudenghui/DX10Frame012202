#ifndef MAP_UNIT
#define MAP_UNIT

#include"ModelsDemo.h"

class MapUnit:public ModelsDemo
{
public:
	MapUnit( SourceLoader* source,
			float posX, float posY, float posZ,
			float unitLength, float unitWidth,	// 单元格宽高，用于创建VertexBuffer
			GamePic* srcPic);
	~MapUnit();

	bool SetHLeftTop(float height);
	bool SetHRightTop(float height);
	bool SetHRightBottom(float height);
	bool SetHLeftBottom(float height);
	
private:
	VertexPos3D *vertices;
	D3D11_MAPPED_SUBRESOURCE mapResource;

	// 可以拓展
};



#endif