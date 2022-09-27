

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

#define WIDTH  2000
#define HEIGHT 1200
#define WINDOW_TITLE "plotter"


GC_context settings, info;
GC_component exit_btn, txt_field, toggle, title, settings_btn,
update_psize, tx_red, tx_green, tx_blue, clr_btn;
fvec3_t O = FVEC3(0.0,0.0,0.0);
fvec3_t X = FVEC3(1.0,0.0,0.0);
fvec3_t Y = FVEC3(0.0,1.0,0.0);
fvec3_t Z = FVEC3(0.0,0.0,1.0);
fvec3_t pos = FVEC3(1000.0,600.0,1.0);

f64_t system_scale = 1.0;

bool MOUSE1 = false;
bool MOUSE2 = false;
bool SHIFT = false;

i64_t pmx=0, pmy=0;

vlist_t vlist;

i32_t psize = 5;

bool show_axles = true;

color_t pcolor = COLOR(0,100,100);

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

void rotate_xyz_x(f64_t ang)
{
  i32_t i;
  O = f3matmult(RX3X3(ang), O);
  X = f3matmult(RX3X3(ang), X);
  Y = f3matmult(RX3X3(ang), Y);
  Z = f3matmult(RX3X3(ang), Z);
  for(i = 0; i < vlist.length; i++)
    vlist.vecs[i] = f3matmult(RX3X3(ang), vlist.vecs[i]);
}

void rotate_xyz_y(f64_t ang)
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
    switch ((*event).type) {
      case SDL_MOUSEBUTTONDOWN:
        switch ((*event).button.button) {
          case SDL_BUTTON_LEFT:  MOUSE1 = true; break;
          case SDL_BUTTON_RIGHT: MOUSE2 = true; break;
          default:               break;
        } break;
      case SDL_MOUSEBUTTONUP:
        switch ((*event).button.button) {
          case SDL_BUTTON_LEFT:  MOUSE1 = false; break;
          case SDL_BUTTON_RIGHT: MOUSE2 = false; break;
          default:               break;
        } break;
      case SDL_MOUSEMOTION:
        if (MOUSE1 || MOUSE2) {
            if(SHIFT) {
              rotate_xyz_y(-(event->motion.x - pmx)/300.0);
              rotate_xyz_x((event->motion.y - pmy)/300.0);
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

        if((*event).key.keysym.sym == SDLK_ESCAPE && !settings.shown) {
          settings.shown = true;
        } else if ((*event).key.keysym.sym == SDLK_ESCAPE) settings.shown = false;
        if ((*event).key.keysym.sym == SDLK_LSHIFT) {
          SHIFT = true;
        }
        break;
      case SDL_KEYUP:
        if ((*event).key.keysym.sym == SDLK_LSHIFT) SHIFT = false;
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


    /**
    *   update screen?
    **/
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
  //scale.shown = c->toggled;
}
void update_color(GC_event evt, void* cmp)
{
  GC_component * c;

  pcolor.r = atoi(tx_red.text.str);
  pcolor.g = atoi(tx_green.text.str);
  pcolor.b = atoi(tx_blue.text.str);
  //scale.shown = c->toggled;
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

  SDL_Event event;

  init_window(&window, &renderer);

  settings = GC_new_ctx(window, renderer, COLOR(0, 100, 100), 0, 0, 400, HEIGHT);
  exit_btn = GC_create_button(40, 1100, 300, 50, "EXIT", GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(176, 189, 112)));
  update_psize = GC_create_button(40, 170, 300, 50, "Pointsize", GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(176, 189, 112)));
  txt_field = GC_create_textfield(40, 110, 300, 50, "", GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0,0,0)));
  tx_red = GC_create_textfield(40, 240, 300, 50, "", GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(255,0,0)));
  tx_green = GC_create_textfield(40, 310, 300, 50, "", GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0,255,0)));
  tx_blue = GC_create_textfield(40, 380, 300, 50, "", GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0,0,255)));
  clr_btn = GC_create_button(40, 450, 300, 50, "set color", GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(176, 189, 112)));
  toggle = GC_create_toggle_button(40, 40, 300, 50, 20, "axles:", GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0,0,0)));
  settings.shown = false;
  toggle.toggled = true;
  GC_add_onclick(&clr_btn, update_color);
  GC_add_onclick(&exit_btn, exit_onclick);
  GC_add_onclick(&toggle, toggle_info);
  GC_add_onclick(&update_psize, set_psize);
  GC_add(&settings, &exit_btn);
  GC_add(&settings, &txt_field);
  GC_add(&settings, &toggle);
  GC_add(&settings, &update_psize);
  GC_add(&settings, &tx_red);
  GC_add(&settings, &tx_green);
  GC_add(&settings, &tx_blue);
  GC_add(&settings, &clr_btn);



  info = GC_new_ctx(window, renderer, COLOR(0, 0, 0), 0, 0, 0, 0);
  title = GC_create_label(800, 10, 0, 25, "Press \"ESC\" to show settings",
    GC_PALETTE(COLOR(0,0,0), COLOR(176, 189, 112), COLOR(176, 189, 112)));

  settings_btn = GC_create_button(10, 10, 140, 25, "Settings", GC_PALETTE(COLOR(0,0,0), COLOR(255,255,255), COLOR(0, 100, 100)));
  GC_add_onclick(&settings_btn, show_settings);
  GC_add(&info, &title);
  GC_add(&info, &settings_btn);

  vlist = read_vlist("XOR2.csv", ',');

  for(int i = 0; i < vlist.length; i++){
    printf("Z: %f\n",vlist.vecs[i].z);
  }

  main_loop(&event);

  GC_destroy(&settings);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(0);
}
