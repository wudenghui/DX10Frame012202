#include "d3dUtil.h"

typedef std::vector<D3DXVECTOR3> VertexList;
typedef std::vector<DWORD> IndexList;
typedef std::vector<UINT> AtrList;

int HARD_EXCLUSE = 0;

int mClientWidth = 800;
int mClientHeight = 600;

HINSTANCE mhAppInst;
HWND      mhMainWnd;

bool MODE_3D = false;	// 是否是3D模式，如果是，则鼠标位置会变化，否则鼠标位置始终位于屏幕中央

// 摩擦力系数
float FricFactor = 0.3f;

// 拾取
RayLine mPickRayLine;

//***************************************************************************************
// Name: Subdivide
// Desc: Function subdivides every input triangle into four triangles of equal area.
//***************************************************************************************
void Subdivide(VertexList& vertices, IndexList& indices)
{
	VertexList vin = vertices;
	IndexList  iin = indices;

	vertices.resize(0);
	indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT numTris = (UINT)iin.size()/3;
	for(UINT i = 0; i < numTris; ++i)
	{
		D3DXVECTOR3 v0 = vin[ iin[i*3+0] ];
		D3DXVECTOR3 v1 = vin[ iin[i*3+1] ];
		D3DXVECTOR3 v2 = vin[ iin[i*3+2] ];

		D3DXVECTOR3 m0 = 0.5f*(v0 + v1);
		D3DXVECTOR3 m1 = 0.5f*(v1 + v2);
		D3DXVECTOR3 m2 = 0.5f*(v0 + v2);

		vertices.push_back(v0); // 0
		vertices.push_back(v1); // 1
		vertices.push_back(v2); // 2
		vertices.push_back(m0); // 3
		vertices.push_back(m1); // 4
		vertices.push_back(m2); // 5
 
		indices.push_back(i*6+0);
		indices.push_back(i*6+3);
		indices.push_back(i*6+5);

		indices.push_back(i*6+3);
		indices.push_back(i*6+4);
		indices.push_back(i*6+5);

		indices.push_back(i*6+5);
		indices.push_back(i*6+4);
		indices.push_back(i*6+2);

		indices.push_back(i*6+3);
		indices.push_back(i*6+1);
		indices.push_back(i*6+4);
	}
}

//***************************************************************************************
// Name: BuildGeoSphere
// Desc: Function approximates a sphere by tesselating an icosahedron.
//***************************************************************************************
void BuildGeoSphere(UINT numSubdivisions, float radius, VertexList& vertices, IndexList& indices)
{
	// Put a cap on the number of subdivisions.
	numSubdivisions = Min(numSubdivisions, UINT(5));

	// Approximate a sphere by tesselating an icosahedron.

	const float X = 0.525731f; 
	const float Z = 0.850651f;

	D3DXVECTOR3 pos[12] = 
	{
		D3DXVECTOR3(-X, 0.0f, Z),  D3DXVECTOR3(X, 0.0f, Z),  
		D3DXVECTOR3(-X, 0.0f, -Z), D3DXVECTOR3(X, 0.0f, -Z),    
		D3DXVECTOR3(0.0f, Z, X),   D3DXVECTOR3(0.0f, Z, -X), 
		D3DXVECTOR3(0.0f, -Z, X),  D3DXVECTOR3(0.0f, -Z, -X),    
		D3DXVECTOR3(Z, X, 0.0f),   D3DXVECTOR3(-Z, X, 0.0f), 
		D3DXVECTOR3(Z, -X, 0.0f),  D3DXVECTOR3(-Z, -X, 0.0f)
	};

	DWORD k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

	vertices.resize(12);
	indices.resize(60);

	for(int i = 0; i < 12; ++i)
		vertices[i] = pos[i];

	for(int i = 0; i < 60; ++i)
		indices[i] = k[i];

	for(UINT i = 0; i < numSubdivisions; ++i)
		Subdivide(vertices, indices);

	// Project vertices onto sphere and scale.
	for(size_t i = 0; i < vertices.size(); ++i)
	{
		D3DXVec3Normalize(&vertices[i], &vertices[i]);
		vertices[i] *= radius;
	}
}

//***************************************************************************************
// Name: AngleFromXY
// Desc: Returns the polar angle of the point (x,y) in [0, 2*PI).
//***************************************************************************************
float AngleFromXY(float x, float y)
{
	float theta = 0.0f;
 
	// Quadrant I or IV
	if(x >= 0.0f) 
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if(theta < 0.0f)
			theta += 2.0f*PI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else      
		theta = atanf(y/x) + PI; // in [0, 2*pi).

	return theta;
}


int NumFile(char* fileName, char* form)
{
	// 计算文件数目
	int numFile = 0;	// 文件总数
	char fName[100] = {};
	
	sprintf(fName, "%s%c%s", fileName, '*', form);	// 未做失败判断

	struct _finddata_t	fileInfo;	// 文本信息结构体
	long hFile = _findfirst( fName, &fileInfo);
	if(hFile == -1)
	{
		MessageBox(0, "找不到文件", "numFile错误", 50);
		return -1;
	}
	numFile++;	// 第一个文件
	while(_findnext(hFile, &fileInfo) != -1)
	{
		numFile++;
	}
	_findclose(hFile);
	return numFile;
	
}	// 结束numFile()

void GetPicSize(ID3D10ShaderResourceView* pic, float* width, float* height)
{
	if(width == NULL && height == NULL)
		return;

	ID3D10Resource* colorTex;

	pic->GetResource(&colorTex);
	 D3D10_TEXTURE2D_DESC colorTexDesc;

    ( (ID3D10Texture2D*)colorTex )->GetDesc( &colorTexDesc );
    colorTex->Release( );

	if(width)
		(*width) = (float)colorTexDesc.Width;

	if(height)
		(*height) = (float)colorTexDesc.Height;
}

float Min(float x, float y)
{
	return x>y ? y : x;
}
float Max(float x, float y)
{
	return x>y ? x : y;
}

void Swap( int* x1, int* x2)
{
	*x1 += *x2;
	*x2 = *x1 - *x2;
	*x1 = *x1 - *x2;
}


// 传入的向量不需要单位化
D3DXVECTOR3 ComputeReflectLineToLine(D3DXVECTOR3* pIn, D3DXVECTOR3* pNorm)
{
	// 单位化法向量，简化计算
	D3DXVec3Normalize( pNorm, pNorm);

	// 对应于输入向量的法线的模
	static float valueN;
	valueN = fabs( D3DXVec3Dot( pIn, pNorm) );

	*pNorm = (*pNorm) * valueN;

	return (*pIn) + (*pNorm) + (*pNorm);
}

D3DXVECTOR3 ComputeCrashSpeed(D3DXVECTOR3* pIn, D3DXVECTOR3* pNorm)
{
	return D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
}

D3DXVECTOR3 ComputeNormPointToLine(D3DXVECTOR3* pIn, D3DXVECTOR3* pDir)
{
	D3DXVec3Normalize(pDir, pDir);

	static float valueD;
	valueD = fabs( D3DXVec3Dot( pIn, pDir) );

	return (*pIn) - (*pDir)*valueD;
}

D3DXVECTOR3 ComputeVVecLineToNorm(D3DXVECTOR3* pIn, D3DXVECTOR3* pNorm)
{
	// 单位化法向量，简化计算
	D3DXVec3Normalize( pNorm, pNorm);

	// 对应于输入向量的法线的模
	static float valueN;
	valueN = fabs( D3DXVec3Dot( pIn, pNorm) );

	*pNorm = (*pNorm) * valueN;

	return (*pIn) + (*pNorm);
}