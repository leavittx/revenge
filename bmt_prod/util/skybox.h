#pragma once

#include "../globals.h"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	void init(string top, string bottom, string left, string right, string front, string back);
	void draw(float size, Color3& color, float modulation = 1.0f);
	void draw(Vector3& direction, Color3& color, float modulation = 1.0f);

private:
	string m_topTexture;
	string m_bottomTexture;
	string m_leftTexture;
	string m_rightTexture;
	string m_frontTexture;
	string m_backTexture;
};
