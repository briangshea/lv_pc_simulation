#ifndef lvgl_panel_H
#define lvgl_panel_H

#include "lvgl/lvgl.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct lv_panel_s lv_panel_t;

extern const lv_obj_class_t lv_panel_class;

lv_obj_t * lv_panel_create(lv_obj_t *parent);

#ifdef __cplusplus
    }
#endif

#endif //lvgl_navbar_H
