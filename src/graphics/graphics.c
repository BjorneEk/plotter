/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * graphics functions
 *
 *==========================================================*/


#include "graphics.h"


 void fill_circle(SDL_Renderer* rndr, color_t c, i32_t _x, i32_t _y, i32_t rad) {
   i32_t i, j, x, y;

   for(i = 0; i < (2*rad); i++) {
     for(j = 0; j < (2*rad); j++) {
       x = (i-rad) * (i-rad);
       y = (j-rad) * (j-rad);
       if ((x + y) <= (rad*rad)){
         SET_PIXEL(rndr, c, _x+(i-rad), _y+(j-rad));
       }
     }
   }
 }

void draw_xyz(SDL_Renderer* rndr, fvec3_t _O, fvec3_t _X, fvec3_t _Y, fvec3_t _Z, fvec3_t pos, f64_t scale)
{
  fvec3_t O, X, Y, Z;


  O = fvec3Sop(_O, scale, *);
  X = fvec3Sop(_X, scale, *);
  Y = fvec3Sop(_Y, scale, *);
  Z = fvec3Sop(_Z, scale, *);

  vec3opR(O, pos, +=);
  vec3opR(X, pos, +=);
  vec3opR(Y, pos, +=);
  vec3opR(Z, pos, +=);

  SDL_RenderDrawLine(rndr,O.x,O.y,X.x,X.y);
  SDL_RenderDrawLine(rndr,O.x,O.y,Y.x,Y.y);
  SDL_RenderDrawLine(rndr,O.x,O.y,Z.x,Z.y);
  SDL_RenderDrawLine(rndr,O.x+1,O.y+1,X.x+1,X.y+1);
  SDL_RenderDrawLine(rndr,O.x+1,O.y+1,Y.x+1,Y.y+1);
  SDL_RenderDrawLine(rndr,O.x+1,O.y+1,Z.x+1,Z.y+1);
}
