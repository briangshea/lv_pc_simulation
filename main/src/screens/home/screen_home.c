/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <time.h>

#include "config.h"
#include "screen.h"
#include "widgets.h"
#include "theme.h"
#include "sprinkler.h"
#include "sprinkler_summary.h"

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

static lv_obj_t *spkrl_sum_pnl = NULL;

static lv_obj_t *sprkl_summary[MAX_PROGS] = {NULL};

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

    lv_obj_t *target = lv_event_get_current_target(e);
    lv_obj_t *scrn = lv_event_get_current_target_obj(e);

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

void hvac_ctrl_temp_roller_cb(lv_event_t *e) {
    LV_ASSERT_NULL(e);

    lv_obj_t *obj = lv_event_get_target_obj(e);
    LV_ASSERT_NULL(obj);

    switch(lv_event_get_code(e)) {
        case LV_EVENT_FOCUSED:
            // swiping up/down on the roller causes the screen to receive a gesture
            // event, disable gestures events while focused.
            //lv_obj_t *scrn = lv_obj_get_screen(obj);

            lv_roller_set_visible_row_cnt(obj, 7);
            break;
        case LV_EVENT_DEFOCUSED:
        case LV_EVENT_LEAVE:
            lv_roller_set_visible_row_cnt(obj, 2);
            break;
    }
    return;
}

void sprinkler_summary_cb(lv_event_t *e) {
   lv_obj_t *obj = lv_event_get_target_obj(e);
   sprinkler_prog_t *prog = (sprinkler_prog_t *)lv_event_get_user_data(e);

   switch(lv_event_get_code(e)) {
    case LV_EVENT_CLICKED:
        screen_sprinkler_display(prog);
        break;
   }
}

void sprinkler_prog_cb(sprinkler_prog_event_t *e) {
    lv_obj_t *obj = (lv_obj_t *)sprinkler_prog_event_get_user_data(e);
    if(!obj) return;

    switch(sprinkler_prog_event_get_type(e)) {
        case SPRINKLER_EVENT_CHANGED:
            sprinkler_summary_refresh(obj);
            break;
        case SPRINKLER_EVENT_STARTED:
        case SPRINKLER_EVENT_ENDED:
        case SPRINKLER_EVENT_CREATED:
        case SPRINKLER_EVENT_DELETED:
        break;
    }
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

    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_ROW);

    nav_bar = lv_navbar_create(screen);
    lv_navbar_set_title(nav_bar, TITLE_SCREEN_HOME);
    lv_obj_add_flag(nav_bar, LV_OBJ_FLAG_EVENT_BUBBLE);

    hvac_ctrl_pnl = lv_panel_create(screen);
    lv_obj_add_flag(hvac_ctrl_pnl, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_set_size(hvac_ctrl_pnl, LV_PCT(25), 400);

    hvac_ctrl_temp_roller = lv_roller_create(hvac_ctrl_pnl);

    char options[255] = {'\0'};
    int offset = 0;
    for(int i = 95; i >= 60; i--) {
        offset += snprintf(&options[offset], 255, "%i\n", i);
    }
    options[offset-1] = 0;

    lv_obj_align(hvac_ctrl_temp_roller, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(hvac_ctrl_temp_roller, &style_roller_temp_sel, LV_PART_SELECTED);
    lv_obj_add_style(hvac_ctrl_temp_roller, &style_roller_temp_main, LV_PART_MAIN);
    lv_obj_remove_flag(hvac_ctrl_temp_roller, LV_OBJ_FLAG_GESTURE_BUBBLE);

    lv_roller_set_options(hvac_ctrl_temp_roller, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(hvac_ctrl_temp_roller, 95-75, LV_ANIM_ON);

    lv_roller_set_visible_row_count(hvac_ctrl_temp_roller, 2);
    lv_obj_add_event_cb(hvac_ctrl_temp_roller, hvac_ctrl_temp_roller_cb, LV_EVENT_ALL, NULL);

    spkrl_sum_pnl = lv_panel_create(screen);
    lv_obj_set_style_pad_all(spkrl_sum_pnl, 5, 0);

    lv_obj_set_size(spkrl_sum_pnl, LV_SIZE_CONTENT, 400);
    lv_obj_set_flex_flow(spkrl_sum_pnl, LV_FLEX_FLOW_COLUMN);

    for(int i=0; i<MAX_PROGS; i++) {
        sprkl_summary[i] = sprinkler_summery_create(spkrl_sum_pnl, progs[i]);
        sprinkler_prog_set_cb(progs[i], sprinkler_prog_cb, sprkl_summary[i]);
        lv_obj_add_event_cb(sprkl_summary[i], sprinkler_summary_cb, LV_EVENT_ALL, progs[i]);
    }

    sprinkler_prog_set_description(progs[0], "A really long program description to test scrolling.");

    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 100, 0, true);
}
