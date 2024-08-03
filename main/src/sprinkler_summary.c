/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "stdio.h"
#include "time.h"
#include "lvgl.h"
#include "widgets.h"
#include "sprinkler.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lv_sprinkler_summery_class
#define DEFAULT_TIME_FMT "%c"

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
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);

    lv_obj_set_style_pad_all(lv_obj_get_parent(obj), pad, 0);

    c->image = lv_image_create(obj);
    lv_obj_set_size(c->image, imgSize, imgSize);

    c->name = lv_label_create(obj);
    lv_obj_add_flag(c->name, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_set_size(c->name, 3*imgSize, LV_SIZE_CONTENT);
    lv_label_set_long_mode(c->name, LV_LABEL_LONG_SCROLL);

    c->desc = lv_label_create(obj);
    lv_obj_set_size(c->desc, 3*imgSize, LV_SIZE_CONTENT);
    lv_label_set_long_mode(c->desc, LV_LABEL_LONG_SCROLL);

    c->next_run_time = lv_label_create(obj);
    lv_obj_set_size(c->next_run_time, 3*imgSize, LV_SIZE_CONTENT);
    lv_label_set_long_mode(c->next_run_time, LV_LABEL_LONG_SCROLL);

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
    char strTime[64] = {0};
    LV_LOG_INFO("begin");
    _lv_sprinkler_summery_t *obj = (_lv_sprinkler_summery_t*)lv_obj_class_create_obj(MY_CLASS, (lv_obj_t*)parent);
    LV_ASSERT_MALLOC(obj);
    lv_obj_class_init_obj((lv_obj_t*)obj);

    lv_label_set_text(obj->name, sprinkler_prog_get_name(prog));
    lv_label_set_text(obj->desc, sprinkler_prog_get_description(prog));

    strftime(strTime, 63, obj->time_fmt, sprinkler_prog_get_next(prog));
    lv_label_set_text(obj->next_run_time, strTime);

    obj->prog = prog;
    return (lv_obj_t*)obj;
}
