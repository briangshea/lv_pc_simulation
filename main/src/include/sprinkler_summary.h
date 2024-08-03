#ifndef sprinkler_summary_H
#define sprinkler_summary_H

#include "lvgl.h"
#include "sprinkler.h"

extern sprinkler_prog_t *progs[MAX_PROGS];

lv_obj_t *sprinkler_summery_create(lv_obj_t * parent, sprinkler_prog_t *prog);
void sprinkler_summary_refresh(lv_obj_t *obj);

#endif //sprinkler_summary_H
