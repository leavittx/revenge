#pragma once

#include "../globals.h"

class Brain : public Scene
{
public:  
	Brain() {}
	~Brain() {}

	void init();
	void update();
	void draw();
	void release();

private:
	bool m_debug;
};
