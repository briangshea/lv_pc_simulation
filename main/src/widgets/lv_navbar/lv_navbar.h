#ifndef lvgl_navbar_H
#define lvgl_navbar_H

#include "lvgl/lvgl.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct lv_navbar_s lv_navbar_t;

extern const lv_obj_class_t lv_navbar_class;

lv_obj_t * lv_navbar_create(lv_obj_t *parent);
void       lv_navbar_set_title(lv_obj_t *parent, const char *title);

#ifdef __cplusplus
    }
#endif

#endif //lvgl_navbar_H
