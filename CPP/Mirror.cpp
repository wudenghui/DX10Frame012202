/*

#include"Mirror.h"


Mirror::Mirror()
: mTech(0), mfxWVPVar(0), mfxWorldVar(0), mfxEyePosVar(0),
  mfxLightVar(0), mfxDiffuseMapVar(0), mfxSpecMapVar(0), mfxTexMtxVar(0),
  mVertexLayout(0), mSpecMapRV(0), mEyePos(0.0f, 0.0f, 0.0f), 
  mRadius(15.0f), mTheta(-0.3f), mPhi(PI*0.4f)
{
	
	D3DXMatrixTranslation(&mCrateWorld, 0.0f, 1.0f, -4.0f);
	D3DXMatrixIdentity(&mRoomWorld);
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mWVP); 
	
	D3DXMatrixIdentity(&mIdentityTexMtx);

	////////////////////
	buildFX();

	buildVertexLayouts();
	
	mCrateMesh.init(1.0f, 1.0f, 1.0f);

	buildRoomGeometry();

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		"PIC/WoodCrate01.dds", 0, 0, &mCrateDiffuseMapRV, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		"PIC/checkboard.dds", 0, 0, &mFloorDiffuseMapRV, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		"PIC/brick01.dds", 0, 0, &mWallDiffuseMapRV, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		"PIC/ice.dds", 0, 0, &mMirrorDiffuseMapRV, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		"PIC/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));


	mParallelLight.dir      = D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
	mParallelLight.ambient  = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	mParallelLight.diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mParallelLight.specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mParallelLight.alive = true;
	mParallelLight.type = PARALLEL;

	D3D10_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));
	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_BACK;
	rsDesc.FrontCounterClockwise = true;

	HR(md3dDevice->CreateRasterizerState(&rsDesc, &mCullCWRS));

	D3D10_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable      = true;
	dsDesc.DepthWriteMask   = D3D10_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc        = D3D10_COMPARISON_LESS; 
    dsDesc.StencilEnable    = true;
    dsDesc.StencilReadMask  = 0xff;
    dsDesc.StencilWriteMask = 0xff;

    dsDesc.FrontFace.StencilFailOp      = D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp      = D3D10_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc        = D3D10_COMPARISON_ALWAYS;

	// We are not rendering backfacing polygons, so these settings do not matter.
    dsDesc.BackFace.StencilFailOp       = D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp  = D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp       = D3D10_STENCIL_OP_REPLACE;
	dsDesc.BackFace.StencilFunc         = D3D10_COMPARISON_ALWAYS;

	HR(md3dDevice->CreateDepthStencilState(&dsDesc, &mDrawMirrorDSS));

	dsDesc.DepthEnable      = true;
	dsDesc.DepthWriteMask   = D3D10_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc        = D3D10_COMPARISON_ALWAYS; 
	dsDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc   = D3D10_COMPARISON_EQUAL;

	HR(md3dDevice->CreateDepthStencilState(&dsDesc, &mDrawReflectionDSS));

	D3D10_BLEND_DESC blendDesc = {0};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.BlendEnable[0] = true;
	blendDesc.SrcBlend       = D3D10_BLEND_BLEND_FACTOR;
	blendDesc.DestBlend      = D3D10_BLEND_INV_BLEND_FACTOR;
	blendDesc.BlendOp        = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha  = D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOpAlpha   = D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HR(md3dDevice->CreateBlendState(&blendDesc, &mDrawReflectionBS));
}

Mirror::~Mirror()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mVertexLayout);

	ReleaseCOM(mCullCWRS);
	ReleaseCOM(mDrawReflectionBS);
	ReleaseCOM(mDrawMirrorDSS);
	ReleaseCOM(mDrawReflectionDSS);

	ReleaseCOM(mRoomVB);
	ReleaseCOM(mCrateDiffuseMapRV);
	ReleaseCOM(mFloorDiffuseMapRV);
	ReleaseCOM(mWallDiffuseMapRV);
	ReleaseCOM(mMirrorDiffuseMapRV);
	ReleaseCOM(mSpecMapRV);
}


void Mirror::ActionUpdate(float dt)
{

	// Update angles based on input to orbit camera around scene.
	if(GetAsyncKeyState('A') & 0x8000)	mTheta -= 2.0f*dt;
	if(GetAsyncKeyState('D') & 0x8000)	mTheta += 2.0f*dt;
	if(GetAsyncKeyState('W') & 0x8000)	mPhi -= 2.0f*dt;
	if(GetAsyncKeyState('S') & 0x8000)	mPhi += 2.0f*dt;
	if(GetAsyncKeyState('Z') & 0x8000)	mRadius -= 15.0f*dt;
	if(GetAsyncKeyState('X') & 0x8000)	mRadius += 15.0f*dt;

	// Update translation factors to slide the crate in the scene.
	if(GetAsyncKeyState('F') & 0x8000)	mCrateWorld(3,0) -= 2.0f*dt;
	if(GetAsyncKeyState('G') & 0x8000)	mCrateWorld(3,0) += 2.0f*dt;
	if(GetAsyncKeyState('R') & 0x8000)	mCrateWorld(3,1) -= 2.0f*dt;
	if(GetAsyncKeyState('T') & 0x8000)	mCrateWorld(3,1) += 2.0f*dt;

	if(mCrateWorld(3,1) <= 1.0f) mCrateWorld(3,1) = 1.0f;

	// Restrict the angle mPhi.
	if( mPhi < 0.1f )	mPhi = 0.1f;
	if( mPhi > PI-0.1f)	mPhi = PI-0.1f;

	// Convert Spherical to Cartesian coordinates: mPhi measured from +y
	// and mTheta measured counterclockwise from -z.
	mEyePos.x =  mRadius*sinf(mPhi)*sinf(mTheta);
	mEyePos.z = -mRadius*sinf(mPhi)*cosf(mTheta);
	mEyePos.y =  mRadius*cosf(mPhi);

	// Build the view matrix.
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &mEyePos, &target, &up);
}

void Mirror::DrawContent()
{

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);
    md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

 
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        ID3D10EffectPass* pass = mTech->GetPassByIndex( p );

		mfxEyePosVar->SetRawValue(&mEyePos, 0, sizeof(D3DXVECTOR3));
		mfxLightVar->SetRawValue(&mParallelLight, 0, sizeof(Light));

		//
		// Draw the floor and walls
		//
		mWVP = mRoomWorld*mView*GetCamera().proj();
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&mRoomWorld);
		mfxSpecMapVar->SetResource(mSpecMapRV);
		mfxTexMtxVar->SetMatrix((float*)&mIdentityTexMtx);
		UINT stride = sizeof(Vertex_3D);
		UINT offset = 0;
		md3dDevice->IASetVertexBuffers(0, 1, &mRoomVB, &stride, &offset);
		drawRoom(pass);
		//
		// Draw the crate
		// 
		mWVP = mCrateWorld*mView*GetCamera().proj();
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&mCrateWorld);
		mfxDiffuseMapVar->SetResource(mCrateDiffuseMapRV);
		mfxSpecMapVar->SetResource(mSpecMapRV);
		mfxTexMtxVar->SetMatrix((float*)&mIdentityTexMtx);
        pass->Apply(0);
		mCrateMesh.draw();
		//
		// Draw the mirror to the back buffer and stencil buffer last
		//
		mWVP = mRoomWorld*mView*GetCamera().proj();
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&mRoomWorld);
		mfxSpecMapVar->SetResource(mSpecMapRV);
		mfxTexMtxVar->SetMatrix((float*)&mIdentityTexMtx);
		md3dDevice->IASetVertexBuffers(0, 1, &mRoomVB, &stride, &offset);

		md3dDevice->OMSetDepthStencilState(mDrawMirrorDSS, 1);
		drawMirror(pass);
		md3dDevice->OMSetDepthStencilState(0, 0);
		//
		// Draw reflected crate in mirror.
		//
		D3DXPLANE mirrorPlane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
		 
		D3DXMATRIX R;
		D3DXMatrixReflect(&R, &mirrorPlane);
		D3DXMATRIX W = mCrateWorld*R;
		mWVP = W*mView*GetCamera().proj();
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&W);
		mfxDiffuseMapVar->SetResource(mCrateDiffuseMapRV);
		mfxSpecMapVar->SetResource(mSpecMapRV);
		mfxTexMtxVar->SetMatrix((float*)&mIdentityTexMtx);

		D3DXVECTOR3 oldDir = mParallelLight.dir;
		D3DXVec3TransformNormal(&mParallelLight.dir, &mParallelLight.dir, &R);
		mfxLightVar->SetRawValue(&mParallelLight, 0, sizeof(Light));	
        pass->Apply(0);

		md3dDevice->RSSetState(mCullCWRS);
		float blendf[] = {0.65f, 0.65f, 0.65f, 1.0f};
		md3dDevice->OMSetBlendState(mDrawReflectionBS, blendf, 0xffffffff);
		md3dDevice->OMSetDepthStencilState(mDrawReflectionDSS, 1);
		mCrateMesh.draw();

		md3dDevice->OMSetDepthStencilState(0, 0);
		md3dDevice->OMSetBlendState(0, blendf, 0xffffffff);
		md3dDevice->RSSetState(0);	
		mParallelLight.dir = oldDir; // restore
    }
}

void Mirror::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
  
	mFX = gMFX;

	mTech = mFX->GetTechniqueByName("TexTech");
	
	mfxWVPVar        = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar      = mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxEyePosVar     = mFX->GetVariableByName("gEyePosW");
	mfxLightVar      = mFX->GetVariableByName("gParaLight");
	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();
}

void Mirror::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 3, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

 
void Mirror::buildRoomGeometry()
{
	// Create and specify geometry.  For this sample we draw a floor
	// and a wall with a mirror on it.  We put the floor, wall, and
	// mirror geometry in one vertex buffer.
	//
	//   |--------------|
	//   |              |
    //   |----|----|----|
    //   |Wall|Mirr|Wall|
	//   |    | or |    |
    //   /--------------/
    //  /   Floor      /
	// /--------------/

 
	Vertex_3D v[30];

	// Floor: Observe we tile texture coordinates.
	v[0] = Vertex_3D(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	v[1] = Vertex_3D(-7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex_3D( 7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);
	
	v[3] = Vertex_3D(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	v[4] = Vertex_3D( 7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);
	v[5] = Vertex_3D( 7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f);

	// Wall: Observe we tile texture coordinates, and that we
	// leave a gap in the middle for the mirror.
	v[6]  = Vertex_3D(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[7]  = Vertex_3D(-7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8]  = Vertex_3D(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);
	
	v[9]  = Vertex_3D(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[10] = Vertex_3D(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);
	v[11] = Vertex_3D(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 2.0f);

	v[12] = Vertex_3D(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[13] = Vertex_3D(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex_3D(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);
	
	v[15] = Vertex_3D(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[16] = Vertex_3D(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);
	v[17] = Vertex_3D(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 2.0f);

	v[18] = Vertex_3D(-7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex_3D(-7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex_3D( 7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);
	
	v[21] = Vertex_3D(-7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex_3D( 7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);
	v[23] = Vertex_3D( 7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 1.0f);

	// Mirror
	v[24] = Vertex_3D(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[25] = Vertex_3D(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[26] = Vertex_3D( 2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	
	v[27] = Vertex_3D(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[28] = Vertex_3D( 2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[29] = Vertex_3D( 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex_3D) * 30;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = v;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mRoomVB));
}

void Mirror::drawRoom(ID3D10EffectPass* pass)
{
	mfxDiffuseMapVar->SetResource(mFloorDiffuseMapRV);
	pass->Apply(0);
	md3dDevice->Draw(6, 0);

	mfxDiffuseMapVar->SetResource(mWallDiffuseMapRV);
	pass->Apply(0);
	md3dDevice->Draw(18, 6);
}

void Mirror::drawMirror(ID3D10EffectPass* pass)
{
	mfxDiffuseMapVar->SetResource(mMirrorDiffuseMapRV);
	pass->Apply(0);
	md3dDevice->Draw(6, 24);
}

*/