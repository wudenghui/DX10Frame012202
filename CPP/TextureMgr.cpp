#include "TextureMgr.h"

using namespace std;

char* TEX_FORM_ARRAY[4] = { ".dds", ".jpg", ".png", ".gif"};

TextureMgr& GetTextureMgr()
{
	static TextureMgr tm;

	return tm;
}

TextureMgr::TextureMgr()
: mRandomTexRV(0)
{
	maxTex = MAX_TEX;
	numTex = 0;

	mTexMsg = (char**)malloc(sizeof(char*) * maxTex);
	if(mTexMsg == NULL)
		return ;
	for(size_t i = 0; i < maxTex; ++i)
	{
		// 初始化数组内容
		mTexMsg[i] = NULL;
	}

	mTextureRVs = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*maxTex);
	if(mTextureRVs == NULL)
		return ;
	for(size_t i = 0; i < maxTex; ++i)
	{
		// 初始化数组内容
		mTextureRVs[i] = NULL;
	}
	init();
}

TextureMgr::~TextureMgr()
{
	
	for(UINT i = 0; i < numTex; ++i)
	{
		if(mTexMsg[i])
			free( mTexMsg[i] );
		ReleaseCOM(mTextureRVs[i]);
	}
	
	free(mTexMsg);
	free(mTextureRVs);
	ReleaseCOM(mRandomTexRV);
	
}

void TextureMgr::init()
{
	BuildRandomTex();
}

bool TextureMgr::dumpInfo()const
{
	FILE* hist;
	do
	{
		hist = fopen("Textures.txt", "w+");
	}
	while(hist == NULL);

	fseek(hist, 0, SEEK_SET);

	char newContent[1024];
	sprintf(newContent, "Num Textures = %d", numTex, numTex);
	if(!fwrite(newContent, sizeof(char), strlen(newContent), hist))
		return false;
	fwrite( "\n", 1, 1, hist);

	for(size_t i = 0; i < numTex; ++i)
	{
		strcpy(newContent, mTexMsg[i]);
		if(!fwrite(newContent, sizeof(char), strlen(newContent), hist))
			return false;
		fwrite( "\n", 1, 1, hist);
	}
	fclose(hist);
	return true;
}

ID3D10ShaderResourceView* TextureMgr::GetRandomTex()
{
	return mRandomTexRV;
}

ID3D10ShaderResourceView* TextureMgr::GetTexRVByIndex(size_t index)
{
	if(index >= numTex)
	{
		MessageBox(0,"参数错误",0,0);
		return NULL;
	}
	return mTextureRVs[index];
}

ID3D10ShaderResourceView* TextureMgr::GetTexRVByName(const char* name)
{
	for(size_t i = 0; i < numTex; ++i)
	if( strcmp( mTexMsg[i], name ) == 0)
	{
		return mTextureRVs[i];
	}
	char* a = (char*)malloc(strlen(name) + 20);
	sprintf(a, "无此图片：%s", name);
	MessageBox(0, a,0,0);
	free(a);
	return NULL;
}

ID3D10ShaderResourceView* TextureMgr::CreateTex(char* fileName)
{
	// Has this texture already been created?
	for(size_t i = 0; i < numTex; ++i)
	if( strcmp( mTexMsg[i], fileName ) == 0)
	{
		return mTextureRVs[i];
	}
	
	// 如果数组已满，扩容数组，重分配内存
	if(maxTex == numTex)
	{
		maxTex += INC_TEX;

		char** tMsg = (char**)malloc(sizeof(char*) * maxTex);
		if(tMsg == NULL)
			return NULL;
		memcpy(tMsg, mTexMsg, sizeof(char*)*numTex);
		free( mTexMsg);
		mTexMsg = tMsg;

		ID3D10ShaderResourceView** tTex = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*maxTex);
		if(tTex == NULL)
			return NULL;
		memcpy(tTex, mTextureRVs, sizeof(ID3D10ShaderResourceView*)*numTex);
		free( mTextureRVs);
		mTextureRVs = tTex;
	}
	
	// create new
	ID3D10ShaderResourceView* rv = 0;

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, fileName, 0, 0, &rv, 0 ));
	if(rv == NULL)
	{
		MessageBox(0,fileName,"加载图片失败CreateTex",0);
		return NULL;
	}
	
	

	
	int len = strlen(fileName) + 1;
	char* name = (char*)malloc(len);
	strcpy(name, fileName);

	mTexMsg[numTex] = name;
	
	
	mTextureRVs[numTex] = rv;

	numTex++;
	return rv;
	
	return NULL;
}

ID3D10ShaderResourceView* TextureMgr::CreateTexArray(char* arrayName, char** fileNames, size_t arraySize)
{
	// Has this texture already been created?
	for(size_t i = 0; i < numTex; ++i)
		if( strcmp( mTexMsg[i], arrayName ) == 0)
			return mTextureRVs[i];

	//
	// Load the texture elements individually from file.  These textures
	// won't be used by the GPU (0 bind flags), they are just used to 
	// load the image data from file.  We use the STAGING usage so the
	// CPU can read the resource.
	//
	ID3D10Texture2D** srcTex = (ID3D10Texture2D**)malloc(sizeof(ID3D10Texture2D*)*arraySize);

	for(UINT i = 0; i < arraySize; ++i)
	{
		D3DX10_IMAGE_LOAD_INFO loadInfo;

        loadInfo.Width  = D3DX10_FROM_FILE;
        loadInfo.Height = D3DX10_FROM_FILE;
        loadInfo.Depth  = D3DX10_FROM_FILE;
        loadInfo.FirstMipLevel = 0;
        loadInfo.MipLevels = D3DX10_FROM_FILE;
        loadInfo.Usage = D3D10_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
        loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        loadInfo.Filter = D3DX10_FILTER_NONE;
        loadInfo.MipFilter = D3DX10_FILTER_NONE;
		loadInfo.pSrcInfo  = 0;


        HR(D3DX10CreateTextureFromFile(md3dDevice, fileNames[i], 
			&loadInfo, 0, (ID3D10Resource**)&srcTex[i], 0));
		if(srcTex[i] == NULL)
		{
			MessageBox(0, fileNames[i], 0, 0);
			return NULL;
		}
	}

	//
	// Create the texture array.  Each element in the texture 
	// array has the same format/dimensions.
	//

	D3D10_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D10_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width              = texElementDesc.Width;
	texArrayDesc.Height             = texElementDesc.Height;
	texArrayDesc.MipLevels          = texElementDesc.MipLevels;
	texArrayDesc.ArraySize          = arraySize;	// 数组大小，表明是纹理数组
	texArrayDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count   = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage              = D3D10_USAGE_DEFAULT;
	texArrayDesc.BindFlags          = D3D10_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags     = 0;
	texArrayDesc.MiscFlags          = 0;

	ID3D10Texture2D* texArray = 0;
	HR(md3dDevice->CreateTexture2D( &texArrayDesc, 0, &texArray));

	//
	// Copy individual texture elements into texture array.
	//

	// for each texture element...
	for(UINT i = 0; i < arraySize; ++i)
	{
		// for each mipmap level...
		for(UINT j = 0; j < texElementDesc.MipLevels; ++j)
		{
			D3D10_MAPPED_TEXTURE2D mappedTex2D;
			srcTex[i]->Map(j, D3D10_MAP_READ, 0, &mappedTex2D);
            
			// 更新纹理数组内容
            md3dDevice->UpdateSubresource(texArray,
				D3D10CalcSubresource(j, i, texElementDesc.MipLevels),
                0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);

            srcTex[i]->Unmap(j);
		}
	}	

	//
	// Create a resource view to the texture array.
	//
	
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = arraySize;

	ID3D10ShaderResourceView* texArrayRV = 0;
	HR(md3dDevice->CreateShaderResourceView(texArray, &viewDesc, &texArrayRV));

	//
	// Cleanup--we only need the resource view.
	//

	ReleaseCOM(texArray);

	for(UINT i = 0; i < arraySize; ++i)
		ReleaseCOM(srcTex[i]); 

	///////////////////////////////////////////////////////////////////////
	// 如果数组已满，扩容数组，重分配内存
	if(maxTex == numTex)
	{
		maxTex += INC_TEX;

		char** tMsg = (char**)malloc(sizeof(char*) * maxTex);
		if(tMsg == NULL)
			return NULL;
		memcpy(tMsg, mTexMsg, sizeof(char*)*numTex);
		free( mTexMsg);
		mTexMsg = tMsg;

		ID3D10ShaderResourceView** tTex = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*maxTex);
		if(tTex == NULL)
			return NULL;
		memcpy(tTex, mTextureRVs, sizeof(ID3D10ShaderResourceView*)*numTex);
		free( mTextureRVs);
		mTextureRVs = tTex;
	}

	// create new
	int len = strlen(arrayName) + 1;
	char* tMsg = (char*)malloc(sizeof(char)*len);
	strcpy(tMsg, arrayName);
	mTexMsg[numTex] = tMsg;

	mTextureRVs[numTex] = texArrayRV;

	numTex++;

	return texArrayRV;
}

ID3D10ShaderResourceView* TextureMgr::CreateCubeTex(char* fileName)
{
	// Has this texture already been created?
	for(size_t i = 0; i < numTex; ++i)
		if( strcmp( mTexMsg[i], fileName ) == 0)
			return mTextureRVs[i];

	// 如果数组已满，扩容数组，重分配内存
	if(maxTex == numTex)
	{
		maxTex += INC_TEX;

		char** tMsg = (char**)malloc(sizeof(char*) * maxTex);
		if(tMsg == NULL)
			return NULL;
		memcpy(tMsg, mTexMsg, sizeof(char*)*numTex);
		free( mTexMsg);
		mTexMsg = tMsg;

		ID3D10ShaderResourceView** tTex = (ID3D10ShaderResourceView**)malloc(sizeof(ID3D10ShaderResourceView*)*maxTex);
		if(tTex == NULL)
			return NULL;
		memcpy(tTex, mTextureRVs, sizeof(ID3D10ShaderResourceView*)*numTex);
		free( mTextureRVs);
		mTextureRVs = tTex;
	}

	// If not, create it.
	D3DX10_IMAGE_LOAD_INFO loadInfo;
    loadInfo.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;	// 说明是CUBE型纹理

	ID3D10Texture2D* tex = 0;

	HR(D3DX10CreateTextureFromFile(md3dDevice, fileName, 
		&loadInfo, 0, (ID3D10Resource**)&tex, 0) );
	if(tex == NULL)
	{
		MessageBox(0,fileName,0,0);
		return NULL;
	}

    D3D10_TEXTURE2D_DESC texDesc;
	tex->GetDesc(&texDesc);

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.TextureCube.MipLevels = texDesc.MipLevels;
    viewDesc.TextureCube.MostDetailedMip = 0;
    
	ID3D10ShaderResourceView* rv = 0;
	HR(md3dDevice->CreateShaderResourceView(tex, &viewDesc, &rv));
   
	ReleaseCOM(tex);

	// create new
	int len = strlen(fileName) + 1;
	char* tMsg = (char*)malloc(sizeof(char)*len);
	strcpy(tMsg, fileName);
	mTexMsg[numTex] = tMsg;

	mTextureRVs[numTex] = rv;

	numTex++;

	return rv;
}

void TextureMgr::BuildRandomTex()
{
	// 
	// Create the random data.
	//
	D3DXVECTOR4 randomValues[1024];

	for(int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = RandF(-1.0f, 1.0f);
		randomValues[i].y = RandF(-1.0f, 1.0f);
		randomValues[i].z = RandF(-1.0f, 1.0f);
		randomValues[i].w = RandF(-1.0f, 1.0f);
	}

    D3D10_SUBRESOURCE_DATA initData;
    initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024*sizeof(D3DXVECTOR4);
    initData.SysMemSlicePitch = 1024*sizeof(D3DXVECTOR4);
	//
	// Create the texture.
	//
    D3D10_TEXTURE1D_DESC texDesc;
    texDesc.Width = 1024;
    texDesc.MipLevels = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.Usage = D3D10_USAGE_IMMUTABLE;
    texDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.ArraySize = 1;

	ID3D10Texture1D* randomTex = 0;
    HR(md3dDevice->CreateTexture1D(&texDesc, &initData, &randomTex));
	//
	// Create the resource view.
	//
    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
    viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;
	
    HR(md3dDevice->CreateShaderResourceView(randomTex, &viewDesc, &mRandomTexRV));

	ReleaseCOM(randomTex);
}

void TextureMgr::CreateTexRVFromDir(char* dirName)
{
	char fName[100];

	struct _finddata_t	fileInfo;	// 文本信息结构体
	long hFile;

	

	int num = sizeof(TEX_FORM_ARRAY) / sizeof(char*);

	for( int i=0; i<num; i++ )
	{
		sprintf(fName, "%s/*%s", dirName, TEX_FORM_ARRAY[i]);	// 未做失败判断
		if(fName[99] != '\0')
			fName[99] = '\0';

		hFile = _findfirst( fName, &fileInfo);
		if(hFile == -1)
		{
			continue;
		}

		sprintf(fName, "%s/%s", dirName, fileInfo.name);	// 未做失败判断
		if(fName[99] != '\0')
			fName[99] = '\0';
		CreateTex(fName);

	

		while(_findnext(hFile, &fileInfo) != -1)
		{
			sprintf(fName, "%s/%s", dirName, fileInfo.name);	// 未做失败判断
			if(fName[99] != '\0')
			fName[99] = '\0';
			CreateTex(fName);
		}
		
		_findclose(hFile);
	}
}

void TextureMgr::CreateCubeMapFromDir(char* dirName)
{
	char fName[100] = {};

	struct _finddata_t	fileInfo;	// 文本信息结构体
	long hFile;

	int num = sizeof(TEX_FORM_ARRAY) / sizeof(char*);

	for( int i=0; i<num; i++ )
	{
		sprintf(fName, "%s/*%s", dirName, TEX_FORM_ARRAY[i]);	// 未做失败判断

		hFile = _findfirst( fName, &fileInfo);
		if(hFile == -1)
		{
			continue;
		}

		sprintf(fName, "%s/%s", dirName, fileInfo.name);	// 未做失败判断
		CreateCubeTex(fName);

		while(_findnext(hFile, &fileInfo) != -1)
		{
			sprintf(fName, "%s/%s", dirName, fileInfo.name);	// 未做失败判断
			if(CreateCubeTex(fName) == NULL)
				MessageBox(0,"加载图片失败",fileInfo.name,0);
		}

		_findclose(hFile);
	}
}

void TextureMgr::CreateTexArrayFromFile(char* DirName)
{
	
}