#ifndef DEMO_VIEW
#define DEMO_VIEW

#include"ResourceList.h"

class DemoView
{
public:
	DemoView();
	~DemoView();
	
	virtual void Update( float dt ){	};
	virtual void Render( ){	};

	////////////////////////////
	virtual void SetWorldMatrix( ){	};	// 需要子类实现
	void Show();
	void Hide();
	bool GetVisible();

	virtual void SetPic( GamePic*  pic ){	};
	virtual void SetVertexBuffer( VertexBuffer* srcVB ){	};

	void SetPicIndex( int index );	// 尽量使用资源管理器定义的资源，避免释放资源造成的问题
	void SetVertexIndex( int index );


public:
	float position_x, position_y, position_z;
	SourceLoader* source_;

protected:
	CRITICAL_SECTION cs;	// 临界区变量

	unsigned int stride;	// 可以优化
	unsigned int offset;

    GamePic* gamePic_;
	int curPicIndex;

	VertexBuffer* vertexBuffer_;
	int curVertexBufferIndex;

    XMFLOAT4X4 projMatrix_;
	XMFLOAT4X4 worldMatrix_;

	bool visible;		// 显示控制变量，默认不可见
	float scale_x, scale_y, scale_z;
	float rotation_x, rotation_y, rotation_z;
};


#endif