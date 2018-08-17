#include"Scene.h"

Scene* curScene = 0;

Scene::Scene()
{
	enableShadow = false;

	maxTextLen = 1024;
	textContent = (char*)malloc(sizeof(char)*maxTextLen);
	SetTextStr("");
	textLen = 0;

	fontColor = CLIENT_FONT_COLOR;
	textErea.left = 5;
	textErea.top = 5;
	textErea.right = 0;
	textErea.bottom = 0;

	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 24;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    strcpy(fontDesc.FaceName, "Times New Roman");
	

	D3DX10CreateFontIndirect(md3dDevice, &fontDesc, &mFont);

	// RS
	D3D10_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));
	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_NONE;

	HR(md3dDevice->CreateRasterizerState(&rsDesc, &mRS));

	mSky.init(0, 500.0f);
	
	// nextScene = NULL;
}

Scene::~Scene()
{
	
	if(textContent)
		free(textContent), textContent = NULL;
	ReleaseCOM(mFont);
	ReleaseCOM(mRS);
	
}

void Scene::Init(ID3D10Effect* fx,					// Ч��
		char* techName, char* shadowTechName)
{
	// ���̶���Ч���ļ�
	mFX = fx;
	mTech				= mFX->GetTechniqueByName("NormalMapTech");
	mShadowTech			= mFX->GetTechniqueByName("ShadowTech");
	mfxEyePosVar		= mFX->GetVariableByName("gEyePosW");
	mfxCubeMapEnabledVar= mFX->GetVariableByName("gReflectCubeMapEnabled")->AsScalar();
	mfxCubeMapVar		= mFX->GetVariableByName("gCubeMap")->AsShaderResource();


	// �̶���Ч���ļ�
	mBuildShadowMapTech            = BuildShadowMapFX->GetTechniqueByName("BuildShadowMapTech");
	mfxBuildShadowMapLightWVPVar   = BuildShadowMapFX->GetVariableByName("gLightWVP")->AsMatrix();
	mfxBuildShadowMapDiffuseMapVar = BuildShadowMapFX->GetVariableByName("gDiffuseMap")->AsShaderResource();

	// ��Ӱ���
	mfxLightWVPVar      = mFX->GetVariableByName("gLightWVP")->AsMatrix();
	mfxShadowMapVar		= mFX->GetVariableByName("gShadowMap")->AsShaderResource();

	mShadowMap.init(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, true, SURFACE_FORM);
 
	D3DXMatrixOrthoLH(&mLightVolume, LIGHT_VIEW_WIDTH, LIGHT_VIEW_HEIGHT, 1.0f, 100.0f);
	ID3D10ShaderResourceView* map = mShadowMap.colorMap();//depthMap();
	if(map == NULL)
		MessageBox(0,"��ShadowLight",0,0);
}

void Scene::ActionUpdate(float dt)
{	
	pActionList.ActionUpdate(dt);
	pNoCullList.ActionUpdate(dt);		// �ޱ�����������
	pAlphaObjectList3D.ActionUpdate(dt);	// 3D͸����������
	pObjectList3D.ActionUpdate(dt);	// 3D��������
	pReflectMapList3D.ActionUpdate(dt);	// ���价��
	pUI.ActionUpdate(dt);
	
	if(Action)
		Action(dt);
}
void Scene::DrawContent()
{
	
	static ID3D10EffectPass* pass;

	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactor, 0xffffffff);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// �۲��λ��
	mfxEyePosVar->SetRawValue(&GetCamera().position(), 0, sizeof(D3DXVECTOR3));
	// ������ͼ
	mfxCubeMapVar->SetResource(mSky.GetCubeMap());
	
	
	if(enableShadow)
	{
		// ������
		D3DXMatrixLookAtLH(&mLightView, &mShadowLight->pos,
			&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		pass = mShadowTech->GetPassByIndex(0);

		static D3DXMATRIX lightVP;
		lightVP = mLightView*mLightVolume;

		// ������Ӱ*************************************************
		mShadowMap.begin();
		pass = mBuildShadowMapTech->GetPassByIndex(0);

		// Reflect�б��������������价��
		md3dDevice->RSSetState(0);
		mfxCubeMapEnabledVar->SetBool(true);

		pReflectMapList3D.DrawToShadowMap( mfxBuildShadowMapLightWVPVar, (mLightView*mLightVolume), mfxBuildShadowMapDiffuseMapVar, pass );

		// ��ͨ�����������������������价��
		mfxCubeMapEnabledVar->SetBool(false);
		pObjectList3D.DrawToShadowMap( mfxBuildShadowMapLightWVPVar, (mLightView*mLightVolume), mfxBuildShadowMapDiffuseMapVar, pass );

		// �ޱ�������������
		md3dDevice->RSSetState(mRS);
		pNoCullList.DrawToShadowMap( mfxBuildShadowMapLightWVPVar, (mLightView*mLightVolume), mfxBuildShadowMapDiffuseMapVar, pass );

		// ֧��͸���ȵ�����ͬ���ޱ�������������Ӱ
		pAlphaObjectList3D.DrawToShadowMap( mfxBuildShadowMapLightWVPVar, (mLightView*mLightVolume), mfxBuildShadowMapDiffuseMapVar, pass );	// 3D͸����������

		mShadowMap.end();

		// ��������**************************************************
		ResetOMTargetsAndViewport();
		pass = mShadowTech->GetPassByIndex(0);

		mfxShadowMapVar->SetResource(mShadowMap.depthMap());

		mfxShadowLightVar->SetRawValue(&mShadowLight->pos, 0, sizeof(Light));

		md3dDevice->IASetInputLayout(PosTanNormalTex);
		// Reflect�б��������������价��
		md3dDevice->RSSetState(0);
		mfxCubeMapEnabledVar->SetBool(true);
		pReflectMapList3D.DrawContentShadow( mfxLightWVPVar, lightVP);

		// ��ͨ�����������������������价��
		mfxCubeMapEnabledVar->SetBool(false);
		pObjectList3D.DrawContentShadow(mfxLightWVPVar, lightVP);	// 3D��������

		// �ޱ�������������
		md3dDevice->RSSetState(mRS);
		pNoCullList.DrawContentShadow( mfxLightWVPVar, lightVP);

		// ֧��͸���ȵ�����ͬ���ޱ�������������Ӱ
		pAlphaObjectList3D.DrawContentShadow( mfxLightWVPVar, lightVP);	// 3D͸����������
	}
	else
	{
		// ������Ӱ
		md3dDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

		md3dDevice->IASetInputLayout(PosTanNormalTex);

		pass = mTech->GetPassByIndex(0);
		// Reflect�б��������������价��
		md3dDevice->RSSetState(0);
		mfxCubeMapEnabledVar->SetBool(true);
		pReflectMapList3D.DrawContent();

		// ��ͨ�����������������������价��
		mfxCubeMapEnabledVar->SetBool(false);
		pObjectList3D.DrawContent();	// 3D��������

		// �ޱ�������������
		md3dDevice->RSSetState(mRS);
		pNoCullList.DrawContent();
		
		// ֧��͸���ȵ�����ͬ���ޱ�������������Ӱ
		pAlphaObjectList3D.DrawContent();	// 3D͸����������
	}
	// ������ͼ�Ļ���
	
	mSky.draw();
	
	// UI֧��͸���ȣ��뱳�������޹�
	md3dDevice->OMSetRenderTargets(1, &mRenderTargetView, 0);
	md3dDevice->IASetInputLayout(PosTex);
	pUI.DrawContent();
	
	mFont->DrawText(0, textContent, -1, &textErea, DT_NOCLIP, fontColor);
	
}

void Scene::OpenShadow(LightObj* light)
{
	light->OpenShadow();
	mShadowLight = light->GetContent();
	mfxShadowLightVar = light->GetMfxLightVar();
	if(mShadowLight == NULL)
	{
		MessageBox(0,"��Ӱ�ƹ�Ϊ��","Scene����",0);
		enableShadow = false;
		return ;
	}

	enableShadow = true;
}

void Scene::SetTextStr(char* text)
{
	// ��ֹ���
	textLen = strlen(text);
	if(textLen<maxTextLen-1)
		strcpy( textContent, text);
	else
	{
		memcpy(text, textContent, maxTextLen-1);
		textContent[maxTextLen-1] = '\0';
	}
}	// ����DrawText()
