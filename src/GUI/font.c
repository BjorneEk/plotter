/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * font functions
 *
 *==========================================================*/
#include <sdl/SDL.h>
#include "font.h"
#include "../util/types.h"



#define CEIL(a) ((((i32_t)a) < 0.5f) ? ((i32_t)a) : (((i32_t)a)+1))
extern const u8_t FONT5X7[50][7][5];



font_t load_font(const u8_t * restrict font, i64_t len, i32_t width, i32_t height)
{
  font_t res;

  res.font   = malloc(len * 200 * sizeof(u8_t));
  res.length = len;
  memcpy(res.font, font, len * 35);

  res.x_scale = 5.0f / (f64_t)width;
  res.y_scale = 7.0f / (f64_t)height;
  res.width = width;
  res.height = height;
  return res;
}

u8_t * get_fontchar(i32_t c, font_t f)
{
  u8_t * res = (u8_t*)malloc(35);
  switch(c) {
    case 0x8f: memcpy(res, (void*)(f.font + (35 * (FONT_INTERNATIONAL    ))), 35*sizeof(u8_t)); break;
    case 0x8e: memcpy(res, (void*)(f.font + (35 * (FONT_INTERNATIONAL + 1))), 35*sizeof(u8_t)); break;
    case 0x99: memcpy(res, (void*)(f.font + (35 * (FONT_INTERNATIONAL + 2))), 35*sizeof(u8_t)); break;
    case 0x86: memcpy(res, (void*)(f.font + (35 * (FONT_INTERNATIONAL + 3))), 35*sizeof(u8_t)); break;
    case 0x84: memcpy(res, (void*)(f.font + (35 * (FONT_INTERNATIONAL + 4))), 35*sizeof(u8_t)); break;
    case 0x94: memcpy(res, (void*)(f.font + (35 * (FONT_INTERNATIONAL + 5))), 35*sizeof(u8_t)); break;
    default:
      if((c - FONT_OFFSET) >= f.length || (c - FONT_OFFSET) < 0)
        memcpy(res, (void*)(f.font + (35 * FONT_QUESTIONMARK)), 35*sizeof(u8_t));
      memcpy(res, (void*)(f.font + (35 * (c - FONT_OFFSET))), 35*sizeof(u8_t));
  }
  return res;
}

void draw_char(SDL_Renderer * rndr, font_t f, i32_t ch, i32_t * _x, i32_t * _y)
{
  f64_t x, y;
  if(ch == ' ') {
    (*_x) += f.width;
    return;
  }
  u8_t * c = get_fontchar(ch, f);
  for (x = 0; x < f.width; x++)
    for (y = 0; y < f.height; y++)
      if(*(c+((i32_t)(x * f.x_scale))+(5*((i32_t)(y * f.y_scale)))))
        SDL_RenderDrawPoint(rndr, (*_x) + x, (*_y) + y);
  (*_x) += f.width;
  free(c);
}

void draw_string(SDL_Renderer * rndr, font_t f, char * str, i32_t _x, i32_t _y)
{
  i32_t x;

  x = _x;
  for (char * c = str; *c != '\0'; c++) {
    draw_char(rndr, f, *c, &x, &_y);
    x += 7;
  }
}
