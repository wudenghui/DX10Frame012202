#ifndef KEYBOARD
#define KEYBOARD

#include"DxInput.h"
#include"GameObj.h"

class Keyboard : public GameObj
{
public:
	friend Keyboard& GetKeyboard();
	void ActionUpdate(float dt);
	bool KeyPressed( int key );
	bool KeyClicked( int key );

	void Init();
	void Release();

private:
	Keyboard();
	~Keyboard();

private:
	LPDIRECTINPUTDEVICE8 keyboardDevice_;
	char keyboardKeys_[256];		// ��ǰ����
	char prevKeyboardKeys_[256];	// ǰһ������
};

Keyboard& GetKeyboard();

#endif