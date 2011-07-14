#include "primitives.h"

GroundPlane::GroundPlane()
{
	m_vertices = 0;
	m_xres = 0;
	m_yres = 0;
	m_drawTop = false;
}
GroundPlane::~GroundPlane()
{
}

void GroundPlane::setDrawTop(bool top)
{
	m_drawTop = top;
}

void GroundPlane::init(int xres, int yres, float fade, float size, float ypos, float wrap, float ydispmin, float ydispmax)
{
	m_xres = xres;
	m_yres = yres;

	m_vertexcount = xres * yres;

	m_vertices = new Vertex[m_vertexcount];
	for (int y = 0; y < m_yres; y++)
	{
		float yt = y / (float)m_yres;
		for (int x = 0; x < m_xres; x++)
		{
			float xt = x / (float)m_xres;

			float d = sqrtf((xt - 0.5f) * (xt - 0.5f) + (yt - 0.5f) * (yt - 0.5f));
			float fadevalue = powf((0.5f - d)/0.5f, fade);

			Vector3 pos = Vector3((xt - 0.5f) * size, ypos + Math::randBetween(ydispmin, ydispmax), (yt - 0.5f) * size);
			Vertex v;
			v.texcoord = TexCoord(xt * wrap, yt * wrap);
			v.color = Color3(1,1,1) * (fadevalue);
			v.position = pos;
			v.normal = Vector3(0, 1, 0);
			m_vertices[x + y * m_xres] = v;
		}
	}
}


void GroundPlane::draw(float modulation)
{
	//WTF?
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	for (int y = 0; y < m_yres - 1; y++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < m_xres; x++)
		{
			Vertex& v1 = m_vertices[x + y * m_xres];
			Vertex& v2 = m_vertices[x + (y + 1) * m_xres];

			glColor4f(1,1,1,1);
			v1.color.useModulated(modulation);
			glNormal3fv((float *)&v1.normal);
			glTexCoord2f(v1.texcoord.u, v1.texcoord.v);
			glVertex3fv((float *)&v1.position);

			v2.color.useModulated(modulation);
			glNormal3fv((float *)&v2.normal);
			glTexCoord2f(v2.texcoord.u, v2.texcoord.v);
			glVertex3fv((float *)&v2.position);
		}
		glEnd();
	}

	if (m_drawTop)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glColor3f(0,0,0);
		Vector3 add = Vector3(0, 0.001f, 0);
		for (int y = 0; y < m_yres - 1; y++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int x = 0; x < m_xres; x++)
			{
				Vertex& v1 = m_vertices[x + y * m_xres];
				Vertex& v2 = m_vertices[x + (y + 1) * m_xres];

				glVertex3fv((float *)&(v1.position + add));
				glVertex3fv((float *)&(v2.position + add));
			}
			glEnd();
		}
	}

}


void Primitives::picture2D(string picture, float x, float y, float size)
{
	g_system.setOrthoMode(1, 1);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthMask(0);
	g_textures.bindTexture(picture, GL_TEXTURE0_ARB);

	Image& image = g_textures.image(picture);

	float width = (float)image.getWidth();
	float height = (float)image.getHeight();

	float maxdim = max(width, height);
	width /= maxdim;
	height /= maxdim;

	float aspect = width / height;

	width *= size;
	height *= size;

	height *= g_system.getAspectRatio();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(x - width/2, y - height/2);
	glTexCoord2f(1, 1);
	glVertex2f(x + width/2, y - height/2);
	glTexCoord2f(1, 0);
	glVertex2f(x + width/2, y + height/2);
	glTexCoord2f(0, 0);
	glVertex2f(x - width/2, y + height/2);
	glEnd();


	g_system.setPerspectiveMode();
	glDepthMask(1);
	
}


void Primitives::noiseOverlay(float power, float alpha)
{
/*
	glColor4f(1,1,1,alpha * (power + (1-power) * Math::randFloat()));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    switch(rand()%6)
    {

		case 0: g_textures.bindTexture("noise1.jpg", GL_TEXTURE0_ARB); break;
        case 1: g_textures.bindTexture("noise2.jpg", GL_TEXTURE0_ARB); break;
        case 2: g_textures.bindTexture("noise3.jpg", GL_TEXTURE0_ARB); break;
		case 3: g_textures.bindTexture("kohina1.png", GL_TEXTURE0_ARB); break;
        case 4: g_textures.bindTexture("kohina2.png", GL_TEXTURE0_ARB); break;
        case 5: g_textures.bindTexture("kohina3.png", GL_TEXTURE0_ARB); break;
    }
	Primitives::fullscreenPicture(rand()%2 == 0, rand()%2 == 0, 0, 0);//Math::randFloat(), Math::randFloat());
*/
}
void Primitives::fullscreenFade(Color4& color)
{
	glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
	g_system.setOrthoMode(1,1);
	glLoadIdentity();

	g_textures.clearTextureUnits();
	color.use();
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(1, 0);
	glVertex2f(1, 1);
	glVertex2f(0, 1);
	glEnd();

    glDepthMask(1);
	g_system.setPerspectiveMode();
	glLoadIdentity();

}
void Primitives::fullscreenPicture(bool xflip, bool yflip, float xoff, float yoff)
{
	glDisable(GL_DEPTH_TEST);
    glDepthMask(0);

	g_system.setOrthoMode(1,1);
	glLoadIdentity();

	float x0 = 0;
	float x1 = 1;
	float y0 = 1;
	float y1 = 0;

	if (xflip)
	{
		x1 = 0;
		x0 = 1;
	}
	if (yflip)
	{
		y0 = 0;
		y1 = 1;
	}

	glBegin(GL_QUADS);
	glTexCoord2f(x0+xoff, y0+yoff);
	glVertex2f(0, 0);
	glTexCoord2f(x1+xoff, y0+yoff);
	glVertex2f(1, 0);
	glTexCoord2f(x1+xoff, y1+yoff);
	glVertex2f(1, 1);
	glTexCoord2f(x0+xoff, y1+yoff);
	glVertex2f(0, 1);

	glEnd();

    glDepthMask(1);
	g_system.setPerspectiveMode();
	glLoadIdentity();
}


void Primitives::cubeWireframe(float size)
{
	Vector3 upleftback = Vector3( -size, -size, -size);
	Vector3 upleftfront = Vector3(-size, -size, +size);
	Vector3 uprightback = Vector3( size, -size, -size);
	Vector3 uprightfront = Vector3(size, -size, +size);

	Vector3 downleftback = Vector3( -size, size, -size);
	Vector3 downleftfront = Vector3(-size, size, +size);
	Vector3 downrightback = Vector3( size, size, -size);
	Vector3 downrightfront = Vector3(size, size, +size);

	//upper part
	glBegin(GL_LINE_LOOP);
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&upleftfront);
	glEnd();

	//lower part
	glBegin(GL_LINE_LOOP);
	glVertex3fv((float *)&downleftback);
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downleftfront);
	glEnd();

	//connecting pieces
	glBegin(GL_LINES);
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&downleftback);
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&downleftfront);
	glEnd();

}

void Primitives::cubeFlat(Color3& color, float size, bool textured)
{
	Vector3 upleftback = Vector3( -size, -size, -size);
	Vector3 upleftfront = Vector3(-size, -size, +size);
	Vector3 uprightback = Vector3( size, -size, -size);
	Vector3 uprightfront = Vector3(size, -size, +size);

	Vector3 downleftback = Vector3( -size, size, -size);
	Vector3 downleftfront = Vector3(-size, size, +size);
	Vector3 downrightback = Vector3( size, size, -size);
	Vector3 downrightfront = Vector3(size, size, +size);

	glBegin(GL_QUADS);
	glColor3fv((float *)&color);

	if (textured)
	{
		//yläsivu
		glNormal3f(0, 1, 0);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
		glVertex3fv((float *)&upleftback);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
		glVertex3fv((float *)&uprightback);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
		glVertex3fv((float *)&uprightfront);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
		glVertex3fv((float *)&upleftfront);

		//alasivu
		glNormal3f(0, -1, 0);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
		glVertex3fv((float *)&downleftback);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
		glVertex3fv((float *)&downrightback);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
		glVertex3fv((float *)&downrightfront);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
		glVertex3fv((float *)&downleftfront);

		//etusivu
		glNormal3f(0, 0, -1);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
		glVertex3fv((float *)&upleftfront);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
		glVertex3fv((float *)&uprightfront);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
		glVertex3fv((float *)&downrightfront);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
		glVertex3fv((float *)&downleftfront);

		//takasivu
		glNormal3f(0, 0, 1);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
		glVertex3fv((float *)&upleftback);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
		glVertex3fv((float *)&uprightback);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
		glVertex3fv((float *)&downrightback);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
		glVertex3fv((float *)&downleftback);

		//vasen sivu
		glNormal3f(1, 0, 0);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
		glVertex3fv((float *)&upleftback);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
		glVertex3fv((float *)&upleftfront);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
		glVertex3fv((float *)&downleftfront);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
		glVertex3fv((float *)&downleftback);

		//oikea sivu
		glNormal3f(-1, 0, 0);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
		glVertex3fv((float *)&uprightback);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
		glVertex3fv((float *)&uprightfront);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
		glVertex3fv((float *)&downrightfront);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
		glVertex3fv((float *)&downrightback);
	}
	else
	{
		//yläsivu
		glNormal3f(0, 1, 0);
		glVertex3fv((float *)&upleftback);
		glVertex3fv((float *)&uprightback);
		glVertex3fv((float *)&uprightfront);
		glVertex3fv((float *)&upleftfront);

		//alasivu
		glNormal3f(0, -1, 0);
		glVertex3fv((float *)&downleftback);
		glVertex3fv((float *)&downrightback);
		glVertex3fv((float *)&downrightfront);
		glVertex3fv((float *)&downleftfront);

		//etusivu
		glNormal3f(0, 0, -1);
		glVertex3fv((float *)&upleftfront);
		glVertex3fv((float *)&uprightfront);
		glVertex3fv((float *)&downrightfront);
		glVertex3fv((float *)&downleftfront);

		//takasivu
		glNormal3f(0, 0, 1);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
		glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);

		//vasen sivu
		glNormal3f(1, 0, 0);
		glVertex3fv((float *)&upleftback);
		glVertex3fv((float *)&upleftfront);
		glVertex3fv((float *)&downleftfront);
		glVertex3fv((float *)&downleftback);

		//oikea sivu
		glNormal3f(-1, 0, 0);
		glVertex3fv((float *)&uprightback);
		glVertex3fv((float *)&uprightfront);
		glVertex3fv((float *)&downrightfront);
		glVertex3fv((float *)&downrightback);
	}
	glEnd();
}


void Primitives::dashedLine(Vector3 &v1, Vector3 &v2, float steplength, float spacing)
{
	glBegin(GL_LINES);

	float length = (v2 - v1).length();
	int steps = (int)(length / steplength);

	Vector3 dir = v2 - v1;
	dir.normalize();

	Vector3 cursor = v1;

	for (int i = 0; i < steps; i++)
	{
		glVertex3fv((float *)&cursor);
		glVertex3fv((float *)&(cursor + dir * steplength*spacing));
		cursor += dir * steplength;
	}
	glEnd();
}
