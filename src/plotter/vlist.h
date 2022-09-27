/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * vlist header file,
 *
 *==========================================================*/
#ifndef _VLIST_H_
#define _VLIST_H_

#include "../graphics/graphics.h"
#include "../vec/vec.h"

typedef struct vlist {
  u32_t length;
  union {
    fvec3_t *vecs;
    f64_t *vals;
  };
}vlist_t;

vlist_t read_vlist(const char * filename, char delim);

void plot_vlist(SDL_Renderer * rndr, vlist_t vl, f64_t scale, fvec3_t pos, i32_t psize, color_t c);



#endif /* _VLIST_H_ */
