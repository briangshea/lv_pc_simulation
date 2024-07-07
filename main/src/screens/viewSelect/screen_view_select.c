/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_view_select.h"
#include "screen.h"
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
static lv_obj_t *screen = NULL;
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
    LV_ASSERT_NULL(e);

    switch(lv_event_get_code(e)) {
        case LV_EVENT_DELETE:
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
                    screen_home_display();
                break;
            }
        break;
    }
    return;
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void screen_vs_display(void) {
    // Create a screen object
    screen = lv_obj_create(NULL);
    LV_ASSERT_MALLOC(screen);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_event_cb(screen, screen_vs_event_handler,
        LV_EVENT_ALL,
        NULL
    );

    nav_bar = lv_nav_bar_create(screen, TITLE_SCREEN_VIEW_SELECT);

    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 100, 0, true);
}
