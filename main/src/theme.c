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
const lv_style_const_prop_t style_screen_def[] = {
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
LV_STYLE_CONST_INIT(style_screen, style_screen_def);


const lv_style_const_prop_t style_global_def[] = {
    LV_STYLE_CONST_PAD_TOP(0),
    LV_STYLE_CONST_PAD_BOTTOM(0),
    LV_STYLE_CONST_PAD_LEFT(0),
    LV_STYLE_CONST_PAD_RIGHT(0),
    LV_STYLE_CONST_OUTLINE_WIDTH(0),
#ifdef DEBUG_LAYOUT
    LV_STYLE_CONST_BORDER_COLOR(LV_COLOR_MAKE(0x55, 0x55, 0x55)),
    LV_STYLE_CONST_BORDER_WIDTH(1),
#endif
    LV_STYLE_CONST_PROPS_END
};
LV_STYLE_CONST_INIT(style_global, style_global_def);

const lv_style_const_prop_t style_panel_def[] = {
    LV_STYLE_CONST_BG_COLOR(LV_COLOR_MAKE(0x33, 0x33, 0x33)),
    LV_STYLE_CONST_RADIUS(5),
    LV_STYLE_CONST_BG_OPA(0x80),

    LV_STYLE_CONST_PROPS_END
};
LV_STYLE_CONST_INIT(style_panel, style_panel_def);

const lv_style_const_prop_t style_roller_temp_sel_def[] = {
    LV_STYLE_CONST_TEXT_FONT(&lv_font_montserrat_48),
    LV_STYLE_CONST_PROPS_END
};
LV_STYLE_CONST_INIT(style_roller_temp_sel, style_roller_temp_sel_def);

const lv_style_const_prop_t style_roller_temp_main_def[] = {
    LV_STYLE_CONST_TEXT_FONT(&lv_font_montserrat_24),
    LV_STYLE_CONST_PROPS_END
};
LV_STYLE_CONST_INIT(style_roller_temp_main, style_roller_temp_main_def);

const lv_style_const_prop_t style_navbar_def[] = {
    LV_STYLE_CONST_BG_COLOR(LV_COLOR_MAKE(0x66, 0x66, 0xaa)),
    LV_STYLE_CONST_RADIUS(5),
    LV_STYLE_CONST_BG_OPA(0x80),
    LV_STYLE_CONST_PAD_TOP(3),
    LV_STYLE_CONST_PAD_BOTTOM(3),
    LV_STYLE_CONST_PAD_LEFT(3),
    LV_STYLE_CONST_PAD_RIGHT(3),
    LV_STYLE_CONST_PROPS_END
};
LV_STYLE_CONST_INIT(style_navbar, style_navbar_def);

const lv_style_const_prop_t style_image_def[] = {
    LV_STYLE_CONST_RADIUS(5),
    LV_STYLE_CONST_CLIP_CORNER(true),
    LV_STYLE_CONST_PROPS_END
};
LV_STYLE_CONST_INIT(style_image, style_image_def);

const lv_style_const_prop_t style_sprinkler_summary_enabled_def[] = {
    LV_STYLE_CONST_BG_COLOR(LV_COLOR_MAKE(0x11, 0x66, 0x11)),
    LV_STYLE_CONST_PROPS_END
};
LV_STYLE_CONST_INIT(style_sprinkler_summary_enabled, style_sprinkler_summary_enabled_def);

const lv_style_const_prop_t style_sprinkler_summary_disabled_def[] = {
    LV_STYLE_CONST_BG_COLOR(LV_COLOR_MAKE(0x66, 0x11, 0x11)),
    LV_STYLE_CONST_PROPS_END
};
LV_STYLE_CONST_INIT(style_sprinkler_summary_disabled, style_sprinkler_summary_disabled_def);


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
    LV_ASSERT_NULL(obj);

    if(lv_obj_check_type(obj, &lv_obj_class) && lv_obj_get_parent(obj) == NULL) {
        // This is a screen type object, apply default screen theme
        lv_obj_add_style(obj, &style_screen, 0);
    } else {
        // Add Global Style
        lv_obj_add_style(obj, &style_global, 0);

        if(lv_obj_check_type(obj, &lv_navbar_class)) {
            // Style the Navigation Bar
            lv_obj_add_style(obj, &style_navbar, 0);
        } else if(lv_obj_check_type(obj, &lv_panel_class)) {
            // Style Panels
            lv_obj_add_style(obj, &style_panel, 0);
        } else if(lv_obj_check_type(obj, &lv_clock_class)) {

        } else if(lv_obj_check_type(obj, &lv_image_class)) {
            // Round corners on images by default
            lv_obj_add_style(obj, &style_image, 0);
        }
    }
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
