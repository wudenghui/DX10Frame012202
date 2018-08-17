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

	// �����麯����ص���������Ļ��ƣ���������ԣ�������дActionUpdate����Ĳ���϶�̬��Ҳ����ʹ�ûص��������ڶ�������϶�̬
	virtual void ActionUpdate(float dt);
	virtual void DrawContent(){	}

	// ��ӰShadow
	virtual void DrawContentShadow(ID3D10EffectMatrixVariable* mfxLightWVPVar, D3DXMATRIX lightVP){	}
	virtual void DrawToShadowMap(
		ID3D10EffectMatrixVariable* mfxBuildShadowMapLightWVPVar,
		D3DXMATRIX lightVP,
		ID3D10EffectShaderResourceVariable* diffuseMapVar,
		ID3D10EffectPass* pass){}

	// ��ײ���
	virtual bool Crash(GameObj* mesh){	return false;	}	// ��ײ���
	virtual bool Crashed(GameObj* mesh){	return false;	}
	// ��������
	void SetAction(  void(* Func)(float ) );	// ����// ����ָ��
protected:
	void (*Action)(float );	// �ص�������ʹ����ģ�黯��ǿ
	void* pParameter;

	// ����ɾ������ڵ�
public:
	void ListReleased(){	mList = NULL, listID = NULL;	}
	void SetListID(void* id){	listID = id;	}
	void SetList(void* list){	mList = list;	}	// ����ɾ���ڵ�


	// ��ײ������Զ������ͷ�
	void CrashListReleased(){	mCrashList = NULL, mCrashListID = NULL;	}
	void SetCrashListID(void* id){	mCrashListID = id;	}
	void SetCrashList(void* list){	mCrashList = list;	}	// ����ɾ���ڵ�
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
	// 2Dͼ������ڵ�
	DemoPoint* pNext;
	DemoPoint* pPrev;
	GameObj* pContent;
	// �ڵ��������е���ţ�����
	// UINT index;
}*pDemoPoint, DemoPoint;

#endif