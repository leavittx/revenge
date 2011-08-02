#include "brain.h"

void Brain::init()
{
  g_params.useNamespace("brain");
  m_debug = g_params.getBool("debug");
}
void Brain::release()
{
}
void Brain::update()
{
  g_params.useNamespace("brain");
}
void Brain::draw()
{
  glLoadIdentity();

  glClearColor(1, 0, 0, 0);
  g_params.useNamespace("brain");
}
