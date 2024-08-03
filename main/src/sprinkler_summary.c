/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <time.h>
#include "lvgl.h"
#include "config.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lv_sprinkler_summery_class

#ifdef USE_24H_TIME
# define DEFAULT_TIME_FMT "%a %b %d %H:%M"
#else
# define DEFAULT_TIME_FMT "%a %b %d %l:%M %p"
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct lv_sprinkler_summery_s {
    lv_obj_t obj;               // base object data
    lv_obj_t * image;           // image to display
    lv_obj_t * name;            // description
    lv_obj_t * desc;            // description
    lv_obj_t * next_run_time;   // next run time
    sprinkler_prog_t * prog;
    char* time_fmt;             // time format string
} _lv_sprinkler_summery_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_constructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_destructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_event_cb(const lv_obj_class_t * class_p, lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_sprinkler_summery_class = {
    .name = "sprinkler_summary",

    .constructor_cb = &lv_constructor_cb,
    .destructor_cb = &lv_destructor_cb,
    //.event_cb = &lv_event_cb,

    .base_class = &lv_panel_class,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .instance_size = sizeof(_lv_sprinkler_summery_t),

    //.editable =
    //.group_def =
    .theme_inheritable = 1,
    //.user_data =
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
static void lv_constructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj) {
    LV_LOG_INFO("begin");
    const int width = class_p->width_def;
    const int pad = 5;
    const int imgSize = 64;

    _lv_sprinkler_summery_t *c = (_lv_sprinkler_summery_t *)obj;

    lv_obj_set_style_bg_opa(obj, 0x80, 0);

    c->image = lv_image_create(obj);
    lv_obj_set_size(c->image, imgSize, imgSize);
    lv_image_set_src(c->image, ASSET_DEFAULT_PROG_IMAGE);

    c->name = lv_label_create(obj);
    //lv_obj_add_flag(c->name, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_set_size(c->name, 3*imgSize, LV_SIZE_CONTENT);
    lv_label_set_long_mode(c->name, LV_LABEL_LONG_SCROLL);
    lv_obj_align_to(c->name, c->image, LV_ALIGN_TOP_LEFT, imgSize+pad, 0);

    c->desc = lv_label_create(obj);
    lv_obj_set_size(c->desc, 3*imgSize, LV_SIZE_CONTENT);
    lv_label_set_long_mode(c->desc, LV_LABEL_LONG_SCROLL);
    lv_obj_align_to(c->desc, c->image, LV_ALIGN_LEFT_MID, imgSize+pad, 0);

    c->next_run_time = lv_label_create(obj);
    lv_obj_set_size(c->next_run_time, 3*imgSize, LV_SIZE_CONTENT);
    lv_label_set_long_mode(c->next_run_time, LV_LABEL_LONG_SCROLL);
    lv_obj_align_to(c->next_run_time, c->image, LV_ALIGN_BOTTOM_LEFT, imgSize+pad, 0);

    c->time_fmt = calloc(strlen(DEFAULT_TIME_FMT)+1, sizeof(char));
    strcpy(c->time_fmt, DEFAULT_TIME_FMT);
}

static void lv_destructor_cb(const lv_obj_class_t * class_p, lv_obj_t * obj) {
    LV_LOG_INFO("begin");
    _lv_sprinkler_summery_t *c = (_lv_sprinkler_summery_t*)obj;
    lv_obj_delete(c->image);
    lv_obj_delete(c->next_run_time);
    lv_obj_delete(c->desc);
    lv_obj_delete(c->name);
    free(c->time_fmt);
}

static void lv_event_cb(const lv_obj_class_t * class_p, lv_event_t * e) {
    LV_LOG_INFO("begin");
}

lv_obj_t *sprinkler_summery_create(lv_obj_t * parent, sprinkler_prog_t *prog) {
    LV_LOG_INFO("begin");
    _lv_sprinkler_summery_t *obj = (_lv_sprinkler_summery_t*)lv_obj_class_create_obj(MY_CLASS, (lv_obj_t*)parent);
    LV_ASSERT_MALLOC(obj);
    lv_obj_class_init_obj((lv_obj_t*)obj);
    obj->prog = prog;
    sprinkler_summary_refresh(obj);
    return (lv_obj_t*)obj;
}

void sprinkler_summary_refresh(lv_obj_t *obj) {
    time_t startTime;
    char strTime[64] = {0};
    _lv_sprinkler_summery_t *_obj = (_lv_sprinkler_summery_t *)obj;

    if(!lv_obj_check_type(obj, &lv_sprinkler_summery_class)) return;
    if(!_obj->prog) return;

    if(sprinkler_prog_get_enabled(_obj->prog)) {
        lv_obj_add_style(obj, &style_sprinkler_summary_enabled, 0);
        lv_obj_remove_style(obj, &style_sprinkler_summary_disabled, 0);
    } else {
        lv_obj_remove_style(obj, &style_sprinkler_summary_enabled, 0);
        lv_obj_add_style(obj, &style_sprinkler_summary_disabled, 0);
    }

    lv_label_set_text(_obj->name, sprinkler_prog_get_name(_obj->prog));
    lv_label_set_text(_obj->desc, sprinkler_prog_get_description(_obj->prog));

    startTime = sprinkler_prog_get_next(_obj->prog);
    if(startTime!=0) {
        strftime(strTime, 63, _obj->time_fmt, localtime(&startTime));
        lv_label_set_text_fmt(_obj->next_run_time, "Next Run: %s", strTime);
    } else {
        lv_label_set_text(_obj->next_run_time, "Next Run: N/A");
    }
}
