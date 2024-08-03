#ifndef sprinkler_H
#define sprinkler_H

#include <stdlib.h>
#include <time.h>

typedef struct _sprinkler_prog_s sprinkler_prog_t;
typedef struct _sprinkler_zone_s sprinkler_zone_t;

typedef enum {
    SPRINKLER_EVENT_CHANGED = 0,
    SPRINKLER_EVENT_STARTED,
    SPRINKLER_EVENT_ENDED,
    SPRINKLER_EVENT_CREATED,
    SPRINKLER_EVENT_DELETED
} sprinkler_event_type_t;

typedef struct {
    sprinkler_event_type_t type;
    sprinkler_prog_t *prog;
    void * user_data;
} sprinkler_prog_event_t;

typedef struct {
    sprinkler_event_type_t type;
    sprinkler_zone_t *zone;
    void * user_data;
} sprinkler_zone_event_t;

typedef void (*sprinkler_prog_event_cb)(sprinkler_prog_event_t *e);
typedef void (*sprinkler_zone_event_cb)(sprinkler_zone_event_t *e);

sprinkler_prog_t * sprinkler_prog_init(sprinkler_prog_event_cb program_cb, void *user_data);
void sprinkler_prog_delete(sprinkler_prog_t *p);

size_t sprinkler_prog_get_interval(sprinkler_prog_t *p);
void sprinkler_prog_set_interval(sprinkler_prog_t *p, size_t interval);
const struct tm * sprinkler_prog_get_start(sprinkler_prog_t *p);
void sprinkler_prog_set_start(sprinkler_prog_t *p, struct tm *start);
const struct tm * sprinkler_prog_get_end(sprinkler_prog_t *p);
void sprinkler_prog_set_end(sprinkler_prog_t *p, struct tm *end);
const struct tm * sprinkler_prog_get_next(sprinkler_prog_t *p);
int sprinkler_prog_get_scale(sprinkler_prog_t *p);
void sprinkler_prog_set_scale(sprinkler_prog_t *p, int scale);
size_t sprinkler_prog_get_zone_count(sprinkler_prog_t *p);
const sprinkler_zone_t *sprinkler_prog_zone_at(sprinkler_prog_t *p, size_t at);
char *sprinkler_prog_get_name(sprinkler_prog_t *p);
void sprinkler_prog_set_name(sprinkler_prog_t *p, char *name);
char *sprinkler_prog_get_description(sprinkler_prog_t *p);
void sprinkler_prog_set_description(sprinkler_prog_t *p, char *description);

sprinkler_zone_t * sprinkler_zone_init(sprinkler_zone_event_cb zone_cb, void *user_data);
void sprinkler_zone_delete(sprinkler_zone_t *);
size_t sprinkler_zone_get_duration(sprinkler_zone_t *z);
void sprinkler_zone_set_duration(sprinkler_zone_t *z, size_t duration);
int sprinkler_zone_get_scale(sprinkler_zone_t *z);
void sprinkler_zone_set_scale(sprinkler_zone_t *z, int scale);
const struct tm * sprinkler_zone_last_run(sprinkler_zone_t *z);
const struct tm * sprinkler_zone_next_run(sprinkler_zone_t *z);
const char * sprinkler_zone_get_image(sprinkler_zone_t *z);
void sprinkler_zone_set_image(sprinkler_zone_t *z, char * image);
const char * sprinkler_zone_get_name(sprinkler_zone_t *z);
void sprinkler_zone_set_name(sprinkler_zone_t *z, char * name);
const char * sprinkler_zone_get_description(sprinkler_zone_t *z);
void sprinkler_sone_set_description(sprinkler_zone_t *z, char *description);

#endif //sprinkler_H
