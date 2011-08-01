#pragma once

#include "../globals.h"

class Heart : public Scene
{
public:
  Heart() {};
 ~Heart() {};

  void init();
  void update();
  void draw();
  void release();

private:
  bool m_debug;

  float res[2];
};
