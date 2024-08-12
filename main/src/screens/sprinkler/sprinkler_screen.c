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

static lv_obj_t *parameter_list = NULL;

static lv_obj_t *lbl_name = NULL;
static lv_obj_t *name = NULL;

static lv_obj_t *lbl_desc = NULL;
static lv_obj_t *desc = NULL;

static lv_obj_t *lbl_start = NULL;
static lv_obj_t *start = NULL;

static lv_obj_t *lbl_interval = NULL;
static lv_obj_t *interval = NULL;

static lv_obj_t *lbl_scale = NULL;
static lv_obj_t *scale = NULL;
static lv_obj_t *scale_label = NULL;

static lv_obj_t *lbl_enable = NULL;
static lv_obj_t *enable = NULL;

static lv_obj_t *keyboard = NULL;

static sprinkler_prog_dsc_t sprinkler_prog_dsc = 0;
static sprinkler_prog_t *activeProg = NULL;

static lv_obj_t *fo = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *      VARIABLES
 **********************/
static const int lbl_width = 100;
static const int offset = 5;
static const int spacing = 3;
static const int scale_w_adjust = 14;


/**********************
 *  STATIC FUNCTIONS
 **********************/
static
void screen_sprinkler_event_handler(lv_event_t * e) {
    LV_ASSERT_NULL(e);

    lv_obj_t *target = lv_event_get_current_target(e);
    lv_obj_t *scrn = lv_event_get_current_target_obj(e);

    switch(lv_event_get_code(e)) {
        case LV_EVENT_DELETE:
            sprinkler_prog_remove_cb(activeProg, sprinkler_prog_dsc);

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
        case LV_EVENT_CLICKED:
            if(keyboard) {
                lv_obj_t *ta = lv_keyboard_get_textarea(keyboard);
                lv_obj_remove_state(ta, LV_STATE_FOCUSED);
                lv_obj_delete(keyboard);
                keyboard = NULL;
            }
            if(fo) {
                lv_obj_remove_state(fo, LV_STATE_FOCUSED);
                fo = NULL;
            }
            break;
    }
    return;
}

static
void display_keyboard(lv_obj_t* textarea_obj) {
    int h = lv_obj_get_height(screen);
    if(!keyboard) {
        keyboard = lv_keyboard_create(screen);
        lv_obj_set_width(keyboard, LV_PCT(80));
        lv_obj_set_height(keyboard, h/3);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_align_to(keyboard, screen, LV_ALIGN_BOTTOM_MID, 0, -spacing);
    }
    lv_keyboard_set_textarea(keyboard, textarea_obj);
    //lv_obj_set_height(parameter_list, h-lv_get_obj);
}

static
void set_scale() {
    int s, q, r;
    s = sprinkler_prog_get_scale(activeProg);
    q = s/10;
    r = s-q*10;
    lv_label_set_text_fmt(scale_label, "%d.%d%%", q, r);
}

static
void sprinkler_screen_refresh(void) {
    lv_textarea_set_text(name, sprinkler_prog_get_name(activeProg));

    if(sprinkler_prog_get_enabled(activeProg))
        lv_obj_add_state(enable, LV_STATE_CHECKED);
    else
        lv_obj_remove_state(enable, LV_STATE_CHECKED);

    lv_textarea_set_text(desc, sprinkler_prog_get_description(activeProg));

    char strTime[64] = {0};
    sprinkler_time_t const * time = sprinkler_prog_get_start(activeProg);
    lv_snprintf(strTime, 64, "%d:%d", time->hour, time->min);
    lv_textarea_set_text(start, strTime);

    lv_spinbox_set_value(interval, sprinkler_prog_get_interval(activeProg));
    lv_slider_set_value(scale, sprinkler_prog_get_scale(activeProg), LV_ANIM_OFF);
    set_scale();
}

static
void sprinkler_prog_cb(sprinkler_prog_event_t *e) {
    switch(sprinkler_prog_event_get_type(e)) {
        case SPRINKLER_EVENT_CHANGED:
            sprinkler_screen_refresh();
            break;
        case SPRINKLER_EVENT_STARTED:
        case SPRINKLER_EVENT_ENDED:
        case SPRINKLER_EVENT_DELETED:
        break;
    }
}

typedef enum {
    PROG_FIELD_NAME,
    PROG_FIELD_DESC,
    PROG_FIELD_START
} program_field_t;

static
void textarea_event_callback(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_current_target_obj(e);
    program_field_t field = (program_field_t)lv_event_get_user_data(e);

    if(fo) {
        lv_obj_remove_state(fo, LV_STATE_FOCUSED);
        fo = NULL;
    }

    switch(lv_event_get_code(e)) {
        case LV_EVENT_FOCUSED:
            fo = obj;
            // Set keyboard object to name
            switch(field) {
                case PROG_FIELD_NAME:
                    display_keyboard(name);
                    break;
                case PROG_FIELD_DESC:
                    display_keyboard(desc);
            }
            break;
        case LV_EVENT_DEFOCUSED:
            switch(field) {
                case PROG_FIELD_NAME:
                    sprinkler_prog_set_name(activeProg, lv_textarea_get_text(name));
                    break;
                case PROG_FIELD_DESC:
                    sprinkler_prog_set_description(activeProg, lv_textarea_get_text(desc));
                    break;
            } //end switch
            // hide keyboard
            if(keyboard) {
                lv_obj_delete(keyboard);
                keyboard = NULL;
            }
            break;
    } // end switch
}

static
void enable_event_cb(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_current_target_obj(e);
    int enabled = (lv_obj_get_state(obj)&LV_STATE_CHECKED)!=0;
    sprinkler_prog_set_enabled(activeProg, enabled);
}


static
void scale_value_changed(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_current_target_obj(e);
    sprinkler_prog_set_scale(activeProg, lv_slider_get_value(obj));
    set_scale();
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void screen_sprinkler_display(sprinkler_prog_t *selectedProgram) {
    // Set the selected Program to Display
    activeProg = selectedProgram;

    // Create a screen object

    screen = lv_obj_create(NULL);
    LV_ASSERT_MALLOC(screen);

    if(!activeProg) activeProg = progs[0];

    lv_obj_add_event_cb(screen, screen_sprinkler_event_handler,
        LV_EVENT_ALL,
        NULL
    );

    //lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_ROW);
    //lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    nav_bar = lv_navbar_create(screen);
    lv_navbar_set_title(nav_bar, TITLE_SCREEN_SPRINKLER);
    lv_obj_add_flag(nav_bar, LV_OBJ_FLAG_EVENT_BUBBLE);

    parameter_list = lv_obj_create(screen);
    lv_obj_remove_style_all(parameter_list);
    lv_obj_set_size(parameter_list, LV_SIZE_CONTENT, 300);
    lv_obj_add_flag(parameter_list, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_align_to(parameter_list, nav_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);

    lbl_name = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_name, "%s:", PROGRM_LABEL_NAME);
    lv_obj_set_width(lbl_name, LV_SIZE_CONTENT);

    name = lv_textarea_create(parameter_list);
    lv_textarea_set_one_line(name, true);
    lv_obj_align(name, LV_ALIGN_TOP_LEFT, lbl_width+offset, 0);
    lv_obj_align_to(lbl_name, name, LV_ALIGN_OUT_LEFT_MID, -offset, 0);
    lv_obj_add_event_cb(name, textarea_event_callback, LV_EVENT_ALL, (void*)PROG_FIELD_NAME);

    lbl_enable = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_enable, "%s:", PROGRM_LABEL_ENABLE);

    enable = lv_switch_create(parameter_list);
    lv_obj_set_size(enable, 38, 19);
    lv_obj_align_to(enable, name, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);
    lv_obj_align_to(lbl_enable, enable, LV_ALIGN_OUT_LEFT_MID, -offset, 0);
    lv_obj_add_event_cb(enable, enable_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lbl_desc = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_desc, "%s:", PROGRM_LABEL_DESC);

    desc = lv_textarea_create(parameter_list);
    lv_obj_align_to(desc, enable, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);
    lv_obj_align_to(lbl_desc, desc, LV_ALIGN_OUT_LEFT_MID, -offset, 0);
    lv_obj_add_event_cb(desc, textarea_event_callback, LV_EVENT_ALL, (void*)PROG_FIELD_DESC);

    lbl_start = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_start, "%s:", PROGRM_LABEL_START);

    start = lv_textarea_create(parameter_list);
    lv_textarea_set_one_line(start, true);
    //lv_obj_add_event_cb(start, date_picker_cb, LV_EVENT_CLICKED, (void*)PROG_FIELD_START);

    lv_obj_align_to(start, desc, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);
    lv_obj_align_to(lbl_start, start, LV_ALIGN_OUT_LEFT_MID, -offset, 0);

    lbl_interval = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_interval, "%s (%s):", PROGRM_LABEL_INTERVAL, PROGRM_LABEL_INTERVAL_UNITS);

    interval = lv_spinbuttons_create(parameter_list);
    lv_spinbox_set_range(interval, 1, 30);
    lv_spinbox_set_digit_format(interval, 2, 0);
    lv_spinbox_step_prev(interval);
    lv_obj_align_to(interval, start, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);
    lv_obj_align_to(lbl_interval, interval, LV_ALIGN_OUT_LEFT_MID, -offset, 0);

    lbl_scale = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_scale, "%s:", PROGRM_LABEL_SCALE);

    scale = lv_slider_create(parameter_list);
    lv_obj_set_width(scale, lv_obj_get_width(desc)-2*scale_w_adjust);
    lv_slider_set_range(scale, SPRINKLER_SCALE_MIN, SPRINKLER_SCALE_MAX);
    lv_slider_set_value(scale, sprinkler_prog_get_scale(activeProg), LV_ANIM_OFF);
    lv_obj_add_event_cb(scale, scale_value_changed, LV_EVENT_VALUE_CHANGED, NULL);

    scale_label = lv_label_create(scale);
    lv_obj_align(scale_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(scale_label, (lv_color_t)LV_COLOR_MAKE(0xff, 0xff, 0xff), 0);

    lv_obj_align_to(scale, interval, LV_ALIGN_OUT_BOTTOM_LEFT, scale_w_adjust, spacing+3);
    lv_obj_align_to(lbl_scale, scale, LV_ALIGN_OUT_LEFT_MID, -(offset+scale_w_adjust), 0);

    sprinkler_screen_refresh();
    sprinkler_prog_dsc = sprinkler_prog_add_cb(activeProg, sprinkler_prog_cb, NULL);

    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 100, 0, true);
}
