#include"GameObj.h"

GameObj::GameObj()
{
	Action = NULL;
	pParameter = NULL;
	mList = listID = NULL;
	mCrashList = mCrashListID = NULL;
}	// 结束GameObj()

GameObj::~GameObj()
{
	Action = NULL;
	pParameter = NULL;
	mList = listID = NULL;
	mCrashList = mCrashListID = NULL;
}	// 结束GameObj()

void GameObj::SetAction( void(* Func)(float) )
{
	Action = Func;
}	// 结束SetAction()

void GameObj::ActionUpdate(float dt)
{
	if(Action == NULL)
		return ;
	Action(dt);
	return;
}	// 结束ActionUpdate()