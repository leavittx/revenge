#pragma once

#include "../globals.h"

class glUtil
{
public:
	//get vectors from the modelview matrix that undo the rotation and allow alignment with camera  
	static void antiRotate(Vector3& x, Vector3& y, Vector3& z);
	//set up modelview matrix quickly
        static void camLook(Vector3& cam, Vector3& target, const Vector3& up = Vector3(0, 1, 0));

	static void drawImage(Vector3& screenPos, float xsize, float ysize);

	//draw a fullscreen image onto the screen
	static void fullscreenQuad(string source);

	static void fullscreenQuad(Color4& color);
	//blit source to dest using a FBO
	static void blit(string source, string dest);

};
