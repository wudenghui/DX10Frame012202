#ifndef DIRECT_INPUT
#define DIRECT_INPUT

#include"d3dUtil.h"
#include<dinput.h>

class DXInput
{
public:
	friend DXInput& GetDXInput();
	LPDIRECTINPUT8 GetDevice(){	return directInput_;	}

private:
	DXInput();
	~DXInput();

private:
	LPDIRECTINPUT8 directInput_;

};

DXInput& GetDXInput();

#endif