#include <stdio.h>

class Vector
{
  public:
    float x,y,z;

  Vector()
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }

  Vector(const float* v)
  {
    x = v[0];
    y = v[1];
    z = v[2];
  }

  Vector(const float* v)
  {
    x = v[0];
    y = v[1];
    z = v[2];
  }

  Vector(const float* p1, const float* p2)
  {
    x = p2[0]-p1[0];
    y = p2[1]-p1[1];
    z = p2[2]-p1[2];
  }

  void normalizer()
  {
    float len;
    len = sqrt(x*x + y*y + z*z);
    x = x/len;
    y = y/len;
    z = z/len;
  }
}
