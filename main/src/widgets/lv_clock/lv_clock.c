
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_clock.h"
#include "stdio.h"
#include "time.h"

/*********************
 *      DEFINES
 *********************/
#define LV_CLOCK_CLASS &lv_clock_class
#define LV_CLOCK_MAXSTRFTIME_LEN 64

/**********************
 *      TYPEDEFS
 **********************/
typedef struct lv_clock_s {
    lv_obj_t obj;               // base object data
    lv_obj_t * display;         // label to display the time value

    lv_timer_t * update_timer;  // interval timer to update clock display

    char *fmt;                  // format to pass to strftime

} _lv_clock_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_clock_class = {
    .base_class = &lv_obj_class,
    .width_def = LV_PCT(100),
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(lv_clock_t)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC FUNCTIONS
 **********************/
void _lv_clock_interval_cb(lv_timer_t * inst) {
    time_t now;
    struct tm *timeinfo;
    lv_clock_t *obj;

    LV_ASSERT_NULL(inst);

    time(&now);
    timeinfo = localtime(&now);

    lv_clock_set_value(inst->user_data, timeinfo);
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * lv_clock_create(lv_obj_t *parent) {
    LV_LOG_INFO("lv_clock begin");

    lv_obj_t * obj = lv_obj_class_create_obj(&lv_clock_class,parent);
    lv_obj_class_init_obj(obj);

    LV_ASSERT_MALLOC(obj);
    if (obj == NULL) return NULL;

    lv_clock_t * _clock = (lv_clock_t *)obj;
    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);

    _clock->display = lv_label_create(obj);
    lv_obj_set_size(_clock->display, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(_clock->display, 1);
    lv_obj_set_align(_clock->display, LV_ALIGN_CENTER);

    return obj;
}

void lv_clock_set_interval(lv_obj_t * obj, uint32_t interval_ms) {
    LV_ASSERT_NULL(obj);
    if(!lv_obj_check_type(obj, &lv_clock_class))
        return;

    lv_clock_t *_obj = (lv_clock_t*)obj;

    if(interval_ms > 0) {
        // add or update interval timer
        if(_obj->update_timer) {
            lv_timer_set_period(_obj->update_timer, interval_ms);
        } else {
            _obj->update_timer = lv_timer_create(_lv_clock_interval_cb, interval_ms, (void*)obj);
        }
    } else {
        if(_obj->update_timer) {
            lv_timer_delete(_obj->update_timer);
            _obj->update_timer = NULL;
        }
    }
}

void lv_clock_set_value(lv_obj_t * obj, struct tm *timeinfo) {
    char timestr[LV_CLOCK_MAXSTRFTIME_LEN+1];

    if(!lv_obj_check_type(obj, &lv_clock_class))
        return;

    lv_clock_t * this = (lv_clock_t*)obj;

    if(this->fmt == NULL) {
        strftime(timestr, LV_CLOCK_MAXSTRFTIME_LEN, "%c", timeinfo);
    } else {
        strftime(timestr, LV_CLOCK_MAXSTRFTIME_LEN, this->fmt, timeinfo);
    }

    lv_label_set_text(this->display, timestr);
}
