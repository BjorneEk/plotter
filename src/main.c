

////////////////////////////////////////////////////////////////////////////
///        @author Gustaf Franzén :: https://github.com/BjorneEk;        ///
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
///        @author Gustaf Franzén :: https://github.com/BjorneEk;        ///
////////////////////////////////////////////////////////////////////////////


#include <sdl/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "util/assert.h"
#include "GUI/GC_context.h"
#include "graphics/graphics.h"
#include "plotter/vlist.h"
#include "vec/vec.h"
#include "vec/matrix.h"

#define DEFAULT_WIDTH  1820
#define DEFAULT_HEIGHT 980
#define WINDOW_TITLE "Plotter"
#define DEFAULT_DELIMITOR ','


GC_context settings, info;
GC_component exit_btn, txt_field, toggle, title, settings_btn,
update_psize, tx_red, tx_green, tx_blue, clr_btn;


i32_t HEIGHT, WIDTH;

/**
 * the coordinate axles, orgin, and position on screen
 **/
fvec3_t O, X, Y, Z, pos;

f64_t system_scale;

/**
 * global variables that are true if the key is down
 **/
bool MOUSE1, MOUSE2, SHIFT;

/**
 * previous mouse coordinates
 **/
i64_t pmx, pmy;

/**
 * the list containing all the points
 **/
vlist_t vlist;

/**
 * the point size in  pixels
 **/
i32_t psize;

bool show_axles;

color_t pcolor;


void init_default_globals()
{
  O = FVEC3(0.0,0.0,0.0);
  X = FVEC3(1.0,0.0,0.0);
  Y = FVEC3(0.0,1.0,0.0);
  Z = FVEC3(0.0,0.0,1.0);
  system_scale = 500.0;
  MOUSE1 = false;
  MOUSE2 = false;
  SHIFT  = false;
  HEIGHT = DEFAULT_HEIGHT;
  WIDTH  = DEFAULT_WIDTH;
  pos = FVEC3(WIDTH/2.0 -(system_scale/2),HEIGHT/2.0+(system_scale/2),1.0);
  pmx=0;
  pmy=0;
  psize = 1;
  show_axles = true;
  pcolor = COLOR(0,100,100);
}

void init_window(SDL_Window ** window, SDL_Renderer ** renderer)
{
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, window, renderer);

  assert(*window != NULL && *renderer != NULL,"Failed to initialize SDL2 window");

  SDL_SetWindowTitle(*window, WINDOW_TITLE);
  SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 0);
  SDL_RenderClear(*renderer);
  SDL_RenderPresent(*renderer);
}

void rotate_x(f64_t ang)
{
  i32_t i;

  O = f3matmult(RX3X3(ang), O);
  X = f3matmult(RX3X3(ang), X);
  Y = f3matmult(RX3X3(ang), Y);
  Z = f3matmult(RX3X3(ang), Z);
  for(i = 0; i < vlist.length; i++)
    vlist.vecs[i] = f3matmult(RX3X3(ang), vlist.vecs[i]);
}

void rotate_y(f64_t ang)
{
  i32_t i;

  O = f3matmult(RY3X3(ang), O);
  X = f3matmult(RY3X3(ang), X);
  Y = f3matmult(RY3X3(ang), Y);
  Z = f3matmult(RY3X3(ang), Z);
  for(i = 0; i < vlist.length; i++)
    vlist.vecs[i] = f3matmult(RY3X3(ang), vlist.vecs[i]);
}

bool event_loop(SDL_Event * event)
{
  while (SDL_PollEvent(event)) {
    GC_trigger_event(&settings, *event);
    GC_trigger_event(&info, *event);

    switch (event->type) {
      case SDL_MOUSEBUTTONDOWN:
        switch (event->button.button) {
          case SDL_BUTTON_LEFT:  MOUSE1 = true; break;
          case SDL_BUTTON_RIGHT: MOUSE2 = true; break;
          default:               break;
        } break;

      case SDL_MOUSEBUTTONUP:
        switch (event->button.button) {
          case SDL_BUTTON_LEFT:  MOUSE1 = false; break;
          case SDL_BUTTON_RIGHT: MOUSE2 = false; break;
          default:               break;
        } break;

      case SDL_MOUSEMOTION:
        if (MOUSE1 || MOUSE2) {
            if(SHIFT) {
              rotate_y(-(event->motion.x - pmx)/300.0);
              rotate_x( (event->motion.y - pmy)/300.0);
            }
            else {
              pos.x += (event->motion.x - pmx);
              pos.y += (event->motion.y - pmy);
            }
          }
          pmx = event->motion.x;
          pmy = event->motion.y;
        break;

      case SDL_MOUSEWHEEL:
          if(event->wheel.y > 0)      system_scale += 10;
          else if(event->wheel.y < 0) system_scale -= 10;
          break;

      case SDL_KEYDOWN:
        if(event->key.keysym.sym == SDLK_ESCAPE && !settings.shown)
          settings.shown = true;
        else if (event->key.keysym.sym == SDLK_ESCAPE)
          settings.shown = false;
        if (event->key.keysym.sym == SDLK_LSHIFT) SHIFT = true;
        break;

      case SDL_KEYUP:
        if (event->key.keysym.sym == SDLK_LSHIFT) SHIFT = false;
        break;

      case SDL_QUIT: return true; break;
    }
  }
  return false;
}

void main_loop(SDL_Event * event)
{
  bool quit;

  quit = false;
  while (!quit) {
    SDL_SetRenderDrawColor(settings.renderer, 176, 189, 112, 0);
    SDL_RenderClear(settings.renderer);
    SDL_SetRenderDrawColor(settings.renderer, 0, 0, 0, 0);
    if(show_axles) draw_xyz(settings.renderer, O, X, Y, Z, pos, 5000.0f);
    plot_vlist(settings.renderer, vlist, system_scale, pos, psize, pcolor);

    GC_draw(&info);
    GC_draw(&settings);

    SDL_RenderPresent(settings.renderer);
    quit = event_loop(event);
  }
}

void quit()
{
  GC_destroy(&settings);
  SDL_DestroyRenderer(settings.renderer);
  SDL_DestroyWindow(settings.window);
  SDL_Quit();
  exit(0);
}

void toggle_info(GC_event evt, void* cmp)
{
  GC_component * c;

  c = (GC_component *)cmp;
  show_axles = !c->toggled;
}

void set_psize(GC_event evt, void* cmp)
{
  GC_component * c;

  psize = atoi(txt_field.text.str);
}

void update_color(GC_event evt, void* cmp)
{
  GC_component * c;

  pcolor.r = atoi(tx_red.text.str);
  pcolor.g = atoi(tx_green.text.str);
  pcolor.b = atoi(tx_blue.text.str);
}

void show_settings(GC_event evt, void* cmp)
{
  settings.shown = true;
}

void exit_onclick(GC_event evt, void* component)
{
  quit();
}

int main(int argc, char * args[]) {
  SDL_Window   * window   = NULL;
  SDL_Renderer * renderer = NULL;
  char * filename;
  i32_t i;
  char delim;
  SDL_Event event;

  delim = DEFAULT_DELIMITOR;
  init_default_globals();

  assert(!(argc < 2), "invalid argument count, enter a csv file with coordinates");

  if (!strcmp(args[1], "--help")) {
    printf("Usage:\n");
    printf("%s <filename>\n", args[0]);
    printf("Options:\n");
    printf("  <\033[1;33m-d\033[0m> or <\033[1;33m--delimitor\033[0m> delimitor '-d ,' is default\n");
    printf("  <\033[1;33m-h\033[0m>, <\033[1;33m-w\033[0m> or <\033[1;33m--height\033[0m>, <\033[1;33m--width\033[0m> setting window height and width. default is 1820*980\n");
    printf("  <\033[1;33m--help\033[0m> for usage info\n");
    printf("Example: ");
    printf("  \"%s data.csv -d | -h 900 -w 1800\"\n", args[0]);
    printf("that would specify that data.csv contains the coordinates separated by '|'\n eg \"1.0|2.0|3.0\" or \"1.0| 2.0| 3.0\" since spaces are ignored and a window size of 1800*900\n ");
    return 0;
  } else if (argc != 4 && argc != 2 && argc != 6 && argc != 8) {
    log_err(none)("invalid argument count, enter a csv file with coordinates");
    printf("%s <filename>\n", args[0]);
    return -1;
  }

  if(argc == 8 || argc == 6 || argc == 4 || argc == 2) {
    for(i = 1; i < argc; i++) {
      if(!strcmp(args[i], "-d") || !strcmp(args[i], "--delimitor")){
        assert(i+1 < argc, "no delimitor was specified try --help for help");
        delim = *args[i+1];
      } else if(!strcmp(args[i], "-h") || !strcmp(args[i], "--height")){
        assert(i+1 < argc, "no height was specified try --help for help");
        HEIGHT = atoi(args[i+1]);
        printf("set height: %s, (%i)\n", args[i+1], HEIGHT);
        pos = FVEC3(WIDTH/2.0 -(system_scale/2),HEIGHT/2.0+(system_scale/2),1.0);
      }else if(!strcmp(args[i], "-w") || !strcmp(args[i], "--width")){
        assert(i+1 < argc, "no width was specified try --help for help");
        WIDTH = atoi(args[i+1]);
        printf("set width: %s, (%i)\n", args[i+1], WIDTH);
        pos = FVEC3(WIDTH/2.0 -(system_scale/2),HEIGHT/2.0+(system_scale/2),1.0);
      }
      else if(strcmp(args[i-1], "-d") && strcmp(args[i-1], "--delimitor") &&
              strcmp(args[i-1], "-h") && strcmp(args[i-1], "--height")    &&
              strcmp(args[i-1], "-w") && strcmp(args[i-1], "--width")       ) {
        filename = args[i];
        printf("filename: %s, prev: %s\n", filename, args[i-1]);
      }
    }
  }
  init_window(&window, &renderer);

  settings     = GC_new_ctx(window, renderer, COLOR(0, 100, 100), 0, 0, 400, HEIGHT);
  info         = GC_new_ctx(window, renderer, COLOR(0, 0, 0), 0, 0, 0, 0);

  exit_btn     = GC_create_button(40, HEIGHT-100, 300, 50, "EXIT",
                   GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(176, 189, 112)));
  update_psize = GC_create_button(40, 170, 300, 50, "Pointsize",
                   GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(176, 189, 112)));
  txt_field    = GC_create_textfield(40, 110, 300, 50, "",
                   GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0,0,0)));
  tx_red       = GC_create_textfield(40, 240, 300, 50, "",
                   GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(255,0,0)));
  tx_green     = GC_create_textfield(40, 310, 300, 50, "",
                   GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0,255,0)));
  tx_blue      = GC_create_textfield(40, 380, 300, 50, "",
                   GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0,0,255)));
  clr_btn      = GC_create_button(40, 450, 300, 50, "set color",
                   GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(176, 189, 112)));
  toggle       = GC_create_toggle_button(40, 40, 300, 50, 20, "Axles:",
                   GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0,0,0)));
  title        = GC_create_label(((WIDTH)/2) - (230), 10, 0, 25, "Press \"ESC\" to show settings",
                   GC_PALETTE(COLOR(0,0,0), COLOR(176, 189, 112), COLOR(176, 189, 112)));
  settings_btn = GC_create_button(10, 10, 140, 25, "Settings",
                   GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0, 100, 100)));

  settings.shown = false;
  toggle.toggled = true;

  GC_add_onclick(&clr_btn,      update_color);
  GC_add_onclick(&exit_btn,     exit_onclick);
  GC_add_onclick(&toggle,       toggle_info);
  GC_add_onclick(&update_psize, set_psize);
  GC_add_onclick(&settings_btn, show_settings);

  GC_add(&settings, &exit_btn);
  GC_add(&settings, &txt_field);
  GC_add(&settings, &toggle);
  GC_add(&settings, &update_psize);
  GC_add(&settings, &tx_red);
  GC_add(&settings, &tx_green);
  GC_add(&settings, &tx_blue);
  GC_add(&settings, &clr_btn);
  GC_add(&info,     &title);
  GC_add(&info,     &settings_btn);

  vlist = read_vlist(filename, delim);

  rotate_x(PI_HALF);
  main_loop(&event);

  GC_destroy(&settings);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(0);
}
