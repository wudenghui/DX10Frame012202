#ifndef TEXTUREMGR_H
#define TEXTUREMGR_H

#include"Directx10.h"


#define MAX_TEX	1024	// 纹理数组初始数目
#define INC_TEX	512		// 扩增数组数目
#define TEX_PATH	"PIC/"	// 图片路径

// 所支持的图片格式数组
extern char* TEX_FORM_ARRAY[4];



class TextureMgr
{
public:
	friend TextureMgr& GetTextureMgr();
 
	void init();

	// for debug
	bool dumpInfo()const;

	// 从文件夹中加载所有图片
	void CreateTexRVFromDir(char* dirName);
	void CreateCubeMapFromDir(char* dirName);
	void CreateTexArrayFromFile(char* dirName);

	// 单个加载函数
	ID3D10ShaderResourceView* GetTexRVByIndex(size_t index);
	ID3D10ShaderResourceView* GetTexRVByName(const char* name);
	ID3D10ShaderResourceView* GetRandomTex();
	ID3D10ShaderResourceView* CreateTex(char* fileName);
	ID3D10ShaderResourceView* CreateTexArray(
		char* arrayName,  // 新建成的纹理数组名
		char** fileNames,
		size_t arraySize);	// 文件名称

	// .dds files can store cube textures in one file
	ID3D10ShaderResourceView* CreateCubeTex(char* fileName);


private:
	TextureMgr();
	TextureMgr(const TextureMgr& rhs);
	TextureMgr& operator=(const TextureMgr& rhs);
	~TextureMgr();

	void BuildRandomTex();


private:
	char** mTexMsg;	// 纹理名称数组

	UINT numTex;
	UINT maxTex;


	ID3D10ShaderResourceView** mTextureRVs;

	ID3D10ShaderResourceView* mRandomTexRV;
};

TextureMgr& GetTextureMgr();

#endif // TEXTUREMGR_H