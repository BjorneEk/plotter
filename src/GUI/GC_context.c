/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * GUI_context functions
 *
 *==========================================================*/


#include "GC_context.h"
#include "../util/assert.h"
#include "../graphics/graphics.h"
GC_node * GC_create_node(GC_component * c)
{
  GC_node * res;

  res = malloc(sizeof(GC_node));
  assert(res != NULL, "Out of Memory (GC_create_node)");
  res->c = c;
  res->next = NULL;
  return res;
}

GC_context GC_new_ctx(SDL_Window * window, SDL_Renderer * renderer, color_t c,
  i32_t x, i32_t y, i32_t w, i32_t h)
{
  GC_context res;

  res.x                = x;
  res.y                = x;
  res.w                = w;
  res.h                = h;
  res.color            = c;
  res.renderer         = renderer;
  res.window           = window;
  res.count            = 0;
  res.shown            = true;
  res.components       = NULL;
  return res;
}

void GC_add(GC_context * self, GC_component * c)
{
  GC_node * tmp;

  if(self->components == NULL) {
    self->components = GC_create_node(c);
    self->components->next = self->components;
    self->count++;
    return;
  }
  tmp = self->components->next;
  self->components->next = GC_create_node(c);
  self->components->next->next = tmp;
  self->components = self->components->next;
  self->count++;
}

GC_event get_GC_event(SDL_Event e) {
  GC_event res;
  res.evt = e;
  switch (e.type) {
    case SDL_MOUSEBUTTONDOWN:
      res.type = GC_MOUSE_PRESSED;  break;
    case SDL_MOUSEBUTTONUP:
      res.type = GC_MOUSE_RELEASED; break;
    case SDL_MOUSEMOTION:
      res.type = GC_MOUSE_MOTION;   break;
    case SDL_TEXTINPUT:
      res.type = GC_TEXT_INPUT;     break;
    case SDL_KEYDOWN:
      res.type = GC_KEY_PRESSED;    break;
    case SDL_KEYUP:
      res.type = GC_KEY_RELEASED;   break;
    case SDL_MOUSEWHEEL:
      res.type = GC_MOUSE_SCROLLED; break;
    default:
      res.type = GC_NO_EVENT;       break;
  }
  return res;
}


void GC_trigger_event(GC_context * self, SDL_Event event)
{
  GC_event ev;
  GC_node * cn;
  GC_lnode * ln;
  i32_t i, j;

  if(!self->shown) return;

  ev = get_GC_event(event);
  cn = self->components->next;
  for(i = 0; i < self->count; i++, cn = cn->next) {
    if (cn->c->listener_count > 0) {
    ln = cn->c->listeners->next;
      for(j = 0; j < cn->c->listener_count; j++, ln = ln->next) {
        if(ln->listener.type == ev.type) ln->listener.listener_func(ev, cn->c);
      }
    }
  }
}

void GC_draw(GC_context * self)
{
  i32_t i;
  GC_node * n;

  if (!self->shown) return;
  FILL_RECT(self->renderer, self->color, self->x, self->y, self->w, self->h);
  n = self->components->next;
  for(i = 0; i < self->count; i++, n = n->next) {
    GC_draw_component(self->renderer, n->c);
  }
  return;
}

void GC_destroy(GC_context * self)
{
  GC_node * tmp;
  if(self->components != NULL) {
    tmp = self->components;
    while(tmp != NULL && self->components != NULL) {
      self->components = self->components->next;
      GC_destroy_component(tmp->c);
      free(tmp);
      tmp = self->components;
    }
  }
}
