#ifndef lvgl_spinbuttons_H
#define lvgl_spinbuttons_H

#include "lvgl/lvgl.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct lv_spinbuttons_s lv_spinbuttons_t;

extern const lv_obj_class_t lv_spinbuttons_class;

lv_obj_t * lv_spinbuttons_create(lv_obj_t *parent);

#ifdef __cplusplus
    }
#endif

#endif //lvgl_spinbuttons_H
