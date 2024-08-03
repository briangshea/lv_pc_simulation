/**
 * @file main
 *
 */

#ifndef theme_H
#define theme_H

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl/lvgl.h"

/**
 * Theme styles exported from theme.c
 */
LV_STYLE_CONST_EXTERN(style_roller_temp_sel);
LV_STYLE_CONST_EXTERN(style_roller_temp_main);
LV_STYLE_CONST_EXTERN(style_sprinkler_summary_enabled);
LV_STYLE_CONST_EXTERN(style_sprinkler_summary_disabled);

void init_theme(void);

#endif // theme_H
