
#include "font.h"
#include "GC_component.h"
#include "../util/assert.h"
#include "../graphics/graphics.h"
#include "GC_listener.h"
#include <sdl/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>


#define BORDER_WIDTH 3

#define BTN_FPALETTE(_p) GC_PALETTE((_p).fg, (_p).bg, COLOR_SCALE((_p).border, 1.2))
#define BTN_HPALETTE(p) GC_PALETTE(p.fg, COLOR_SCALE(p.bg, 0.8), p.border)
#define BTN_PPALETTE(p) GC_PALETTE(COLOR_SCALE(p.bg, 0.8), \
  COLOR_SCALE(p.border, 1.2), COLOR_SCALE(p.bg, 0.8))

#define BTN_TOGGLE_HPALETTE(_p) GC_PALETTE(GREY, (_p).bg, (_p).border)
#define BTN_TOGGLE_TPALETTE(_p) GC_PALETTE(BLUE, (_p).bg, (_p).border)




/**
 * bg = SDLRECT(x, y, width, height);
 * SET_COLOR(rndr, r, g, b);
 * SDL_RenderFillRect(rndr, &bg);
 **/

GC_text create_default_text(u32_t height, const char * text)
{
  GC_text res;
  u32_t font_height;
  u32_t font_width;

  extern const u8_t _FONT5X7[98][7][5];

  res.str = malloc(GC_MAX_TEXT_LENGTH * sizeof(char));
  strcpy(res.str, text);

  font_height = (u32_t)((float)height * 0.6);
  font_width  = (u32_t)((float)font_height * (5.0f/7.0f));
  res.font    = load_font(_FONT5X7, 98, font_width, font_height);
  res.x       = height*0.2;
  res.y       = height*0.2;
  res.length  = strlen(text);
  return res;
}

bool GC_contains(GC_component * self, i32_t x, i32_t y)
{
  return x >= self->x && x <= (self->x+self->width) &&
         y >= self->y && y <= (self->y+self->height);
}

void delay(f64_t s)
{
  i32_t millis;
  clock_t start;

  millis = (i32_t)(100000.0 * s);
  start = clock();
  while (clock() < start + millis);
}

void * cursor_blink_func(void* cmp)
{
    pthread_detach(pthread_self());

    GC_component * c;
    i32_t millis;
    clock_t start;

    c = (GC_component *)cmp;

    while(c->focused) {
      delay(5);
      c->blink = !c->blink;
    }
    c->blink = true;
    pthread_exit(NULL);
}


void trigger_onclick(GC_component * c, GC_event evt)
{
  i32_t i;
  GC_lnode * n;

  n = c->listeners->next;
  for(i = 0; i < c->listener_count; i++, n = n->next) {
    if(n->listener.type == GC_CLICKED) n->listener.listener_func(evt, c);
  }
}

void hover_listener_func(GC_event evt, void* cmp)
{
  GC_component * c;
  c = (GC_component *)cmp;
  c->hovered = GC_contains(c, evt.evt.motion.x, evt.evt.motion.y);
}

void pressed_listener_func(GC_event evt, void* cmp)
{
  GC_component * c;
  pthread_t blink;

  c = (GC_component *)cmp;
  c->pressed = GC_contains(c, evt.evt.motion.x, evt.evt.motion.y);
  if(c->type == GC_TEXT_FIELD && !c->focused && c->pressed) {
    pthread_create(&blink, NULL, &cursor_blink_func, c);
  }
  c->focused = c->pressed;
}

void released_listener_func(GC_event evt, void* cmp)
{
  GC_component * c;

  c = (GC_component *)cmp;

  if(c->pressed && GC_contains(c, evt.evt.motion.x, evt.evt.motion.y)){
    trigger_onclick(c, evt);
    c->toggled = !c->toggled;
  }
  c->pressed = false;
  return;
}


void key_pressed_listener_func(GC_event evt, void* cmp)
{
  GC_component * c;

  c = (GC_component *)cmp;
  if (!c->focused) return;
  switch (evt.evt.key.keysym.sym) {
    case SDLK_BACKSPACE:
      if(c->text.length != 0 && c->cursor_pos != 0) {
        c->text.length--;
        c->cursor_pos--;
        memmove(c->text.str+c->cursor_pos, c->text.str+c->cursor_pos+1, (c->text.length-c->cursor_pos+1)+1);
        c->blink = true;
      }
      break;
    case SDLK_LEFT:
      if(c->cursor_pos != 0) c->cursor_pos--;
      c->blink = true;
      break;
    case SDLK_RIGHT:
      if(c->cursor_pos < c->text.length) c->cursor_pos++;
      c->blink = true;
      break;
    case SDLK_RETURN:
      c->focused = false;
      break;
  }
}

void text_input_listener_func(GC_event evt, void* cmp)
{
  GC_component * c;
  i32_t len;
  c = (GC_component *)cmp;
  if (!c->focused) return;
  if ((c->text.length+1) > GC_MAX_TEXT_LENGTH) return;
  len = strlen(evt.evt.text.text);
  memmove(c->text.str+c->cursor_pos+len, c->text.str+c->cursor_pos, (c->text.length-c->cursor_pos)+1);
  memmove(c->text.str+c->cursor_pos, evt.evt.text.text, len);
  c->text.length += len;
  c->cursor_pos++;
  c->blink = true;
}

static const GC_listener hover_listener = (GC_listener){
    .type = GC_MOUSE_MOTION,
    .listener_func = hover_listener_func
};
static const GC_listener pressed_listener = (GC_listener){
    .type = GC_MOUSE_PRESSED,
    .listener_func = pressed_listener_func
};
static const GC_listener released_listener = (GC_listener){
    .type = GC_MOUSE_RELEASED,
    .listener_func = released_listener_func
};
static const GC_listener text_input_listener = (GC_listener){
    .type = GC_TEXT_INPUT,
    .listener_func = text_input_listener_func
};
static const GC_listener key_pressed_listener = (GC_listener){
    .type = GC_KEY_PRESSED,
    .listener_func = key_pressed_listener_func
};

void GC_draw_text(SDL_Renderer * rndr, GC_text t, i32_t x, i32_t y)
{
  draw_string(rndr, t.font, t.str, t.x + x, t.y + y);
}
void GC_draw_cursor(SDL_Renderer * rndr, GC_text t, i32_t _x, i32_t _y, i32_t cp)
{
  SDL_RenderDrawLine(rndr, _x+ (cp)*(t.font.width+7) + 6, _y +5, _x + (cp)*(t.font.width+7) +6, _y + t.font.height +15);
  SDL_RenderDrawLine(rndr, _x+ (cp)*(t.font.width+7) + 7, _y +5, _x + (cp)*(t.font.width+7) +7, _y + t.font.height +15);
}

void set_pixel(SDL_Renderer* rndr, color_t c, i32_t x, i32_t y)
{
  SET_COLOR(rndr, c);
  SDL_RenderDrawPoint(rndr, x, y);
}

GC_palette GC_get_drawpalette(GC_component * c)
{
  if(c->pressed) return c->pressed_palette;
  if(c->toggled) return c->toggled_palette;
  if(c->hovered) return c->hovered_palette;
  if(c->focused) return c->focused_palette;
  return c->palette;
}



void GC_button_drawfunc(SDL_Renderer * rndr, GC_component * c)
{
  GC_palette p;

  p = GC_get_drawpalette(c);
  switch(c->border_style) {
    case GC_NO_BORDER:
      FILL_RECT(rndr, p.bg, c->x, c->y, c->width, c->height);
      break;
    case GC_LINE_BORDER:
      FILL_RECT(rndr, p.border, c->x, c->y, c->width, c->height);
      FILL_RECT(rndr, p.bg, (c->x + BORDER_WIDTH), (c->y + BORDER_WIDTH),
        (c->width - (2*BORDER_WIDTH)), (c->height - (2*BORDER_WIDTH)));
      break;
  }
  SET_COLOR(rndr, p.fg);
  GC_draw_text(rndr, c->text, c->x, c->y);
  return;
}

void GC_tbutton_drawfunc(SDL_Renderer * rndr, GC_component * c)
{
  GC_palette p;
  i32_t cx, cy;

  cy = c->y + (c->height/2);
  cx = c->x + c->width - c->radius - (c->width*0.1);

  p = GC_get_drawpalette(c);
  switch(c->border_style) {
    case GC_NO_BORDER:
      fill_circle(rndr, p.bg, cx, cy, c->radius);
      break;
    case GC_LINE_BORDER:
      fill_circle(rndr, p.border, cx, cy, c->radius);
      fill_circle(rndr, p.bg, cx, cy, (i32_t)((f64_t)c->radius * 0.8f));
      break;
  }
  if(c->toggled)
    fill_circle(rndr, p.fg, cx, cy, (i32_t)((f64_t)c->radius * 0.7f));
  else if (c->pressed || c->hovered)
    fill_circle(rndr, p.fg, cx, cy, (i32_t)((f64_t)c->radius * 0.5f));

  SET_COLOR(rndr, p.border);
  GC_draw_text(rndr, c->text, c->x, c->y);
  return;
}

void GC_label_drawfunc(SDL_Renderer * rndr, GC_component * c)
{
  GC_palette p;

  p = GC_get_drawpalette(c);
  switch(c->border_style) {
    case GC_NO_BORDER:
      FILL_RECT(rndr, p.bg, c->x, c->y, c->width, c->height);
      break;
    case GC_LINE_BORDER:
      FILL_RECT(rndr, p.border, c->x, c->y, c->width, c->height);
      FILL_RECT(rndr, p.bg, (c->x + BORDER_WIDTH), (c->y + BORDER_WIDTH),
        (c->width - (2*BORDER_WIDTH)), (c->height - (2*BORDER_WIDTH)));
      break;
  }
  SET_COLOR(rndr, p.fg);
  GC_draw_text(rndr, c->text, c->x, c->y);
  return;
}

void GC_text_field_drawfunc(SDL_Renderer * rndr, GC_component * c)
{
  GC_palette p;

  p = GC_get_drawpalette(c);
  switch(c->border_style) {
    case GC_NO_BORDER:
      FILL_RECT(rndr, p.bg, c->x, c->y, c->width, c->height);
      break;
    case GC_LINE_BORDER:
      FILL_RECT(rndr, p.border, c->x, c->y, c->width, c->height);
      FILL_RECT(rndr, p.bg, (c->x + BORDER_WIDTH), (c->y + BORDER_WIDTH),
        (c->width - (2*BORDER_WIDTH)), (c->height - (2*BORDER_WIDTH)));
      break;
  }
  SET_COLOR(rndr, p.fg);
  GC_draw_text(rndr, c->text, c->x, c->y);
  if(c->blink && c->focused)
    GC_draw_cursor(rndr, c->text, c->x, c->y, c->cursor_pos);
  return;
}


GC_component GC_create_button(i32_t x, i32_t y, u32_t width, u32_t height, const char * text, GC_palette p)
{
  GC_component res;

  res = GC_CREATE(GC_BUTTON, x, y, width, height, text, p, BTN_FPALETTE(p),
    BTN_HPALETTE(p),
    BTN_PPALETTE(p),p);
  GC_add_listener(&res, hover_listener);
  GC_add_listener(&res, pressed_listener);
  GC_add_listener(&res, released_listener);
  res.draw_func = GC_button_drawfunc;

  return res;
}

GC_component GC_create_toggle_button(i32_t x, i32_t y,u32_t width, u32_t height, u32_t radius, const char * text, GC_palette p)
{
  GC_component res;

  res = GC_CREATE(GC_BUTTON, x, y, width, height, text, p, BTN_FPALETTE(p),
    BTN_TOGGLE_HPALETTE(p),
    BTN_TOGGLE_TPALETTE(p),BTN_TOGGLE_TPALETTE(p));
  GC_add_listener(&res, hover_listener);
  GC_add_listener(&res, pressed_listener);
  GC_add_listener(&res, released_listener);
  res.radius = radius;
  res.draw_func = GC_tbutton_drawfunc;

  return res;
}

GC_component GC_create_label(i32_t x, i32_t y, u32_t width, u32_t height, const char * text, GC_palette p)
{
  GC_component res;

  res = GC_CREATE(GC_LABEL, x, y, width, height, text, p, p, p, p, p);
  res.border_style = GC_NO_BORDER;
  res.draw_func = GC_label_drawfunc;
  return res;
}

GC_component GC_create_panel(i32_t x, i32_t y, u32_t width, u32_t height, GC_palette p)
{
  return GC_CREATE(GC_PANEL, x, y, width, height, "", p, p, p, p, p);
}

GC_component GC_create_textfield(i32_t x, i32_t y, u32_t width, u32_t height, const char * text, GC_palette p)
{
  GC_component res;

  res = GC_CREATE(GC_TEXT_FIELD, x, y, width, height, text, p, p, p, p, p);
  res.draw_func = GC_text_field_drawfunc;
  res.cursor_pos = strlen(text);
  GC_add_listener(&res, hover_listener);
  GC_add_listener(&res, pressed_listener);
  GC_add_listener(&res, released_listener);
  GC_add_listener(&res, text_input_listener);
  GC_add_listener(&res, key_pressed_listener);
  return res;
}

void GC_add_listener(GC_component * c, GC_listener l)
{
  GC_lnode * tmp;
  if (c->listeners == NULL) {
    c->listeners = new_lnode(l);
    c->listeners->next = c->listeners;
    c->listener_count++;
    return;
  }
  tmp = c->listeners->next;
  c->listeners->next = new_lnode(l);
  c->listeners->next->next = tmp;
  c->listeners = c->listeners->next;
  c->listener_count++;
}
void GC_add_onclick(GC_component * component, void (*onclick)(GC_event, void*))
{
  GC_add_listener(component, (GC_listener){.type=GC_CLICKED, .listener_func=onclick});
}

void GC_draw_component(SDL_Renderer * rndr, GC_component * c)
{
  c->draw_func(rndr, c);
}


void GC_destroy_component(GC_component * c)
{
  i32_t i;
  GC_lnode * tmp;

  if (c->text.str != NULL) free(c->text.str);
  if (c->listeners != NULL) {

    tmp = c->listeners;

    for(i = 0; i < c->listener_count; i++) {
      tmp = c->listeners->next;
      free(c->listeners);
      c->listeners = tmp;
    }
  }
}
