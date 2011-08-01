#pragma once

#include "../globals.h"

class Runko : public Scene
{
public:
        Runko() {}
        ~Runko() {}
	
	void init();
	void update();
	void draw();
	void release();
	
private:
	bool m_debug;
	
};
