#ifndef MOUSE
#define MOUSE

/*
	单例模式
	重要说明：鼠标独立于精灵链表，单独、最后绘制，使鼠标可以始终处于最顶端
*/

#include"DxInput.h"
#include"Sprite.h"

extern char MousePos[512];

#define NORMAL			0
#define LKEY_PRESSED	1
#define LKEY_CLICKED	2
#define RKEY_PRESSED	3
#define RKEY_CLICKED	4



typedef struct POSITION
{
public:
	POSITION(){	;	}
	POSITION(float posX, float posY){	x = posX, y = posY;	}

	float x;
	float y;
}*PPOSITION, POSITION;

class Mouse : public GameObj
{
public:
	friend Mouse& GetMouse();

	void Init(
		ID3D10ShaderResourceView** pa, int np,
		ID3D10Effect* fx,					// 效果
		char* techName
		);

	Sprite& GetCursor(){	return mCursor;	}
	POSITION GetPositoin(){	return POSITION(position_x, position_y);	}
	float GetPosX(){	return position_x;	}
	float GetPosY(){	return position_y;	}
	float GetDeltaX(){	return delta_x;	}
	float GetDeltaY(){	return delta_y;	}
	float GetMouseWheel(){	return mouseWheel_;	}
	float GetSpeed(){	return speed;	}
	int GetState(){	return state;	}

	void SetSpeed(float spd);
	void ActionUpdate(float dt);
	void DrawContent();

	void CatchObj(){	mCatchObj = true;	}

	void InitDevice();
	void Release();

private:
	LPDIRECTINPUTDEVICE8 mouseDevice_;
	DIMOUSESTATE mouseState_;
	DIMOUSESTATE prevMouseState_;

	// 计算拾取射线
	void ComputePickRay(float sx, float sy);

private:
	Mouse();
	~Mouse();
	
	int state;
	float position_x, position_y;
	float delta_x, delta_y;
	float mouseWheel_;	// 鼠标滚轮
	float speed;
	
	bool mCatchObj;

private:
	Sprite mCursor;

};

Mouse& GetMouse();

#endif