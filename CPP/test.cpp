#include"test.h"

Test::Test()
{
}
 
Test::~Test()
{
	
}

void Test::init()
{
	ID3D10ShaderResourceView* diffuse[3] = {GetTextureMgr().GetTexRVByName("PIC/stone_diffuse.dds"),
		GetTextureMgr().GetTexRVByName("PIC/bricks_diffuse.dds"),
		GetTextureMgr().GetTexRVByName("PIC/floor_diffuse.DDS")};
	
	ID3D10ShaderResourceView* spec[3] = {GetTextureMgr().GetTexRVByName("PIC/spec.dds"),
		GetTextureMgr().GetTexRVByName("PIC/spec.dds"),
		GetTextureMgr().GetTexRVByName("PIC/spec.dds")};

	ID3D10ShaderResourceView* norm[3] = {GetTextureMgr().GetTexRVByName("PIC/stone_normal.dds"),
		GetTextureMgr().GetTexRVByName("PIC/bricks_normal.dds"),
		GetTextureMgr().GetTexRVByName("PIC/floor_normal.DDS")};
	D3DXVECTOR3 ref = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 reflect[3] = { ref, ref, ref};

	box.Init( 1.0f, 20.0f, 1.0f,
		GetCamera().position().x, GetCamera().position().y, GetCamera().position().z + 3.0f,
		CubeMapFX, "NormalMapTech",  "ShadowTech",
		3, diffuse, spec, norm, reflect);
}

void Test::DrawContent()
{
	box.DrawContent();
}

void Test::ActionUpdate(float dt)
{

	box.SetPosition( GetCamera().position() + D3DXVECTOR3 (0.0f, 0.0f, 18.0f));
}