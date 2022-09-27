/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * GC_listener header file
 *
 *==========================================================*/

#ifndef _GC_LISTENER_H_
#define _GC_LISTENER_H_

#include <sdl/SDL.h>

/**
 * event types
 **/
typedef enum event_type {
  GC_MOUSE_MOTION,
  GC_MOUSE_PRESSED,
  GC_MOUSE_RELEASED,
  GC_MOUSE_SCROLLED,
  GC_KEY_PRESSED,
  GC_KEY_RELEASED,
  GC_TEXT_INPUT,
  GC_CLICKED,   // external event
  GC_KEY_TYPED, // external event
  GC_TOGGLED,    // external event
  GC_NO_EVENT
} GC_event_type;

/**
 * event used by the library to trigger external events
 * based on sdl events aswell as the external events themselves
 **/
typedef struct event {
  GC_event_type type;
  SDL_Event evt;
} GC_event;

/**
 * event listener used to handle basic backend functionality
 * such as changing button colors on hover and press aswell as
 * triggering the external events, and handling them
 **/
typedef struct listener {
  /**
   * the listener type
   **/
  GC_event_type type;

  /**
   * the function to be executed when a event is triggered.
   * GC_event is the event that was triggered and the void*
   * points to the component that it was triggered on
   **/
  void (*listener_func)(GC_event, void*);
} GC_listener;

/**
 * used in the component struct to create a linked list whith all
 * its listeners
 **/
typedef struct lnode {

  GC_listener listener;

  struct lnode * next;

} GC_lnode;

GC_lnode * new_lnode(GC_listener listener);

#endif /* _GC_LISTENER_H_ */
