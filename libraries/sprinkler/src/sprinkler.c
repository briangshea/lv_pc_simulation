#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sprinkler.h"

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

#define SEND_PROG_EVENT(evt) {                \
        if(p->program_cb) {                   \
            sprinkler_prog_event_t e;         \
            e.type = (evt);                   \
            e.prog = p;                       \
            e.user_data = p->user_data;       \
            p->program_cb(&e);                \
        }                                     \
    }

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
    size_t interval;                // number of days between program runs

    struct tm start;                // absolute program start date/time
    struct tm end;                  // absolute program end date/time
    struct tm next;                 // absolute next program run date/time
    struct tm last;                 // absolute last program run date/time

    int scale;                      // how much each zone's time will be scaled by 0 = 0 and 1000=100.0%

    size_t zone_count;              // number of zones controlled by program
    sprinkler_zone_t *zones[SPRINKLER_MAX_ZONES];  // array of zones to be controlled by program, null terminated list
    sprinkler_prog_event_cb program_cb; // callback to handle program events
    char *name;                     // name of the program
    char *description;              // description of the program
    void *user_data;
};

/**
 * Sprinkler Zone Model
 */
struct _sprinkler_zone_s {
    size_t seconds;                 // number of seconds zone will run for.
    size_t scale;                   // how much each zone's time will be scaled by 0 = 0 and 1000=100.0%

    //int gpio_num;                   // gpio_num to enable for zone control.

    struct tm last_run;             // last run time
    struct tm next_run;             // next run time

    sprinkler_zone_event_cb zone_cb; // callback to handle zone events
    void *user_data;
    char *image;                    // zone image to display
    char *name;                     // zone name.
    char *description;              // description of the zone
};

sprinkler_prog_t * sprinkler_prog_init(sprinkler_prog_event_cb program_cb, void *user_data) {
    sprinkler_prog_t *p = calloc(1, sizeof(sprinkler_prog_t));
    SPKLR_ASSERT_NULL(p);
    //memset(p, 0, sizeof(sprinkler_prog_t));
    prog_count++;
    p->program_cb = program_cb;
    p->user_data = user_data;
    p->name = malloc(33);
    snprintf(p->name, 32, "Program %d", prog_count);
    p->description = malloc(33);
    snprintf(p->description, 32, "Program %d", prog_count);
    p->scale = 1000;
    p->interval = 1;
    SEND_PROG_EVENT(SPRINKLER_EVENT_CREATED);
    return p;
}

void sprinkler_prog_delete(sprinkler_prog_t *p) {
    if(!p) return;
    SEND_PROG_EVENT(SPRINKLER_EVENT_DELETED);
    if(p->name) free(p->name);
    if(p->description) free(p->description);
    free(p);
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

const struct tm * sprinkler_prog_get_start(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return &p->start;
}

void sprinkler_prog_set_start(sprinkler_prog_t *p, struct tm *start) {
    SPKLR_ASSERT_NULL(p);
    memcpy(&p->start, start, sizeof(p->start));
    SEND_PROG_EVENT(SPRINKLER_EVENT_CHANGED);
}

const struct tm * sprinkler_prog_get_end(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return &p->end;
}

void sprinkler_prog_set_end(sprinkler_prog_t *p, struct tm *end) {
    SPKLR_ASSERT_NULL(p);
    memcpy(&p->end, end, sizeof(p->end));
    SEND_PROG_EVENT(SPRINKLER_EVENT_CHANGED);
}

const struct tm * sprinkler_prog_get_next(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return &p->next;
}

int sprinkler_prog_get_scale(sprinkler_prog_t *p) {
    SPKLR_ASSERT_NULL(p);
    return p->scale;
}

void sprinkler_prog_set_scale(sprinkler_prog_t *p, int scale) {
    SPKLR_ASSERT_NULL(p);
    if(scale>10000) p->scale = 10000;
    else if( scale < 0 ) p->scale = 0;
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

void sprinkler_prog_set_name(sprinkler_prog_t *p, char *name) {
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

void sprinkler_prog_set_description(sprinkler_prog_t *p, char *description) {
    SPKLR_ASSERT_NULL(p);
    p->name = realloc(p->description, strlen(description)+1);
    SPKLR_ASSERT_MALLOC(p->description);
    memcpy(p->description, description, strlen(description)+1);
    SEND_PROG_EVENT(SPRINKLER_EVENT_CHANGED);
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
    SEND_ZONE_EVENT(SPRINKLER_EVENT_CREATED);
}

void sprinkler_zone_delete(sprinkler_zone_t *z) {
    if(!z) return;
    SEND_ZONE_EVENT(SPRINKLER_EVENT_DELETED);
    if(z->image) free(z->image);
    if(z->name) free(z->name);
    if(z->description) free(z->description);
    free(z);
}

size_t sprinkler_zone_get_duration(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return z->seconds;
}

void sprinkler_zone_set_duration(sprinkler_zone_t *z, size_t duration) {
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

const struct tm * sprinkler_zone_last_run(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return &z->last_run;
}

const struct tm * sprinkler_zone_next_run(sprinkler_zone_t *z) {
    SPKLR_ASSERT_NULL(z);
    return &z->next_run;
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

void sprinkler_sone_set_description(sprinkler_zone_t *z, char *description) {
    SPKLR_ASSERT_NULL(z);
    z->description = realloc(z->description, strlen(description)+1);
    SPKLR_ASSERT_MALLOC(z->description);
    memcpy(z->description, description, strlen(description)+1);
    SEND_ZONE_EVENT(SPRINKLER_EVENT_CHANGED);
}
