/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * vlist functions
 *
 *==========================================================*/
#include "../util/assert.h"
#include "vlist.h"
#include "../util/IO.h"
#include <sdl/SDL.h>


#define WARN_NDIV_TRIM "Input file data count was not divisible by 3, triming excess values"
i32_t nfloats(char * buff, char delim)
{
  char * c;
  i32_t res;

  for(c=buff,res=0;*c!='\0';c++,res+=(*c==delim));
  if(*--c==delim)res--;
  return res;
}

vlist_t read_vlist(const char * filename, char delim)
{
  u32_t blen;
  i32_t fcnt, n, pf;
  char * buff, *c, *cc;
  vlist_t res;
  char tmp[100];

  buff = read_file(filename);

  fcnt = nfloats(buff, delim);

  res.length = (fcnt /3);
  assert_warn(!(fcnt%3), WARN_NDIV_TRIM);

  res.vals = malloc(res.length * sizeof(fvec3_t));
  assert(res.vals != NULL, "Out of memory(readi_vlist)");


  for(pf=0,c=buff;*c!='\0'&&pf<(res.length*3);c++) {
    if(*c!=' ' && *c!=delim){
      for(n=0;*(c+n)!=' '&&*(c+n)!=delim;n++);
      strncpy(tmp, c, n);
      tmp[n+1] = '\0';
      res.vals[pf++] = atof(tmp);
      c+=n;
    }
  }
  free(buff);
  return res;
}

void plot_vlist(SDL_Renderer * rndr, vlist_t vl, f64_t scale, fvec3_t pos, i32_t psize, color_t c)
{
  i32_t i;
  fvec3_t v;
  for(i = 0; i < vl.length; i++) {
    v = fvec3Sop(vl.vecs[i], scale, *);

    vec3opR(v, pos, +=);
    fill_circle(rndr, c, v.x, v.y, psize);
  }
}

void plot_vlist_perspective(SDL_Renderer * rndr, vlist_t vl,
    f64_t scale, fvec3_t pos, i32_t psize, color_t c)
{
  i32_t i;
  fvec3_t v;
  for(i = 0; i < vl.length; i++) {
    v = fvec3Sop(vl.vecs[i], scale, *);

    vec3opR(v, pos, +=);
    fill_sphere(rndr, c, v.x, v.y, psize);
  }
}
