#ifndef TEXTUREMGR_H
#define TEXTUREMGR_H

#include"Directx10.h"


#define MAX_TEX	1024	// ���������ʼ��Ŀ
#define INC_TEX	512		// ����������Ŀ
#define TEX_PATH	"PIC/"	// ͼƬ·��

// ��֧�ֵ�ͼƬ��ʽ����
extern char* TEX_FORM_ARRAY[4];



class TextureMgr
{
public:
	friend TextureMgr& GetTextureMgr();
 
	void init();

	// for debug
	bool dumpInfo()const;

	// ���ļ����м�������ͼƬ
	void CreateTexRVFromDir(char* dirName);
	void CreateCubeMapFromDir(char* dirName);
	void CreateTexArrayFromFile(char* dirName);

	// �������غ���
	ID3D10ShaderResourceView* GetTexRVByIndex(size_t index);
	ID3D10ShaderResourceView* GetTexRVByName(const char* name);
	ID3D10ShaderResourceView* GetRandomTex();
	ID3D10ShaderResourceView* CreateTex(char* fileName);
	ID3D10ShaderResourceView* CreateTexArray(
		char* arrayName,  // �½��ɵ�����������
		char** fileNames,
		size_t arraySize);	// �ļ�����

	// .dds files can store cube textures in one file
	ID3D10ShaderResourceView* CreateCubeTex(char* fileName);


private:
	TextureMgr();
	TextureMgr(const TextureMgr& rhs);
	TextureMgr& operator=(const TextureMgr& rhs);
	~TextureMgr();

	void BuildRandomTex();


private:
	char** mTexMsg;	// ������������

	UINT numTex;
	UINT maxTex;


	ID3D10ShaderResourceView** mTextureRVs;

	ID3D10ShaderResourceView* mRandomTexRV;
};

TextureMgr& GetTextureMgr();

#endif // TEXTUREMGR_H