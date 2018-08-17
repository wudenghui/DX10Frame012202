#include"DemoView.h"

DemoView::DemoView():
	position_x( 0 ), position_y( 0 ), position_z( 0.0f ),
	scale_x( 1.0f ), scale_y( 1.0f ), scale_z( 1.0f ),
	rotation_x( 0.0f ), rotation_y( 0.0f ), rotation_z( 0.0f ),
	visible( 0 ), curVertexBufferIndex( 0 ), curPicIndex( 0 )
{
	::InitializeCriticalSection(&cs); // ��ʼ���ٽ�������
}	// ����DemoView()

DemoView::~DemoView()
{
	gamePic_ = 0;
	vertexBuffer_ = 0;
	curVertexBufferIndex = 0;
	curPicIndex = 0;
	::DeleteCriticalSection(&cs);	// �ͷ��ٽ�������
}	// ����~DemoView()

void DemoView::Hide()
{
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط�
	visible = false;
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����Hide()

void DemoView::Show()
{
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط�
	visible = true;
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����Show()

bool DemoView::GetVisible()
{
	return visible;
}	// ����GetVisible()

void DemoView::SetVertexIndex( int index )
{
	if(index>vertexBuffer_->numVB)
	{
		MessageBox(NULL, "���㻺�������Ƴ���", "SetVertexBuffer����", 50);
		return ;
	}
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط�
	curVertexBufferIndex = index;
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetVertex()

void DemoView::SetPicIndex( int index )
{
	if(index>gamePic_->GetNumPic())
	{
		MessageBox(NULL, "ͼƬ��Ƴ���", "SetPic����", 50);
		return ;
	}
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط�
	curPicIndex = index;
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}