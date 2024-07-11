/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "screen_home.h"
#include "screen.h"
#include "widgets.h"
#include "theme.h"
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

static lv_obj_t *hvac_ctrl_pnl = NULL;
static lv_obj_t *hvac_ctrl_temp_roller = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC FUNCTIONS
 **********************/
void screen_home_event_handler(lv_event_t * e) {
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
                    screen_vs_display();
                break;
            }
        break;
    }

    return;
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void screen_home_display(void) {
    // Create a screen object
    screen = lv_obj_create(NULL);
    LV_ASSERT_MALLOC(screen);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_event_cb(screen, screen_home_event_handler,
        LV_EVENT_ALL,
        NULL
    );

    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);

    nav_bar = lv_nav_bar_create(screen, TITLE_SCREEN_HOME);
    lv_obj_add_flag(nav_bar, LV_OBJ_FLAG_EVENT_BUBBLE);

    hvac_ctrl_pnl = lv_panel_create(screen);
    lv_obj_set_size(hvac_ctrl_pnl, LV_PCT(25), 400);

    hvac_ctrl_temp_roller = lv_roller_create(hvac_ctrl_pnl);

    char options[255] = {'\0'};
    int offset = 0;
    for(int i = 60; i <= 95; i++) {
        offset += snprintf(&options[offset], 255, "%i\n", i);
    }
    options[offset-1] = 0;

    lv_obj_add_style(hvac_ctrl_temp_roller, &style_roller_temp_sel, LV_PART_SELECTED);
    lv_obj_add_style(hvac_ctrl_temp_roller, &style_roller_temp_main, LV_PART_MAIN);
    lv_roller_set_options(hvac_ctrl_temp_roller, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(hvac_ctrl_temp_roller, 75-60, LV_ANIM_ON);
    lv_obj_align(hvac_ctrl_temp_roller, LV_ALIGN_CENTER, 0, 0);
    lv_roller_set_visible_row_count(hvac_ctrl_temp_roller, 2);

    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 100, 0, true);
}
