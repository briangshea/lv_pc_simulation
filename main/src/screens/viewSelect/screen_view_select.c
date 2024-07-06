/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_view_select.h"
#include "widgets.h"
#include "stdio.h"
#include "time.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *screen_vs = NULL;
static lv_obj_t *nav_bar = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC FUNCTIONS
 **********************/
void screen_vs_event_handler(lv_event_t * e) {
    static bool isDeleting = false;

    // Prevent any callbacks during delete
    if( isDeleting ) return;

    LV_ASSERT_NULL(e);

    switch(e->code) {
        case LV_EVENT_DELETE:
            isDeleting = true;
            lv_obj_delete(nav_bar);
        break;
        case LV_EVENT_GESTURE:
            const lv_dir_t g_dir = lv_indev_get_gesture_dir(lv_indev_active());
            switch(g_dir) {
                case LV_DIR_LEFT:
                    // Choose Screen on left
                break;
                case LV_DIR_RIGHT:
                    // Choose Screen on right
                break;
                case LV_DIR_BOTTOM:
                    // Choose Screen on bottom
                break;
                case LV_DIR_TOP:
                break;
            }
        break;
    }
    return;
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void screen_home_display(lv_obj_t * screen) {
    // Create a screen object
    home_screen = lv_obj_create(NULL);
    LV_ASSERT_MALLOC(home_screen);
    lv_obj_add_event_cb(home_screen, screen_home_event_handler,
        LV_EVENT_DELETE  | \
        LV_EVENT_GESTURE,  \
        NULL
    );

    nav_bar = lv_nav_bar_create(home_screen, TITLE_SCREEN_HOME);

    lv_scr_load_anim(home_screen, LV_SCR_LOAD_ANIM_FADE_IN, 100, 0, true);
}
