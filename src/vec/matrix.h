/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * matrix header
 *
 *==========================================================*/

#ifndef _MATRIX_H_
#define _MATRIX_H_
#include "../util/types.h"
#include <math.h>

#define M3X3(_a,_b,_c,_d,_e,_f,_g,_h,_i) (m3x3_t) {.m={ \
  (_a), (_b), (_c),                                     \
  (_d), (_e), (_f),                                     \
  (_g), (_h), (_i),}}
#define M4X4(_a,_b,_c,_d,_e,_f,_g,_h,_i,_j,_k,_l,_m,_n,_o,_p) (m4x4_t) {.m={ \
  (_a), (_b), (_c), (_d)                                                     \
  (_e), (_f), (_g), (_h)                                                     \
  (_i), (_j), (_k), (_l)                                                     \
  (_m), (_n), (_o), (_p)}}

#define RX3X3(_ang) M3X3(     \
  1.0,      0.0,         0.0, \
  0.0, cos(_ang), -sin(_ang), \
  0.0, sin(_ang),  cos(_ang))

#define RY3X3(_ang) M3X3(      \
  cos(_ang),  0.0,  sin(_ang), \
  0.0,        1.0,        0.0, \
  -sin(_ang), 0.0,  cos(_ang))

#define RZ3X3(_ang) M3X3(     \
  cos(_ang), -sin(_ang), 0.0, \
  sin(_ang),  cos(_ang), 0.0, \
  0.0,              0.0, 1.0)

#define FOV_RAD(angle)  (1.0 / tan((angle) * 0.008726646259971647884618453842443063))

#define PERSPECTIVE(near, far, fov, ar) M4X4(                                    \
  ar * FOV_RAD((fov)), 0.0,            0.0,                                  0.0,\
  0.0,                 FOV_RAD((fov)), 0.0,                                  0.0,\
  0.0,                 0.0,            (far) / ((far)-(near)),               1.0,\
  0.0,                 0.0,            (-(far) * (near)) / ((far) - (near)), 0.0)


typedef struct m3x3 { f64_t m[9]; } m3x3_t;
typedef struct m4x4 { f64_t m[16];} m4x4_t;


#define m3x3_mult(a, b) M3X3(     \
  a.m[0]*b.m[0]+a.m[1]*b.m[4]+a.m[2]*b.m[6],a.m[0]*b.m[1]+a.m[1]*b.m[4]+a.m[2]*b.m[7],a.m[0]*b.m[2]+a.m[1]*b.m[5]+a.m[2]*b.m[8], \
  a.m[3]*b.m[0]+a.m[4]*b.m[4]+a.m[5]*b.m[6],a.m[3]*b.m[1]+a.m[4]*b.m[4]+a.m[5]*b.m[7],a.m[3]*b.m[2]+a.m[4]*b.m[5]+a.m[5]*b.m[8], \
  a.m[6]*b.m[0]+a.m[7]*b.m[4]+a.m[8]*b.m[6],a.m[6]*b.m[1]+a.m[7]*b.m[4]+a.m[8]*b.m[7],a.m[6]*b.m[2]+a.m[7]*b.m[5]+a.m[8]*b.m[8])


#endif /* _MATRIX_H_ */
