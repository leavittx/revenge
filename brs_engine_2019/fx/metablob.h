#pragma once

#include "../globals.h"

class Metablob : public Scene
{
public:
  Metablob() {}
 ~Metablob() {}

  void init();
  void update();
  void draw();
  void release();

private:
  bool m_debug;

  float res[2];
};
