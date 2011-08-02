#pragma once

#include "../globals.h"

class Brain : public Scene
{
public:  
  Brain() {};
  ~Brain() {};

  void init();
  void update();
  void draw();
  void release();

private:
  MeshFactory mf;
  Mesh* brain;
  float a;
  bool m_debug;

};