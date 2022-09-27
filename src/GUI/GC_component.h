/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * GUI_component header file
 *
 *==========================================================*/

#ifndef _GUI_COMPONENT_H_
#define _GUI_COMPONENT_H_

#include "../util/types.h"
#include "GC_listener.h"
#include "../graphics/graphics.h"
#include "font.h"
#include "string.h"

#define GC_MAX_TEXT_LENGTH 200

#define GC_TEXT(_x, _y, _str, _font) \
  (GC_text){.x=(_x),.y=(_y), .length = strlen((_str)),.str=(_str),.font=(_font)}

#define GC_PALETTE(_fg, _bg, _brdr) (GC_palette){.fg=(_fg),.bg=(_bg),.border=(_brdr)}

#define PALETTE_EQ(p1, p2) COLOR_EQ((p1).fg, (p2).fg) && \
                           COLOR_EQ((p1).bg, (p2).bg) && \
                           COLOR_EQ((p1).border, (p2).border)

typedef enum component_type {
  GC_BUTTON,
  GC_TOGGLE_BUTTON,
  GC_LABEL,
  GC_TEXT_FIELD,
  GC_PANEL
} GC_type;

typedef enum border_style {
  GC_LINE_BORDER,
  GC_NO_BORDER
} GC_border;

/**
 * text struct containing information used
 * in a single text display
 **/
typedef struct text {
  i32_t x, y;
  u32_t length;
  char*   str;
  font_t  font;
} GC_text;

/**
 * standard color palette of a component
 **/
typedef struct GC_palette {
  color_t fg;
  color_t bg;
  color_t border;
} GC_palette;


/**
 * a component struct containing all information
 * tied to a component
 **/
typedef struct component {

  /**
   * the component type
   **/
  GC_type type;

  /**
   * the x and y coordinated of the component
   **/
  i32_t x, y;

  /**
   * the size of the component
   **/
  u32_t width, height, radius;

  /**
   * the position of the cursor in the text of the component
   * unly used by components that take text input
   **/
  i32_t cursor_pos;

  /**
   * the border stype of the component
   **/
  GC_border border_style;

  /**
   * the default color palette used py the component
   **/
  GC_palette palette;

  /**
   * the color palettes used py the component
   * when it is in the diferent states:
   * focused, hovered, pressed and toggled.
   * used to create visual feedback tied to these states.
   **/
  GC_palette focused_palette;
  GC_palette hovered_palette;
  GC_palette pressed_palette;
  GC_palette toggled_palette;

  /**
   * the component text
   **/
  GC_text text;

  /**
   * state flaggs of the component.
   *
   * blink is toggled on and of whith time delay
   * from a separate thread when focused in order
   * to create a blinking cursor.
   **/
  struct {
    u8_t focused : 1;
    u8_t hovered : 1;
    u8_t pressed : 1;
    u8_t toggled : 1;
    u8_t blink   : 1;
  };

  u32_t listener_count;

  GC_lnode * listeners;

  void (*draw_func)(SDL_Renderer *, struct component*);

} GC_component;

#define GC_CREATE(_t, _x, _y, _w, _h, _txt, _p, _pf, _ph, _pp, _pt) \
  (GC_component){                                                   \
    .type             = (_t),                                       \
    .text             = create_default_text((_h), (_txt)),          \
    .cursor_pos       = strlen((_txt)),                             \
    .listener_count   = 0,                                          \
    .x                = (_x),                                       \
    .y                = (_y),                                       \
    .width            = (_w),                                       \
    .height           = (_h),                                       \
    .palette          = (_p),                                       \
    .focused_palette  = (_pf),                                      \
    .hovered_palette  = (_ph),                                      \
    .pressed_palette  = (_pp),                                      \
    .toggled_palette  = (_pt),                                      \
    .border_style     = GC_LINE_BORDER,                             \
    .focused          = false,                                      \
    .hovered          = false,                                      \
    .pressed          = false,                                      \
    .toggled          = false,                                      \
    .blink            = false,                                      \
    .listeners        = NULL}

GC_component GC_create_button(
  i32_t x,
  i32_t y,
  u32_t width,
  u32_t height,
  const char * text,
  GC_palette palett);

GC_component GC_create_toggle_button(
  i32_t x,
  i32_t y,
  u32_t width,
  u32_t height,
  u32_t radius,
  const char * text,
  GC_palette p);

GC_component GC_create_label(
  i32_t x,
  i32_t y,
  u32_t width,
  u32_t height,
  const char * text,
  GC_palette palett);

GC_component GC_create_textfield(
  i32_t x,
  i32_t y,
  u32_t width,
  u32_t height,
  const char * text,
  GC_palette palett);

GC_component GC_create_panel(
  i32_t x,
  i32_t y,
  u32_t width,
  u32_t height,
  GC_palette palett);

void GC_add_listener(GC_component * component, GC_listener listener);

void GC_add_onclick(GC_component * component, void (*onclick)());

void GC_draw_component(SDL_Renderer * rndr, GC_component * c);

void GC_destroy_component(GC_component * c);

#endif /* _GUI_COMPONENT_H_ */
