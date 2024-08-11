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

static lv_obj_t *lbl_end = NULL;
static lv_obj_t *end = NULL;

static lv_obj_t *lbl_interval = NULL;
static lv_obj_t *interval = NULL;

static lv_obj_t *lbl_scale = NULL;
static lv_obj_t *scale = NULL;
static lv_obj_t *scale_label = NULL;

static lv_obj_t *lbl_enable = NULL;
static lv_obj_t *enable = NULL;

static lv_obj_t *keyboard = NULL;
static lv_obj_t *calendar = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC FUNCTIONS
 **********************/
void screen_sprinkler_event_handler(lv_event_t * e) {
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

static
void sprinkler_prog_cb(sprinkler_zone_event_t *e) {
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

typedef enum {PROG_START_FIELD, PROG_END_FIELD} calendar_field_t;
typedef struct {
    sprinkler_prog_t *p;
    calendar_field_t field;
} calendar_cb_data_t;

void calendar_start_changed_cb(lv_event_t *e) {
    lv_obj_t *this = lv_event_get_current_target_obj(e);
    calendar_cb_data_t *cbdata = (calendar_cb_data_t *)lv_event_get_user_data(e);
    sprinkler_prog_t *p = cbdata->p;

    lv_calendar_date_t day;
    struct tm t = {0};
    time_t newTime;

    lv_calendar_get_pressed_date(calendar, &day);
    t.tm_year = day.year-YEAR_EPOC;
    t.tm_mday = day.day;
    t.tm_mon  = day.month;

    newTime = mktime(&t);
    if(cbdata->field == PROG_START_FIELD) {
        sprinkler_prog_set_start(p, &newTime);
    }
    else if(cbdata->field == PROG_END_FIELD) {
        sprinkler_prog_set_end(p, &newTime);
    }

    lv_obj_delete(calendar);
    calendar = NULL;
    free(cbdata);
}

void start_clicked_cb(lv_event_t *e) {
    lv_obj_t *this = lv_event_get_target_obj(e);
    sprinkler_prog_t *p = (sprinkler_prog_t *)lv_event_get_user_data(e);
    calendar_cb_data_t *cbdata = malloc(sizeof(calendar_cb_data_t));

    cbdata->field = PROG_START_FIELD;
    cbdata->p = p;

    if(!calendar) {
        calendar = lv_calendar_create(lv_obj_get_parent(this));
        lv_calendar_header_arrow_create(calendar);
        lv_obj_add_event_cb(calendar, calendar_start_changed_cb, LV_EVENT_VALUE_CHANGED, cbdata);
    }

    struct tm *start_tm = localtime(sprinkler_prog_get_start(p));

    lv_calendar_set_showed_date(calendar, YEAR_EPOC+start_tm->tm_year, start_tm->tm_mon);
    lv_calendar_date_t days[1];
    days[0].day = start_tm->tm_mday;
    days[0].month = start_tm->tm_mon;
    days[0].year = YEAR_EPOC+start_tm->tm_year;

    lv_calendar_set_highlighted_dates(calendar, days, 1);
    lv_obj_align_to(calendar, start, LV_ALIGN_CENTER, 0, 9);
    lv_obj_remove_state(this, LV_STATE_FOCUSED);
    lv_obj_add_state(calendar, LV_STATE_FOCUSED);
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void screen_sprinkler_display(sprinkler_prog_t *activeProg) {
    const int lbl_width = 100;
    const int offset = 5;
    const int spacing = 3;
    const int scale_w_adjust = 14;

    // Create a screen object

    screen = lv_obj_create(NULL);
    LV_ASSERT_MALLOC(screen);

    if(!activeProg) activeProg = progs[0];

    lv_obj_add_event_cb(screen, screen_sprinkler_event_handler,
        LV_EVENT_ALL,
        NULL
    );

    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    nav_bar = lv_navbar_create(screen);
    lv_navbar_set_title(nav_bar, TITLE_SCREEN_SPRINKLER);
    lv_obj_add_flag(nav_bar, LV_OBJ_FLAG_EVENT_BUBBLE);

    parameter_list = lv_obj_create(screen);
    lv_obj_remove_style_all(parameter_list);
    lv_obj_set_size(parameter_list, LV_SIZE_CONTENT, 300);
    lv_obj_add_flag(parameter_list, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);

    lbl_name = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_name, "%s:", PROGRM_LABEL_NAME);
    lv_obj_set_width(lbl_name, LV_SIZE_CONTENT);

    name = lv_textarea_create(parameter_list);
    lv_textarea_set_text(name, sprinkler_prog_get_name(activeProg));
    lv_textarea_set_one_line(name, true);

    lv_obj_align(name, LV_ALIGN_TOP_LEFT, lbl_width+offset, 0);
    lv_obj_align_to(lbl_name, name, LV_ALIGN_OUT_LEFT_MID, -offset, 0);

    lbl_enable = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_enable, "%s:", PROGRM_LABEL_ENABLE);

    enable = lv_switch_create(parameter_list);
    lv_obj_add_state(enable, sprinkler_prog_get_enabled(activeProg)?LV_STATE_CHECKED:LV_STATE_DEFAULT);
    lv_obj_set_size(enable, 38, 19);

    lv_obj_align_to(enable, name, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);
    lv_obj_align_to(lbl_enable, enable, LV_ALIGN_OUT_LEFT_MID, -offset, 0);

    lbl_desc = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_desc, "%s:", PROGRM_LABEL_DESC);

    desc = lv_textarea_create(parameter_list);
    lv_textarea_set_text(desc, sprinkler_prog_get_description(activeProg));

    lv_obj_align_to(desc, enable, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);
    lv_obj_align_to(lbl_desc, desc, LV_ALIGN_OUT_LEFT_MID, -offset, 0);

    lbl_start = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_start, "%s:", PROGRM_LABEL_START);

    start = lv_textarea_create(parameter_list);
    lv_textarea_set_one_line(start, true);
    lv_obj_add_event_cb(start, start_clicked_cb, LV_EVENT_CLICKED, activeProg);

    lv_obj_align_to(start, desc, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);
    lv_obj_align_to(lbl_start, start, LV_ALIGN_OUT_LEFT_MID, -offset, 0);

    {
    char strTime[64] = {0};
    time_t *time = sprinkler_prog_get_start(activeProg);
    struct tm *time_val = localtime(time);
    strftime(strTime, 63, DEFAULT_TIME_FMT, time_val);
    lv_textarea_set_text(start, strTime);
    }

    lbl_end = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_end, "%s:", PROGRM_LABEL_END);

    end = lv_textarea_create(parameter_list);
    lv_textarea_set_one_line(end, true);

    lv_obj_align_to(end, start, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);
    lv_obj_align_to(lbl_end, end, LV_ALIGN_OUT_LEFT_MID, -offset, 0);

    {
    char strTime[64] = {0};
    time_t *time = sprinkler_prog_get_end(activeProg);
    struct tm *time_val = localtime(time);
    strftime(strTime, 63, DEFAULT_TIME_FMT, time_val);
    lv_textarea_set_text(end, strTime);
    }

    lbl_interval = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_interval, "%s (%s):", PROGRM_LABEL_INTERVAL, PROGRM_LABEL_INTERVAL_UNITS);

    interval = lv_spinbuttons_create(parameter_list);
    lv_spinbox_set_range(interval, 1, 30);
    lv_spinbox_set_value(interval, sprinkler_prog_get_interval(activeProg));
    lv_spinbox_set_digit_format(interval, 2, 0);
    lv_spinbox_step_prev(interval);

    lv_obj_align_to(interval, end, LV_ALIGN_OUT_BOTTOM_LEFT, 0, spacing);
    lv_obj_align_to(lbl_interval, interval, LV_ALIGN_OUT_LEFT_MID, -offset, 0);

    lbl_scale = lv_label_create(parameter_list);
    lv_label_set_text_fmt(lbl_scale, "%s:", PROGRM_LABEL_SCALE);

    scale = lv_slider_create(parameter_list);
    lv_obj_set_width(scale, lv_obj_get_width(desc)-2*scale_w_adjust);

    scale_label = lv_label_create(scale);
    lv_obj_align(scale_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(scale_label, "50%");
    lv_obj_set_style_text_color(scale_label, (lv_color_t)LV_COLOR_MAKE(0xff, 0xff, 0xff), 0);

    lv_obj_align_to(scale, interval, LV_ALIGN_OUT_BOTTOM_LEFT, scale_w_adjust, spacing+3);
    lv_obj_align_to(lbl_scale, scale, LV_ALIGN_OUT_LEFT_MID, -(offset+scale_w_adjust), 0);

    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 100, 0, true);
}
