#ifndef lvgl_navbar_H
#define lvgl_navbar_H

#include "lvgl/lvgl.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct lv_nav_bar_s lv_nav_bar_t;

extern const lv_obj_class_t lv_nav_bar_class;

lv_obj_t * lv_nav_bar_create(lv_obj_t *parent, const char *title);


#ifdef __cplusplus
    }
#endif

#endif //lvgl_navbar_H
