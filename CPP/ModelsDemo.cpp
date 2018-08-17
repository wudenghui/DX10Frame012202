/*
    3Dģ��demo
*/


#include"ModelsDemo.h"
#include"objLoader.h"


ModelsDemo::ModelsDemo( ) : 
	viewCB_( 0 ), projCB_( 0 ), worldCB_( 0 )
{
	stride = sizeof( VertexPos3D );	// �����Ż�
	offset = 0;
	::InitializeCriticalSection(&cs); // ��ʼ���ٽ�������
}	// ����ModelsDemo( )


ModelsDemo::~ModelsDemo( )
{
	UnloadContent( );
}	// ����~ModelsDemo( )


bool ModelsDemo::LoadContent( SourceLoader* source,
	float posX, float posY, float posZ,
	float rotX, float rotY, float rotZ,
	float sX, float sY, float sZ)
{
	source_ = source;	// ��Դ

	position_x = posX;
	position_y = posY;
	position_z = posZ;
	scale_x = sX;
	scale_y = sY;
	scale_z = sZ;

	rotation_x = rotX;
	rotation_y = rotY;
	rotation_z = rotZ;

	SetWorldMatrix();
	vertexBuffer_ = source->defVertexBuffer3D;
	gamePic_ = source->defPic;

	viewCB_ = source->viewCB_;
	projCB_ = source->projCB_;
	worldCB_ = source->worldCB_;
	camPosCB_ = source->camPosCB_;

	projMatrix_ = source->projMatrix3D_;

    return true;
}	// ����LoadContent( )


void ModelsDemo::UnloadContent( )
{
    if( gamePic_ ) gamePic_ = 0;
    if( vertexBuffer_ ) vertexBuffer_ = 0;
    if( viewCB_ ) viewCB_ = 0;
    if( projCB_ ) projCB_ = 0;
    if( worldCB_ ) worldCB_ = 0;
	if( camPosCB_ ) camPosCB_ = 0;
}	// ����UnloadContent( )


void ModelsDemo::Update( float dt )
{
	if( d3dContext_ == 0 )
        return;

	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط� 

	d3dContext_->IASetVertexBuffers( 0, 1, &vertexBuffer_->vertexBuffer_[curVertexBufferIndex], &stride, &offset );

	//d3dContext_->IASetIndexBuffer( vertexBuffer_->indexBuffer_[curVertexBufferIndex], DXGI_FORMAT_R16_UINT, 0 );

	d3dContext_->PSSetShaderResources( 0, 1, gamePic_->GetPic(curPicIndex) );

    XMMATRIX worldMat;
	Float4x4_To_Matrix( &worldMat, &worldMatrix_);
	worldMat = XMMatrixTranspose( worldMat );

    d3dContext_->UpdateSubresource( worldCB_, 0, 0, &worldMat, 0, 0 );
    d3dContext_->UpdateSubresource( viewCB_, 0, 0, &viewMatrix_, 0, 0 );
    d3dContext_->UpdateSubresource( projCB_, 0, 0, &projMatrix_, 0, 0 );
	d3dContext_->UpdateSubresource( camPosCB_, 0, 0, &camPosition_, 0, 0 );

    d3dContext_->VSSetConstantBuffers( 0, 1, &worldCB_ );
    d3dContext_->VSSetConstantBuffers( 1, 1, &viewCB_ );
    d3dContext_->VSSetConstantBuffers( 2, 1, &projCB_ );
	d3dContext_->VSSetConstantBuffers( 3, 1, &camPosCB_);

    d3dContext_->Draw( vertexBuffer_->totalVerts_[curVertexBufferIndex], 0 );
	//d3dContext_->DrawIndexed(vertexBuffer_->totalIndex_[curVertexBufferIndex], 0, 0);
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����Update


void ModelsDemo::Render( )
{
	if(visible)
		Update(0.0f);
}	// ����Render()

void ModelsDemo::SetPic( GamePic*  pic )
{
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط�
	gamePic_ = pic;
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetPic


void ModelsDemo::SetVertex( VertexBuffer* vertexBuffer )
{
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط�
	vertexBuffer_ = vertexBuffer;
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetVertex()

void ModelsDemo::SetWorldMatrix( )
{
    XMMATRIX translation = XMMatrixTranslation( position_x, position_y, position_z );
    XMMATRIX rotation = XMMatrixRotationRollPitchYaw( rotation_x, rotation_y, rotation_z );	// ��ת
    XMMATRIX scale = XMMatrixScaling( scale_x, scale_y, scale_z );
	
	XMMATRIX worldM =  rotation * scale * translation;
	Matrix_To_Float4x4( &worldMatrix_, &worldM);
}	// ����GetWorldMatrix()


void ModelsDemo::Translation( float x, float y, float z )
{
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط� 
	XMMATRIX world;
	Float4x4_To_Matrix( &world, &worldMatrix_ );
	world *= XMMatrixTranslation( x, y, z );
	Matrix_To_Float4x4( &worldMatrix_, &world );
	position_x += x;
	position_y += y;
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetPosition()


void ModelsDemo::RotationZ( float rotation )
{
	
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط� 
    XMMATRIX world;
	XMMATRIX r = XMMatrixRotationZ( rotation );	// ��ת
	Float4x4_To_Matrix( &world, &worldMatrix_ );
	world = r * world;
	Matrix_To_Float4x4( &worldMatrix_, &world );
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetRotation()
void ModelsDemo::RotationY( float rotation )
{
	
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط� 
    XMMATRIX world;
	XMMATRIX r = XMMatrixRotationY( rotation );	// ��ת
	Float4x4_To_Matrix( &world, &worldMatrix_ );
	world = r * world;
	Matrix_To_Float4x4( &worldMatrix_, &world );
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetRotation()

void ModelsDemo::RotationX( float rotation )
{
	
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط� 
    XMMATRIX world;
	XMMATRIX r = XMMatrixRotationX( rotation );	// ��ת
	Float4x4_To_Matrix( &world, &worldMatrix_ );
	world = r * world;
	Matrix_To_Float4x4( &worldMatrix_, &world );
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetRotation()

void ModelsDemo::ScaleX( float sX)
{
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط� 
	XMMATRIX world;
	XMMATRIX scale = XMMatrixScaling( sX, 1.0f, 1.0f );
	Float4x4_To_Matrix( &world, &worldMatrix_ );
	world = scale * world;
	Matrix_To_Float4x4( &worldMatrix_, &world );
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetScale()

void ModelsDemo::ScaleY( float sY)
{
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط� 
	XMMATRIX world;
	XMMATRIX scale = XMMatrixScaling( 1.0f, sY, 1.0f );
	Float4x4_To_Matrix( &world, &worldMatrix_ );
	world = scale * world;
	Matrix_To_Float4x4( &worldMatrix_, &world );
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetScale()

void ModelsDemo::ScaleZ( float sZ)
{
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط� 
	XMMATRIX world;
	XMMATRIX scale = XMMatrixScaling( 1.0f, 1.0f, sZ );
	Float4x4_To_Matrix( &world, &worldMatrix_ );
	world = scale * world;
	Matrix_To_Float4x4( &worldMatrix_, &world );
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}	// ����SetScale()

void ModelsDemo::Scale(float sX, float sY, float sZ)
{
	::EnterCriticalSection(&cs);   //����Ҫ�Ĳ��֣�Ҳ���ٽ������󷢻����õĵط� 
	XMMATRIX world;
	XMMATRIX scale = XMMatrixScaling( sX, sY, sZ );
	Float4x4_To_Matrix( &world, &worldMatrix_ );
	world = scale * world;
	Matrix_To_Float4x4( &worldMatrix_, &world );
	::LeaveCriticalSection(&cs);	// �˳��ٽ�������
}