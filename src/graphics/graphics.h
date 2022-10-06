/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * graphics header file
 *
 *==========================================================*/

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <sdl/SDL.h>
#include "../vec/vec.h"
#include "../util/types.h"
#include "color.h"

#define SDLRECT(_x, _y, _w, _h) (SDL_Rect){.x = (_x), .y = (_y), .w = (_w), .h = (_h)}

#define SET_COLOR(_rndr, _clr) SDL_SetRenderDrawColor((_rndr), (_clr).r, (_clr).g, (_clr).b, 1)

#define FILL_RECT(__rndr, __clr, __x, __y, __w, __h)                 \
  SET_COLOR((__rndr), (__clr));                                      \
  SDL_RenderFillRect((__rndr), &SDLRECT((__x), (__y), (__w), (__h)))

#define SET_PIXEL(__rndr, __clr, _x, _y)   \
  SET_COLOR((__rndr), (__clr));            \
  SDL_RenderDrawPoint((__rndr), (_x), (_y))

void fill_circle(SDL_Renderer* rndr, color_t c, i32_t _x, i32_t _y, i32_t rad);

void draw_xyz(SDL_Renderer* rndr, fvec3_t _O, fvec3_t _X, fvec3_t _Y, fvec3_t _Z, fvec3_t pos, f64_t scale);

void fill_sphere(SDL_Renderer* rndr, color_t c, i32_t _x, i32_t _y, i32_t rad);

#endif /* _GRAPHICS_H_ */
