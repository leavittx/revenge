#pragma once

#include "../globals.h"

class EarScene : public Scene
{
public:
	EarScene() {}
	~EarScene() {}

	void init();
	void update();
	void draw();
	void release();

private:
//	bool m_debug;
	float res[2];
};
