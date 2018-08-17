#ifndef GAME_OBJ
#define GAME_OBJ

#include"d3dUtil.h"

#define	MOM	true;
#define CHILD	false;

class GameObj
{
public:
	GameObj();
	~GameObj();

	// 采用虚函数与回调函数并存的机制，增加灵活性，可以重写ActionUpdate在类的层次上多态，也可以使用回调函数，在对象层面上多态
	virtual void ActionUpdate(float dt);
	virtual void DrawContent(){	}

	// 阴影Shadow
	virtual void DrawContentShadow(ID3D10EffectMatrixVariable* mfxLightWVPVar, D3DXMATRIX lightVP){	}
	virtual void DrawToShadowMap(
		ID3D10EffectMatrixVariable* mfxBuildShadowMapLightWVPVar,
		D3DXMATRIX lightVP,
		ID3D10EffectShaderResourceVariable* diffuseMapVar,
		ID3D10EffectPass* pass){}

	// 碰撞检测
	virtual bool Crash(GameObj* mesh){	return false;	}	// 碰撞检测
	virtual bool Crashed(GameObj* mesh){	return false;	}
	// 基础函数
	void SetAction(  void(* Func)(float ) );	// 参数// 函数指针
protected:
	void (*Action)(float );	// 回调函数，使程序模块化更强
	void* pParameter;

	// 用于删除链表节点
public:
	void ListReleased(){	mList = NULL, listID = NULL;	}
	void SetListID(void* id){	listID = id;	}
	void SetList(void* list){	mList = list;	}	// 方便删除节点


	// 碰撞链表永远是最后释放
	void CrashListReleased(){	mCrashList = NULL, mCrashListID = NULL;	}
	void SetCrashListID(void* id){	mCrashListID = id;	}
	void SetCrashList(void* list){	mCrashList = list;	}	// 方便删除节点
	void* GetCrashListID(){	return mCrashListID;	} 
	void* GetCrashList(){	return mCrashList;	}
protected:
	void* listID;
	void* mList;

	void* mCrashListID;
	void* mCrashList;
};


typedef class DemoPoint
{
public:
	// 2D图像链表节点
	DemoPoint* pNext;
	DemoPoint* pPrev;
	GameObj* pContent;
	// 节点在链表中的序号，待定
	// UINT index;
}*pDemoPoint, DemoPoint;

#endif