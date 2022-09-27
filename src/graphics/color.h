/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * color header file
 *
 *==========================================================*/

#ifndef _COLOR_H_
#define _COLOR_H_

#include "../util/types.h"

#define COLOR(_r, _g, _b) ((color_t){.r=(_r),.g=(_g),.b=(_b)})

#define BLACK  COLOR(0,0,0)
#define GREY   COLOR(100,100,100)
#define WHITE  COLOR(255,255,255)
#define GREEN  COLOR(0,255,0)
#define BLUE   COLOR(0,0,255)
#define RED    COLOR(255,0,0)
#define YELLOW COLOR(255,255,0)
#define PURPLE COLOR(255,0,255)

#define NULLCOLOR COLOR(0,0,0,0)
#define COLOR_ADD(c1, c2) COLOR(((c1).r+(c2).r), ((c1).g+(c2).g), ((c1).b+(c2).b))
#define COLOR_SUB(c1, c2) COLOR(((c1).r-(c2).r), ((c1).g-(c2).g), ((c1).b-(c2).b))
#define COLOR_SCALE(c, f) COLOR(((c).r*(f)), ((c).g*(f)), ((c).b*(f)))

#define COLOR_EQ(c1, c2) (((c1).r == (c2).r) &&\
                          ((c1).g == (c2).g) &&\
                          ((c1).b == (c2).b))

typedef struct color {
  /**
   * red, green, blue and alpha values
   **/
  u8_t r, g, b, a;

} color_t;


#endif /* _COLOR_H_ */
