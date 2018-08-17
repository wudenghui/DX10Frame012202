/*
    3D模型demo
*/


#ifndef _MODELS_DEMO_H_
#define _MODELS_DEMO_H_

#include"ArcCamera.h"
#include"DemoView.h"

class ModelsDemo : public DemoView
{
    public:
        ModelsDemo( );
        virtual ~ModelsDemo( );

        bool LoadContent( SourceLoader* source,
			float posX, float posY, float posZ,
			float rotX, float rotY, float rotZ,
			float sX, float sY, float sZ);
        void UnloadContent( );

        void Update( float dt );
        void Render( );

		////////////////////////////
		void SetWorldMatrix( );
		
		void SetPic( GamePic*  pic );
		void SetVertex( VertexBuffer* vertexBuffer );

		void Translation( float x, float y, float z );
		void RotationX( float rX );
		void RotationY( float rY );
        void RotationZ( float rZ );
        void ScaleX( float sX );
		void ScaleY( float sY );
		void ScaleZ( float sZ );
		void Scale(float sX, float sY, float sZ);


    protected:
        ID3D11Buffer* viewCB_;
        ID3D11Buffer* projCB_;
        ID3D11Buffer* worldCB_;
		ID3D11Buffer* camPosCB_;	// 相机位置，计算光强时使用

};

/*
class ModelsDemo
{
    public:
        ModelsDemo( );
        virtual ~ModelsDemo( );

        bool LoadContent( SourceLoader* source,
			float posX, float posY, float posZ,
			float rotX, float rotY, float rotZ,
			float sX, float sY, float sZ);
        void UnloadContent( );

        void Update( float dt );
        void Render( );

		////////////////////////////
		void SetWorldMatrix( );
		void Show();
		void Hide();
		bool GetVisible();
		void SetPic( GamePic*  pic );
		void SetVertex( VertexBuffer* vertexBuffer );
		void SetPic( int index );	// 尽量使用资源管理器定义的资源，避免释放资源造成的问题
		void SetVertex( int index );

		void Translation( float x, float y, float z );
		void RotationX( float rX );
		void RotationY( float rY );
        void RotationZ( float rZ );
        void ScaleX( float sX );
		void ScaleY( float sY );
		void ScaleZ( float sZ );
		void Scale(float sX, float sY, float sZ);


    protected:
		CRITICAL_SECTION cs;	// 临界区变量
		SourceLoader* source_;


		VertexBuffer* vertexBuffer_;
		int curVertexBufferIndex;
		unsigned int stride;	// 可以优化
		unsigned int offset;

        GamePic* gamePic_;
		int curPicIndex;

        ID3D11Buffer* viewCB_;
        ID3D11Buffer* projCB_;
        ID3D11Buffer* worldCB_;
		ID3D11Buffer* camPosCB_;	// 相机位置，计算光强时使用
        XMFLOAT4X4 projMatrix_;
		XMFLOAT4X4 worldMatrix_;

};

*/

#endif