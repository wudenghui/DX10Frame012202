#include"Directx10.h"

CRITICAL_SECTION context_cs;	// 临界区变量（渲染环境）
ID3D10Device*    md3dDevice;
IDXGISwapChain*  mSwapChain;
ID3D10Texture2D* mDepthStencilBuffer;
D3D10_VIEWPORT mViewPort;
ID3D10RenderTargetView* mRenderTargetView;
ID3D10DepthStencilView* mDepthStencilView;
D3D10_DRIVER_TYPE md3dDriverType;
D3DXCOLOR mClearColor;
IDXGIAdapter* mAdapterArray[2];	// 显卡接口数组
UINT numAdapter;		// 显卡数量

D3DXMATRIX mProj_3D;
D3DXMATRIX mProj_2D;
D3DXMATRIX mView;

// 效果
ID3D10Effect* SkyFX	= 0;
ID3D10Effect* BuildShadowMapFX	= 0;

ID3D10Effect* CubeMapFX	= 0;
ID3D10Effect* SpriteFX	= 0;
ID3D10Effect* Sprite3DFX = 0;
ID3D10Effect* ColorVertexFX = 0;

bool enableMultSamp;
unsigned int multSampQuality;

char* DXInfo;


void InitAll()
{
	InitDX();
	InitAllFX();
	InitAllInputLayout();
}
void DestroyAll()
{
	DestroyAllInputLayout();
	DestroyAllFX();
	DestroyDX();
}


void InitDX()
{
	char info[100];
		// 高效函数，最适应硬件
	::InitializeCriticalSection(&context_cs); // 初始化临界区变量
    RECT dimensions;
    GetClientRect( mhMainWnd, &dimensions );
	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;

	unsigned int creationFlags = 0;

    HRESULT result;
    unsigned int driver = 0;

    D3D10_DRIVER_TYPE driverTypes[] =
    {
        D3D10_DRIVER_TYPE_HARDWARE, D3D10_DRIVER_TYPE_WARP,
        D3D10_DRIVER_TYPE_REFERENCE, D3D10_DRIVER_TYPE_SOFTWARE
    };
    unsigned int totalDriverTypes = ARRAYSIZE( driverTypes );

    for( driver = 0; driver < totalDriverTypes; ++driver )
    {
		 result = D3D10CreateDevice(
			0,
			driverTypes[driver],
			0,
			creationFlags, 
			D3D10_SDK_VERSION,
			&md3dDevice );
        if( SUCCEEDED( result ) )
        {
            md3dDriverType = driverTypes[driver];
            break;
        }
	}
	if( FAILED(result))
	{
		MessageBox(0, "d3d10Device创建失败", 0, 0);
		return;
	}

	// 多点采样设置
	enableMultSamp = MULT_SAMP;
	if( enableMultSamp )
	{
		UINT num = NUM_MULT_POINT_SAMP;
		// 先检测硬件是否支持

		HR(md3dDevice->CheckMultisampleQualityLevels(
		SURFACE_FORM, num, &multSampQuality));
		if(multSampQuality == 1)
		{
			//MessageBox(0,"硬件不支持多点采样，取消设置",0,0);
			enableMultSamp = false;

		}
		else
		{
			//MessageBox(0,"硬件支持多点采样",0,0);
			enableMultSamp = true;
		}
	}
	// 是否强制使用Dx10
	if( md3dDriverType == D3D10_DRIVER_TYPE_HARDWARE )
	{
		strcpy(info, "硬件完全支持DX10\n请按 ENTER 键");
	}
	else
	{
		if(md3dDriverType == D3D10_DRIVER_TYPE_WARP)
		{
			strcpy(info, "不完全支持DX10，WRAP模式");
		}
		else if(md3dDriverType == D3D10_DRIVER_TYPE_REFERENCE)
		{
			strcpy(info, "不完全支持DX10，REF模式");
		}
		else
		{
			strcpy(info, "软件层模拟");
		}
	}

	
	// 交换链设置
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = SURFACE_FORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if(enableMultSamp)
	{
		sd.SampleDesc.Count   = NUM_MULT_POINT_SAMP;
		sd.SampleDesc.Quality = multSampQuality-1;
	}
	else
	{
		sd.SampleDesc.Count   = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = mhMainWnd;
	int windowed = WINDOWED;
	if(windowed)
		sd.Windowed = true;	// 全屏控制钮
	else
		sd.Windowed = false;	// 全屏控制钮

	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// 自动适应全屏、窗口切换
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// 最高效的显示模式
	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	      
	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain));	// 创建交换链
	// 使用Factory修改属性
	// 禁用dxgi监听
	dxgiFactory->MakeWindowAssociation( mhMainWnd, DXGI_MWA_NO_WINDOW_CHANGES);
	numAdapter = 0;


	while( !FAILED(dxgiFactory->EnumAdapters( numAdapter, mAdapterArray + numAdapter) ) )
	{
		if(FAILED(mAdapterArray[numAdapter]->CheckInterfaceSupport( __uuidof(ID3D10Device), NULL)))
		{
		//	sprintf(tc, "\n显卡%d不支持DX10", numAdapter );
		//	strcat(info, tc);
		}
		else
		{
		//	sprintf(tc, "\n显卡%d支持DX10", numAdapter );
		//	strcat(info, tc);
		}
		UINT numOutput = 0;
		IDXGIOutput * pOutput;
		while(!FAILED(mAdapterArray[numAdapter]->EnumOutputs( numOutput, &pOutput)))
		{
			
			UINT numModes = 0;
			DXGI_FORMAT format = SURFACE_FORM;
			UINT flags = DXGI_ENUM_MODES_INTERLACED;

			pOutput->GetDisplayModeList(format, flags, &numModes, 0);
			DXGI_MODE_DESC * pModeDescs = new DXGI_MODE_DESC[numModes];
			pOutput->GetDisplayModeList(format, flags, &numModes, pModeDescs);

			for (UINT i = 0; i < numModes; ++i) 
			{
	//			sprintf(tc, "\n模式%d\nWIDTH:%d \n HEIGHT%d \n REFRUSHRATE:NUM%d DENOM %d",
	//				i,
	//				pModeDescs[i].Width,
	//				pModeDescs[i].Height,
	//				pModeDescs[i].RefreshRate.Numerator,
	//				pModeDescs[i].RefreshRate.Denominator
	//				);
	//			strcat(info, tc);
			}

			numOutput++;
		}
	//	sprintf(tc, "\n显示器有%d个", numOutput);
	//	strcat(info, tc);

		numAdapter++;
	}
//	sprintf(tc, "\n一共有%d块显卡", numAdapter );
//	strcat(info, tc);
	DXInfo = new char[strlen(info)];
	strcpy(DXInfo, info);

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();


	// 根据窗口大小设置
    OnResize();
}

void DestroyDX()
{
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(md3dDevice);
}

void ResetOMTargetsAndViewport( )
{
	md3dDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	md3dDevice->RSSetViewports(1, &mViewPort);
}

void OnResize()
{
	RECT mClientSize;
	::GetWindowRect(mhMainWnd,&mClientSize);
	mClientWidth = mClientSize.right - mClientSize.left;
	mClientHeight = mClientSize.bottom - mClientSize.top;

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);


	// Resize the swap chain and recreate the render target view.

	HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, SURFACE_FORM, 0));
	ID3D10Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);


	// Create the depth/stencil buffer and view.

	D3D10_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width     = mClientWidth;
	depthStencilDesc.Height    = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage          = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	HR(md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline.

	md3dDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	

	// Set the viewport transform.
	mViewPort.TopLeftX =  0;
	mViewPort.TopLeftY = 0;
	mViewPort.Width    = mClientWidth;
	mViewPort.Height   = mClientHeight;
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;

	md3dDevice->RSSetViewports(1, &mViewPort);

	// 3D投影矩阵
	///////////////////////
	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj_3D, SIGHT_EREA, aspect, 0.5f, 1000.0f);
	// 2D投影矩阵
	mProj_2D = D3DXMATRIX(
		2.0f/mClientWidth, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f/mClientHeight, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

// 效果
ID3D10Effect* CreateFX(char* fileName)
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
  
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	ID3D10Effect* fx = 0;
	hr = D3DX10CreateEffectFromFile(fileName, 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &fx, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, fileName, true);
	}

	return fx;
}

void InitAllFX()
{
	SkyFX		= CreateFX( EFFECT_SKY_NAME );
	CubeMapFX	= CreateFX( EFFECT_CUBE_MAP_NAME );

	BuildShadowMapFX = CreateFX("FX/buildshadowmap.fx");
	SpriteFX  = CreateFX("FX/sprite.fx");
	
	ColorVertexFX = CreateFX("FX/ColorVertex.fx");
}

void DestroyAllFX()
{
	ReleaseCOM(SkyFX);
	ReleaseCOM(CubeMapFX);
	ReleaseCOM(BuildShadowMapFX);
	ReleaseCOM(SpriteFX );

	ReleaseCOM(ColorVertexFX );
}

// InputLayout输入布局
ID3D10InputLayout* Pos = 0;
ID3D10InputLayout* PosColor = 0;
ID3D10InputLayout* PosTex = 0;
ID3D10InputLayout* PosTanNormalTex = 0;

void InitAllInputLayout()
{
	D3D10_PASS_DESC PassDesc;

	//
	// SkyVertex
	//
 	D3D10_INPUT_ELEMENT_DESC skyVertexDesc[] =
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	SkyFX->GetTechniqueByName("SkyTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(skyVertexDesc, 1, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &Pos));

	// Position-Color

	D3D10_INPUT_ELEMENT_DESC posColorVertexDesc[] =
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	ColorVertexFX->GetTechniqueByName("ColorVertexTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(posColorVertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &PosColor));

	//
	// Position-texture vertex
	//
	
 	D3D10_INPUT_ELEMENT_DESC posTexVertexDesc[] =
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	SpriteFX->GetTechniqueByName("SpriteTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(posTexVertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &PosTex));
		
	// 
	// Position-tangent-normal-texture vertex
	//
	D3D10_INPUT_ELEMENT_DESC tanVertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

    CubeMapFX->GetTechniqueByName("NormalMapTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(tanVertexDesc, 4, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &PosTanNormalTex));
}

void DestroyAllInputLayout()
{
	ReleaseCOM(Pos);
	ReleaseCOM(PosTanNormalTex);
}