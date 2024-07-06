#ifndef lv_clock_H
#define lv_clock_H

#include "lvgl/lvgl.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct lv_clock_s lv_clock_t;

extern const lv_obj_class_t lv_clock_class;

lv_obj_t * lv_clock_create(lv_obj_t *parent);

void lv_clock_set_interval(lv_obj_t * obj, uint32_t interval_ms);
void lv_clock_set_value(lv_obj_t * obj, struct tm *timeinfo);

#ifdef __cplusplus
    }
#endif

#endif // lv_clock_H
