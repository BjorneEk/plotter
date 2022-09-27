/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * GUI_context header file
 *
 *==========================================================*/

#ifndef _GUI_CONTEXT_H_
#define _GUI_CONTEXT_H_

#include "GC_component.h"
#include <sdl/SDL.h>

typedef struct component_list {

  GC_component * c;

  struct component_list * next;

} GC_node;

typedef struct context {

  bool shown;

  i32_t x, y, w, h;

  color_t color;

  SDL_Window * window;

  SDL_Renderer * renderer;

  i32_t count;

  GC_node * components;

} GC_context;


GC_context GC_new_ctx(SDL_Window * window, SDL_Renderer * renderer, color_t c,
  i32_t x, i32_t y, i32_t w, i32_t h);

void GC_add(GC_context * self, GC_component * c);

void GC_trigger_event(GC_context * self, SDL_Event event);

void GC_draw(GC_context * self);

void GC_destroy(GC_context * self);

#endif /* _GUI_CONTEXT_H_ */
