/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * vector functions for float vectors and functions to
 * use them with the matrix library.
 *
 *==========================================================*/

#include "vec.h"
#include <stdio.h>

fvec2_t f2normalized(fvec2_t v)
{
  f64_t l = (f64_t)f2len(v);
  return FVEC2(v.x/l, v.y/l);
}
void normalizef2(fvec2_t *v)
{
  f64_t l = (f64_t)f2len(*v);
  v->x /= l;
  v->y /= l;
}

fvec3_t f3normalized(fvec3_t v)
{
  f64_t l = (f64_t)f3len(v);
  return FVEC3(v.x/l, v.y/l, v.z/l);
}
void normalizef3(fvec3_t *v)
{
  f64_t l = (f64_t)f3len(*v);
  v->x /= l;
  v->y /= l;
  v->z /= l;
}

fvec4_t f4normalized(fvec4_t v)
{
  f64_t l = (f64_t)f4len(v);
  return FVEC4(v.x/l, v.y/l, v.z/l, v.w/l);
}
void normalizef4(fvec4_t *v)
{
  f64_t l = (f64_t)f4len(*v);
  v->x /= l;
  v->y /= l;
  v->z /= l;
  v->w /= l;
}

fvec3_t f3matmult(m3x3_t m, fvec3_t v)
{
  return FVEC3((m.m[0] * v.x) + (m.m[1] * v.y) + (m.m[2] * v.z),
               (m.m[3] * v.x) + (m.m[4] * v.y) + (m.m[5] * v.z),
               (m.m[6] * v.x) + (m.m[7] * v.y) + (m.m[8] * v.z));
}

void fvec2_print(fvec2_t v)
{
  printf("[%3f, %3f]\n", v.x, v.y);
}
void fvec3_print(fvec3_t v)
{
  printf("[%3f, %3f, %3f]\n", v.x, v.y, v.z);
}
void fvec4_print(fvec4_t v)
{
  printf("[%3f, %3f, %3f, %3f]\n", v.x, v.y, v.z, v.w);
}
