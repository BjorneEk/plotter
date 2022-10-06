/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * vector header for float vectors and functions to
 * use them with the matrix library.
 *
 *==========================================================*/

#ifndef _VECTOR_H_
#define _VECTOR_H_


#include <stdbool.h>
#include "../util/types.h"
#include "../util/macro.h"
#include "../util/math.h"
#include "matrix.h"


/**
 *  vector structs have the packed attribute ( "__attribute__((__packed__))" )
 *  this is so that they can be considered as f64_t * and therefore be converted
 *  into maricies
 **/


/**
 * 2, 3 and 4d f64_t vector structs
 **/
typedef struct PACKED { f64_t x, y; }       fvec2_t;
typedef struct PACKED { f64_t x, y, z; }    fvec3_t;
typedef struct PACKED { f64_t x, y, z, w; } fvec4_t;

/**
 * 2, 3 and 4d i32_t vector structs
 **/
typedef struct PACKED { i32_t x, y; }       ivec2_t;
typedef struct PACKED { i32_t x, y, z; }    ivec3_t;
typedef struct PACKED { i32_t x, y, z, w; } ivec4_t;

/**
 * vector constructor helper macros
 **/
#define VEC2(_x, _y)         { .x=(_x), .y=(_y)                   }
#define VEC3(_x, _y, _z)     { .x=(_x), .y=(_y), .z=(_z)          }
#define VEC4(_x, _y, _z, _w) { .x=(_x), .y=(_y), .z=(_z), .w=(_w) }
/**
 * constructor macros for the f64_t vector structs
 **/
#define FVEC2(_x, _y)         ((fvec2_t)VEC2((_x), (_y)))
#define FVEC3(_x, _y, _z)     ((fvec3_t)VEC3((_x), (_y), (_z)))
#define FVEC4(_x, _y, _z, _w) ((fvec4_t)VEC4((_x), (_y), (_z), (_w)))
/**
 * constructor macros for the i32_t vector structs
 **/
#define IVEC2(_x, _y)         ((ivec2_t)VEC2((_x), (_y)))
#define IVEC3(_x, _y, _z)     ((ivec3_t)VEC3((_x), (_y), (_z)))
#define IVEC4(_x, _y, _z, _w) ((ivec4_t)VEC4((_x), (_y), (_z), (_w)))

/**
 * fvec to ivec convertion macros
 **/
#define FVEC2_IVEC2(v) ((ivec2_t)VEC2((i32_t)(v).x, (i32_t)(v).y))
#define FVEC3_IVEC3(v) ((ivec3_t)VEC3((i32_t)(v).x, (i32_t)(v).y, (i32_t)(v).z))
#define FVEC4_IVEC4(v) ((ivec4_t)VEC4((i32_t)(v).x, (i32_t)(v).y, (i32_t)(v).z, (i32_t)(v).w))

/**
 * ivec to fvec convertion macros
 **/
#define IVEC2_FVEC2(v) ((fvec2_t)VEC2((f64_t)(v).x, (f64_t)(v).y))
#define IVEC3_FVEC3(v) ((fvec3_t)VEC3((f64_t)(v).x, (f64_t)(v).y, (f64_t)(v).z))
#define IVEC4_FVEC4(v) ((fvec4_t)VEC4((f64_t)(v).x, (f64_t)(v).y, (f64_t)(v).z, (f64_t)(v).w))

#define FVEC2_TO_FVEC3(v) (FVEC3((v).x, (v).y, 1.0))
#define FVEC2_TO_FVEC4(v) (FVEC4((v).x, (v).y, 1.0, 1.0))
#define FVEC3_TO_FVEC4(v) (FVEC4((v).x, (v).y, (v).z, 1.0))

/**
 * vector dot pruduct macros
 **/
#define f2dot(a, b) ((f64_t)(((a).x*(b).x) + ((a).y*(b).y)))
#define f3dot(a, b) ((f64_t)(((a).x*(b).x) + ((a).y*(b).y) + ((a).z*(b).z)))
#define f4dot(a, b) ((f64_t)(((a).x*(b).x) + ((a).y*(b).y) + ((a).z*(b).z) + ((a).w*(b).w)))

/**
 * vector cross produft functions
 **/
#define f3cross(a, b) ((fvec3_t) {\
  .x = (a).y * (b).z - (a).z * (b).y,\
  .y = (a).z * (b).x - (a).x * (b).z,\
  .z = (a).x * (b).y - (a).y * (b).x})
#define f4cross_(a, b) f3cross((a), (b))

/**
 * vector length macros
 **/
#define f2len(v) ((f64_t)(sqrt(fabs( f2dot( (v), (v) ) ))))
#define f3len(v) ((f64_t)(sqrt(fabs( f3dot( (v), (v) ) ))))
#define f4len(v) ((f64_t)(sqrt(fabs( f4dot( (v), (v) ) ))))

/**
 * vec-vec reference operations, such as +=, *= and /=
 *
 * example: v1 ,v2 are fvec3_t
 *  fvec3opR(v1, v2, +=); => v1 = v1 + v2
 **/
#define vec2opR(a, b, op) {(a).x op (b).x;(a).y op (b).y;}
#define vec3opR(a, b, op) {(a).x op (b).x;(a).y op (b).y;(a).z op (b).z;}
#define vec4opR(a, b, op) {(a).x op (b).x;(a).y op (b).y;(a).z op (b).z;(a).w op (b).w;}
/**
 * vec-scalar reference operations, such as +=, *= and /=
 **/
#define vec2SopR(a, b, op) {(a).x op (b);(a).y op (b);}
#define vec3SopR(a, b, op) {(a).x op (b);(a).y op (b);(a).z op (b);}
#define vec4SopR(a, b, op) {(a).x op (b);(a).y op (b);(a).z op (b);(a).w op (b);}
/**
 * vec-vec operations, such as +, * and /
 *
 * example: v1 ,v2 are fvec3_t
 *  fvec3op(v1, v2, +); returns a new fvec3 containing v1 + v2
 **/
#define fvec2op(a, b, op) (FVEC2(((a).x op (b).x), ((a).y op (b).y)))
#define ivec2op(a, b, op) (IVEC2(((a).x op (b).x), ((a).y op (b).y)))
#define fvec3op(a, b, op) (FVEC3(((a).x op (b).x), ((a).y op (b).y), ((a).z op (b).z)))
#define ivec3op(a, b, op) (IVEC3(((a).x op (b).x), ((a).y op (b).y), ((a).z op (b).z)))
#define fvec4op(a, b, op) (FVEC3(((a).x op (b).x), ((a).y op (b).y), ((a).z op (b).z), ((a).w op (b).w)))
#define ivec4op(a, b, op) (IVEC3(((a).x op (b).x), ((a).y op (b).y), ((a).z op (b).z), ((a).w op (b).w)))

/**
 * fvec-scalar operations, such as +, * and /
 **/
#define fvec2Sop(a, b, op) (FVEC2(((a).x op (b)), ((a).y op (b))))
#define ivec2Sop(a, b, op) (IVEC2(((a).x op (b)), ((a).y op (b))))
#define fvec3Sop(a, b, op) (FVEC3(((a).x op (b)), ((a).y op (b)), ((a).z op (b))))
#define ivec3Sop(a, b, op) (IVEC3(((a).x op (b)), ((a).y op (b)), ((a).z op (b))))
#define fvec4Sop(a, b, op) (FVEC3(((a).x op (b)), ((a).y op (b)), ((a).z op (b)), ((a).w op (b))))
#define ivec4Sop(a, b, op) (IVEC3(((a).x op (b)), ((a).y op (b)), ((a).z op (b)), ((a).w op (b))))

/**
 * squared vector macros
 **/
#define f2sqared(v) fvec2op((v), (v), *)
#define f3sqared(v) fvec3op((v), (v), *)
#define f4sqared(v) fvec4op((v), (v), *)
/*
#define f2sqared(v) (FVEC2(((v).x*(v).x), ((v).y*(v).y)))
#define f3sqared(v) (FVEC3(((v).x*(v).x), ((v).y*(v).y), ((v).z*(v).z)))
#define f4sqared(v) (FVEC4(((v).x*(v).x), ((v).y*(b).y), ((v).z*(b).z), ((v).w*(v).w)))
*/
#define f2equals(a, b) (((a).x == (b).x)&&((a).y == (b).y))
#define f3equals(a, b) (((a).x == (b).x)&&((a).y == (b).y)&&((a).z == (b).z))
#define f4equals(a, b) (((a).x == (b).x)&&((a).y == (b).y)&&((a).z == (b).z)&&((a).w == (b).w))

#define fvec3_normal(u, v, w) (f3normalized(f3cross(f3dif(u, v), f3dif(u, w))))

fvec2_t f2normalized(fvec2_t v);
fvec3_t f3normalized(fvec3_t v);
fvec4_t f4normalized(fvec4_t v);

void    normalizef2(fvec2_t *v);
void    normalizef3(fvec3_t *v);
void    normalizef4(fvec4_t *v);


fvec3_t f3matmult(m3x3_t m, fvec3_t v);
fvec4_t f4matmult(m4x4_t m, fvec4_t v);

fvec3_t project(fvec3_t v, m4x4_t pm);

void fvec2_print(fvec2_t v);
void fvec3_print(fvec3_t v);
void fvec4_print(fvec4_t v);

#endif /* _LA_VECTOR_H_ */
