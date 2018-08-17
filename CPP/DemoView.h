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
	virtual void SetWorldMatrix( ){	};	// ��Ҫ����ʵ��
	void Show();
	void Hide();
	bool GetVisible();

	virtual void SetPic( GamePic*  pic ){	};
	virtual void SetVertexBuffer( VertexBuffer* srcVB ){	};

	void SetPicIndex( int index );	// ����ʹ����Դ�������������Դ�������ͷ���Դ��ɵ�����
	void SetVertexIndex( int index );


public:
	float position_x, position_y, position_z;
	SourceLoader* source_;

protected:
	CRITICAL_SECTION cs;	// �ٽ�������

	unsigned int stride;	// �����Ż�
	unsigned int offset;

    GamePic* gamePic_;
	int curPicIndex;

	VertexBuffer* vertexBuffer_;
	int curVertexBufferIndex;

    XMFLOAT4X4 projMatrix_;
	XMFLOAT4X4 worldMatrix_;

	bool visible;		// ��ʾ���Ʊ�����Ĭ�ϲ��ɼ�
	float scale_x, scale_y, scale_z;
	float rotation_x, rotation_y, rotation_z;
};


#endif