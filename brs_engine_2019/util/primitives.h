#pragma once

#include "../globals.h"

class GroundPlane
{
public:
	GroundPlane();
	~GroundPlane();
	void init(int xres, int yres, float fade = 1.0f, float size = 10.0f, float y = 0.0f, float wrap = 1.0f, float ydispmin = 0.0f, float ydispmax = 0.0f);
	void draw(float modulation = 1.0f);
	void setDrawTop(bool top);

private:
	class Vertex* m_vertices;

	int m_xres;
	int m_yres;
	int m_vertexcount;
	bool m_drawTop;
	

};



class Primitives
{
public:

	//demo specific
	static void noiseOverlay(float power, float alpha);
	static void star2D(Vector3& position, float rotation, float size);

	static void dashedLine(Vector3 &v1, Vector3 &v2, float steplength, float spacing = 0.5f);
	
	//common
	static void fullscreenFade(const Color4& color);
	static void fullscreenPicture(bool xflip = false, bool yflip = false, float xoff = 0.0f, float yoff = 0.0f);
	static void picture2D(string picture, float x, float y, float size);
	static void cubeWireframe(float size);
	static void cubeFlat(Color3& color, float size, bool textured = false);


private:
};
