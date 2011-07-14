#include "scene.h"

Scene::Scene()
{
    m_pos = 0.0f;
}

Scene::~Scene()
{
    //empty virtual destructor
}

void Scene::setPosition(const float t)
{
    m_pos = t;
}