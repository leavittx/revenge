#include "skybox.h"

SkyBox::SkyBox()
{
    m_topTexture = "";
    m_bottomTexture = "";
    m_leftTexture = "";
    m_rightTexture = "";
    m_frontTexture = "";
    m_backTexture = "";
}

SkyBox::~SkyBox()
{

}

void SkyBox::init(string top, string bottom, string left, string right, string front, string back)
{
    m_topTexture = top;
    m_bottomTexture = bottom;
    m_leftTexture = left;
    m_rightTexture = right;
    m_frontTexture = front;
    m_backTexture = back;
}

void SkyBox::draw(Vector3& direction, Color3& color, float modulation)
{
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(0, 0, 0,
              direction.x, direction.y, direction.z,
              0, 1, 0);

    draw(10.0f, color, modulation);
    glPopMatrix();
}


void SkyBox::draw(float size, Color3& color, float modulation)
{
    glDepthMask(0);
    glPushMatrix();

    Vector3 upleftback = Vector3( -size, -size, -size);
    Vector3 upleftfront = Vector3(-size, -size, +size);
    Vector3 uprightback = Vector3( size, -size, -size);
    Vector3 uprightfront = Vector3(size, -size, +size);

    Vector3 downleftback = Vector3( -size, size, -size);
    Vector3 downleftfront = Vector3(-size, size, +size);
    Vector3 downrightback = Vector3( size, size, -size);
    Vector3 downrightfront = Vector3(size, size, +size);

    color.useModulated(modulation);

    //yläsivu
    if (m_topTexture != "")
    {
        g_textures.bindTexture(m_topTexture);
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
        glVertex3fv((float *)&upleftback);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
        glVertex3fv((float *)&uprightback);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
        glVertex3fv((float *)&uprightfront);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
        glVertex3fv((float *)&upleftfront);
        glEnd();
    }
    //alasivu
    if (m_bottomTexture != "")
    {
        g_textures.bindTexture(m_bottomTexture);
        glBegin(GL_QUADS);
        glNormal3f(0, -1, 0);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
        glVertex3fv((float *)&downleftback);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
        glVertex3fv((float *)&downrightback);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
        glVertex3fv((float *)&downrightfront);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
        glVertex3fv((float *)&downleftfront);
        glEnd();
    }
    if (m_frontTexture != "")
    {
        //etusivu
        g_textures.bindTexture(m_frontTexture);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
        glVertex3fv((float *)&upleftfront);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
        glVertex3fv((float *)&uprightfront);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
        glVertex3fv((float *)&downrightfront);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
        glVertex3fv((float *)&downleftfront);
        glEnd();
    }
    if (m_backTexture != "")
    {

        //takasivu
        g_textures.bindTexture(m_backTexture);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
        glVertex3fv((float *)&upleftback);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
        glVertex3fv((float *)&uprightback);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
        glVertex3fv((float *)&downrightback);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
        glVertex3fv((float *)&downleftback);
        glEnd();
    }
    if (m_leftTexture != "")
    {

        //vasen sivu
        g_textures.bindTexture(m_leftTexture);
        glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
        glVertex3fv((float *)&upleftback);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
        glVertex3fv((float *)&upleftfront);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
        glVertex3fv((float *)&downleftfront);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
        glVertex3fv((float *)&downleftback);
        glEnd();
    }
    if (m_backTexture != "")
    {
        //oikea sivu
        g_textures.bindTexture(m_rightTexture);
        glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 0);
        glVertex3fv((float *)&uprightback);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 0);
        glVertex3fv((float *)&uprightfront);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 1, 1);
        glVertex3fv((float *)&downrightfront);
        glMultiTexCoord2f(GL_TEXTURE0_ARB, 0, 1);
        glVertex3fv((float *)&downrightback);
        glEnd();
    }
    glPopMatrix();
    glDepthMask(1);
}

