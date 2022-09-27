/*==========================================================*
 *
 * @author Gustaf Franzén :: https://github.com/BjorneEk;
 *
 * GC_listener functions
 *
 *==========================================================*/

#include "GC_listener.h"
#include "../util/assert.h"

GC_lnode * new_lnode(GC_listener listener)
{
   GC_lnode *res;

  res = malloc(sizeof(GC_lnode));
  assert(res != NULL, "Out of Memory (GC_lnode)");
  res->listener = listener;
  res->next = NULL;
  return res;
}
