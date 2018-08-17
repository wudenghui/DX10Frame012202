#include"DemoView.h"

DemoView::DemoView():
	position_x( 0 ), position_y( 0 ), position_z( 0.0f ),
	scale_x( 1.0f ), scale_y( 1.0f ), scale_z( 1.0f ),
	rotation_x( 0.0f ), rotation_y( 0.0f ), rotation_z( 0.0f ),
	visible( 0 ), curVertexBufferIndex( 0 ), curPicIndex( 0 )
{
	::InitializeCriticalSection(&cs); // 初始化临界区变量
}	// 结束DemoView()

DemoView::~DemoView()
{
	gamePic_ = 0;
	vertexBuffer_ = 0;
	curVertexBufferIndex = 0;
	curPicIndex = 0;
	::DeleteCriticalSection(&cs);	// 释放临界区变量
}	// 结束~DemoView()

void DemoView::Hide()
{
	::EnterCriticalSection(&cs);   //最重要的部分，也是临界区对象发挥作用的地方
	visible = false;
	::LeaveCriticalSection(&cs);	// 退出临界区变量
}	// 结束Hide()

void DemoView::Show()
{
	::EnterCriticalSection(&cs);   //最重要的部分，也是临界区对象发挥作用的地方
	visible = true;
	::LeaveCriticalSection(&cs);	// 退出临界区变量
}	// 结束Show()

bool DemoView::GetVisible()
{
	return visible;
}	// 结束GetVisible()

void DemoView::SetVertexIndex( int index )
{
	if(index>vertexBuffer_->numVB)
	{
		MessageBox(NULL, "顶点缓冲序号徐浩超限", "SetVertexBuffer错误", 50);
		return ;
	}
	::EnterCriticalSection(&cs);   //最重要的部分，也是临界区对象发挥作用的地方
	curVertexBufferIndex = index;
	::LeaveCriticalSection(&cs);	// 退出临界区变量
}	// 结束SetVertex()

void DemoView::SetPicIndex( int index )
{
	if(index>gamePic_->GetNumPic())
	{
		MessageBox(NULL, "图片徐浩超限", "SetPic错误", 50);
		return ;
	}
	::EnterCriticalSection(&cs);   //最重要的部分，也是临界区对象发挥作用的地方
	curPicIndex = index;
	::LeaveCriticalSection(&cs);	// 退出临界区变量
}