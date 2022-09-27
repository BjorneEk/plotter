/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * font header file
 *
 *==========================================================*/

#ifndef _FONT_H_
#define _FONT_H_

#include <sdl/SDL.h>
#include "../util/types.h"
#define FONT_OFFSET 33
#define FONT_QUESTIONMARK 33
#define FONT_QUESTIONMARK 33
#define FONT_INTERNATIONAL 104



typedef struct font7x5 {
  u8_t * font;

  f64_t x_scale;
  f64_t y_scale;

  i32_t width;
  i32_t height;

  size_t length;
} font_t;

typedef struct font_bitmap {
  u8_t bm[7][5];
} font_bitmap_t;

font_t load_font(const u8_t * font, i64_t len, i32_t width, i32_t height);

void draw_char(SDL_Renderer * rndr, font_t f, i32_t ch, i32_t* _x, i32_t * _y);

void draw_string(SDL_Renderer * rndr, font_t f, char * str, i32_t _x, i32_t y);

#endif /* _FONT_H_ */
