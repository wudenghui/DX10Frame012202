#ifndef TEST
#define TEST

#include"Box.h"

class Test : public GameObj
{
public:

	Test();
	~Test();

	void init( );
	
	void DrawContent();
	void ActionUpdate(float dt);

protected:
	Box box;

};


#endif