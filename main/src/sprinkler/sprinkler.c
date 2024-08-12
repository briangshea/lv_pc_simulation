#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sprinkler.h"
#include "linkedlist.h"

#ifndef SPRINKLER_MAX_NAME_LEN
# define SPRINKLER_MAX_NAME_LEN 64
#endif

#ifndef SPRINKLER_MAX_PROGS
# define SPRINKLER_MAX_PROGS 8
#endif

#ifndef SPRINKLER_MAX_ZONES
# define SPRINKLER_MAX_ZONES 8
#endif

#ifndef ASSERT_HANDLER
# define ASSERT_HANDLER while(1)
#endif

#ifndef SPKLR_ASSERT_LOG
# define SPKLR_ASSERT_LOG(...)
#endif

#define SPKLR_ASSERT(x) if((x)) {SPKLR_ASSERT_LOG(); ASSERT_HANDLER; }
#define SPKLR_ASSERT_NULL(x) if(NULL==(x)) { SPKLR_ASSERT_LOG(); ASSERT_HANDLER; }
#define SPKLR_ASSERT_MALLOC(x) if(NULL==(x)) { SPKLR_ASSERT_LOG(); ASSERT_HANDLER; }

#define SEND_PROG_EVENT(evt) _sprinkler_prog_send_event(p, evt)

#define SEND_ZONE_EVENT(evt) {                \
        if(z->zone_cb) {                      \
            sprinkler_zone_event_t e;         \
            e.type = (evt);                   \
            e.zone = z;                       \
            e.user_data = z->user_data;       \
            z->zone_cb(&e);                   \
        }                                     \
    }

static int prog_count = 0;
static int zone_count = 0;

/**
 * Sprinkler Program Model
 */
struct _sprinkler_prog_s {
    char *name;                     // name of the program
    char *description;              // description of the program

    sprinkler_time_t start;         // absolute program start date/time
    time_t next;                    // absolute next program run date/time
    time_t last;                    // absolute last program run date/time

    size_t interval;                // number of days between program runs
    int scale;                      // how much each zone's time will be scaled by 0 = 0 and 1000=100.0%
    int enabled;                    // Is the program enabled for running

    size_t zone_count;              // number of zones controlled by program
    sprinkler_zone_t *zones[SPRINKLER_MAX_ZONES];  // array of zones to be controlled by program, null terminated list

    //sprinkler_prog_event_cb program_cb; // callback to handle program events
    linkedlist_t *program_cb_list;

    void *user_data;
};

typedef struct {
    sprinkler_prog_event_cb cb;
    void *user_data;
} sprinkler_prob_cb_data_t;

/**
 * Sprinkler Zone Model
 */
struct _sprinkler_zone_s {
    time_t seconds;                  // number of seconds zone will run for.
    size_t scale;                    // how much each zone's time will be scaled by 0 = 0 and 1000=100.0%

    time_t last_run;                 // last run time
    time_t next_run;                 // next run time

    sprinkler_zone_event_cb zone_cb; // callback to handle zone events

    char *image;                     // zone image to display
    char *name;                      // zone name.
    char *description;               // description of the zone
    void *user_data;
};

struct _sprinkler_prog_event_s {
    sprinkler_event_type_t type;
    sprinkler_prog_t *prog;
    void * user_data;
};

struct _sprinkler_zone_event_s {
    sprinkler_event_type_t type;
    sprinkler_zone_t *zone;
    void * user_data;
};

static void _sprinkler_prog_send_event(sprinkler_prog_t *p, sprinkler_event_type_t evt) {
    if(p->program_cb_list) {
        sprinkler_prog_event_t e;
        e.type = (evt);
        e.prog = p;
        e.user_data = p->user_data;

        LL_FORWARD(p->program_cb_list, item) {
            sprinkler_prob_cb_data_t *cb = (sprinkler_prob_cb_data_t*)llnode_data(item);
            cb->cb(&e);
        }
    }
}


sprinkler_prog_t * sprinkler_prog_init(void) {
    sprinkler_prog_t *p = calloc(1, sizeof(sprinkler_prog_t));
    SPKLR_ASSERT_NULL(p);
    //memset(p, 0, sizeof(sprinkler_prog_t));
    prog_count++;
    p->program_cb_list = NULL;
    p->user_data = NULL;
    p->name = malloc(33);
    p->enabled = (prog_count==1);
    snprintf(p->name, 32, "Program %d", prog_count);
    p->description = malloc(33);
    snprintf(p->description, 32, "Program %d", prog_count);
    p->scale = 1000;
    p->interval = 1;
    return p;
}

void sprinkler_prog_delete(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    SEND_PROG_EVENT(SPRINKLER_EVENT_DELETED);

    if(p->program_cb_list) ll_destroy(p->program_cb_list, free);
    if(p->name) free(p->name);
    if(p->description) free(p->description);
    free(p);
}

sprinkler_prog_dsc_t sprinkler_prog_add_cb(sprinkler_prog_t *p, sprinkler_prog_event_cb program_cb, void *user_data) {
    SPKLR_ASSERT_NULL(p);
    if(!program_cb) return 0;

    sprinkler_prob_cb_data_t *newcb = (sprinkler_prob_cb_data_t*)calloc(1, sizeof(sprinkler_prob_cb_data_t));

    newcb->cb = program_cb;
    newcb->user_data = user_data;

    if(!p->program_cb_list)
        p->program_cb_list = ll_create();

    ll_push(p->program_cb_list, newcb);
    return (sprinkler_prog_dsc_t)newcb;
}

void sprinkler_prog_remove_cb(sprinkler_prog_t *p, sprinkler_prog_dsc_t dsc) {
    SPKLR_ASSERT_NULL(p);

    if(!p->program_cb_list)
        return;

    LL_FORWARD(p->program_cb_list, node) {
        sprinkler_prog_dsc_t curDsc = llnode_data(node);
        if(dsc == curDsc) {
            llnode_remove(p->program_cb_list, node, free);
            break;
        }
    }
}

size_t sprinkler_prog_get_interval(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return p->interval;
}

void sprinkler_prog_set_interval(sprinkler_prog_t *p, size_t interval) {
    SPKLR_ASSERT_NULL(p);
    p->interval = interval;
    SEND_PROG_EVENT(SPRINKLER_EVENT_CHANGED);
}

sprinkler_time_t const * sprinkler_prog_get_start(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return &p->start;
}

void sprinkler_prog_set_start(sprinkler_prog_t *p, sprinkler_time_t const * start) {
    struct tm *tm_next;
    time_t now = time(NULL);

    SPKLR_ASSERT_NULL(p);
    memcpy(&p->start, start,sizeof(p->start));

    tm_next = localtime(&now);

    if(start->hour<tm_next->tm_hour  ||
      (start->hour==tm_next->tm_hour && start->min<=tm_next->tm_min) ) {
        tm_next->tm_yday++;
        tm_next->tm_mday++;
    }

    tm_next->tm_hour = start->hour;
    tm_next->tm_min = start->min;

    p->next = mktime(tm_next);
    SEND_PROG_EVENT(SPRINKLER_EVENT_CHANGED);
}

time_t *sprinkler_prog_get_next(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return &p->next;
}

int sprinkler_prog_get_scale(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return p->scale;
}

void sprinkler_prog_set_scale(sprinkler_prog_t *p, int scale) {
    SPKLR_ASSERT_NULL(p);
    if(scale>SPRINKLER_SCALE_MAX) p->scale = SPRINKLER_SCALE_MAX;
    else if( scale < SPRINKLER_SCALE_MIN ) p->scale = SPRINKLER_SCALE_MIN;
    else p->scale = scale;
    SEND_PROG_EVENT(SPRINKLER_EVENT_CHANGED);
}

size_t sprinkler_prog_get_zone_count(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return p->zone_count;
}

const sprinkler_zone_t *sprinkler_prog_zone_at(sprinkler_prog_t *p, size_t at) {
    SPKLR_ASSERT_NULL(p);
    SPKLR_ASSERT(at<p->zone_count);
    return p->zones[at];
}

char *sprinkler_prog_get_name(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return p->name;
}

void sprinkler_prog_set_name(sprinkler_prog_t *p, char const * name) {
    SPKLR_ASSERT_NULL(p);
    p->name = realloc(p->name, strlen(name)+1);
    SPKLR_ASSERT_MALLOC(p->name);
    memcpy(p->name, name, strlen(name)+1);
    SEND_PROG_EVENT(SPRINKLER_EVENT_CHANGED);
}

char *sprinkler_prog_get_description(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return p->description;
}

void sprinkler_prog_set_description(sprinkler_prog_t *p, char const * description) {
    SPKLR_ASSERT_NULL(p);
    p->description = realloc(p->description, strlen(description)+1);
    SPKLR_ASSERT_MALLOC(p->description);
    memcpy(p->description, description, strlen(description)+1);
    SEND_PROG_EVENT(SPRINKLER_EVENT_CHANGED);
}

void sprinkler_prog_set_enabled(sprinkler_prog_t *p, int enabled) {
    SPKLR_ASSERT_NULL(p);
    p->enabled = (enabled!=0);
    SEND_PROG_EVENT(SPRINKLER_EVENT_CHANGED);
}

int sprinkler_prog_get_enabled(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return p->enabled;
}


sprinkler_zone_t * sprinkler_zone_init(sprinkler_zone_event_cb zone_cb, void *user_data) {
    sprinkler_zone_t *z = calloc(1, sizeof(sprinkler_zone_t));
    SPKLR_ASSERT_NULL(z);
    z->zone_cb = zone_cb;
    z->user_data = user_data;
    z->name = malloc(33);
    snprintf(z->name, 32, "Zone %d", zone_count);
    z->description = malloc(33);
    snprintf(z->description, 32, "Zone %d", zone_count);
    z->scale = 1000;
    return z;
}

void sprinkler_zone_delete(sprinkler_zone_t *z) {
    if(!z) return;
    SEND_ZONE_EVENT(SPRINKLER_EVENT_DELETED);
    if(z->image) free(z->image);
    if(z->name) free(z->name);
    if(z->description) free(z->description);
    free(z);
}

time_t sprinkler_zone_get_duration(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return z->seconds;
}

void sprinkler_zone_set_duration(sprinkler_zone_t *z, time_t duration) {
    SPKLR_ASSERT_NULL(z);
    z->seconds = duration;
    SEND_ZONE_EVENT(SPRINKLER_EVENT_CHANGED);
}

int sprinkler_zone_get_scale(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return z->scale;
}

void sprinkler_zone_set_scale(sprinkler_zone_t *z, int scale) {
    SPKLR_ASSERT_NULL(z);
    if(scale > 10000) z->scale = 1000;
    else if(scale < 0 ) z->scale = 0;
    else z->scale = scale;
    SEND_ZONE_EVENT(SPRINKLER_EVENT_CHANGED);
}

time_t sprinkler_zone_last_run(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return z->last_run;
}

time_t sprinkler_zone_next_run(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return z->next_run;
}

const char * sprinkler_zone_get_image(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return z->image;
}

void sprinkler_zone_set_image(sprinkler_zone_t *z, char * image) {
    SPKLR_ASSERT_NULL(z);
    z->image = realloc(z->image, strlen(image)+1);
    memcpy(z->image, image, strlen(image)+1);
    SEND_ZONE_EVENT(SPRINKLER_EVENT_CHANGED);
}

const char * sprinkler_zone_get_name(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return z->name;
}

void sprinkler_zone_set_name(sprinkler_zone_t *z, char * name) {
    SPKLR_ASSERT_NULL(z);
    z->name = realloc(z->name, strlen(name)+1);
    SPKLR_ASSERT_MALLOC(z->name);
    memcpy(z->name, name, strlen(name)+1);
    SEND_ZONE_EVENT(SPRINKLER_EVENT_CHANGED);
}

const char * sprinkler_zone_get_description(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return z->description;
}

void sprinkler_zone_set_description(sprinkler_zone_t *z, char *description) {
    SPKLR_ASSERT_NULL(z);
    z->description = realloc(z->description, strlen(description)+1);
    SPKLR_ASSERT_MALLOC(z->description);
    memcpy(z->description, description, strlen(description)+1);
    SEND_ZONE_EVENT(SPRINKLER_EVENT_CHANGED);
}


sprinkler_event_type_t sprinkler_prog_event_get_type(sprinkler_prog_event_t *e) {
    SPKLR_ASSERT_NULL(e);
    return e->type;
}

sprinkler_prog_t * sprinkler_prog_event_get_prog(sprinkler_prog_event_t *e) {
    SPKLR_ASSERT_NULL(e);
    return e->prog;
}

void * sprinkler_prog_event_get_user_data(sprinkler_prog_event_t *e) {
    SPKLR_ASSERT_NULL(e);
    return e->user_data;
}


sprinkler_event_type_t sprinkler_zone_event_get_type(sprinkler_zone_event_t *e) {
    SPKLR_ASSERT_NULL(e);
    return e->type;
}

sprinkler_zone_t * sprinkler_zone_event_get_zone(sprinkler_zone_event_t *e) {
    SPKLR_ASSERT_NULL(e);
    return e->zone;
}

void * sprinkler_zone_event_get_user_data(sprinkler_zone_event_t *e) {
    SPKLR_ASSERT_NULL(e);
    return e->user_data;
}
