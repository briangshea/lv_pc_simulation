/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "lvgl/lvgl.h"
#include "widgets.h"

/*********************
 *      DEFINES
 *********************/
//#define DEBUG_LAYOUT

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_style_const_prop_t style_global_def[] = {
    LV_STYLE_CONST_PAD_TOP(0),
    LV_STYLE_CONST_PAD_BOTTOM(0),
    LV_STYLE_CONST_PAD_LEFT(0),
    LV_STYLE_CONST_PAD_RIGHT(0),

#ifdef DEBUG_LAYOUT
    LV_STYLE_CONST_BORDER_COLOR(LV_COLOR_MAKE(0x55, 0x55, 0x55)),
    LV_STYLE_CONST_BORDER_WIDTH(1),
#endif

    LV_STYLE_CONST_PROPS_END
};

LV_STYLE_CONST_INIT(style_global, style_global_def);

//lv_style_t style_global;

/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC FUNCTIONS
 **********************/
/*
   Will be called when the styles of the base theme are already added
   to add new styles
*/
static void new_theme_apply_cb(lv_theme_t * th, lv_obj_t * obj)
{
    LV_UNUSED(th);

    //if(lv_obj_check_type(obj, &lv_clock_class)) {
        lv_obj_add_style(obj, &style_global, 0);
    //    return;
    //}
    return;
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void init_theme(void) {

    // Initialize Sytle Data
    //lv_style_init(&style_global);

    //lv_style_set_pad_all(&style_global, 3);
    //lv_style_set_border_color(&style_global, (lv_color_t)LV_COLOR_MAKE(0x55, 0x55, 0x55));
    //lv_style_set_border_width(&style_global, 1);

    /*Initialize the new theme from the current theme*/
    lv_theme_t * th_act = lv_disp_get_theme(NULL);
    static lv_theme_t th_new;
    th_new = *th_act;

    /*Set the parent theme and the style apply callback for the new theme*/
    lv_theme_set_parent(&th_new, th_act);
    lv_theme_set_apply_cb(&th_new, new_theme_apply_cb);

    /*Assign the new theme the the current display*/
    lv_disp_set_theme(NULL, &th_new);
}
