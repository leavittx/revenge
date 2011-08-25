#pragma once

#include "../globals.h"

class CreditsScene : public Scene
{
public:
	CreditsScene() {}
	~CreditsScene() {}

	void init();
	void update();
	void draw();
	void release();

private:
};
