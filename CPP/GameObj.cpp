#include"GameObj.h"

GameObj::GameObj()
{
	Action = NULL;
	pParameter = NULL;
	mList = listID = NULL;
	mCrashList = mCrashListID = NULL;
}	// ����GameObj()

GameObj::~GameObj()
{
	Action = NULL;
	pParameter = NULL;
	mList = listID = NULL;
	mCrashList = mCrashListID = NULL;
}	// ����GameObj()

void GameObj::SetAction( void(* Func)(float) )
{
	Action = Func;
}	// ����SetAction()

void GameObj::ActionUpdate(float dt)
{
	if(Action == NULL)
		return ;
	Action(dt);
	return;
}	// ����ActionUpdate()