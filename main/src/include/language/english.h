#ifndef english_H
#define english_H

#ifdef USE_24H_TIME
# define DEFAULT_TIME_FMT "%a %b %d %H:%M"
#else
# define DEFAULT_TIME_FMT "%a %b %d %l:%M %p"
#endif


#define TITLE_SCREEN_HOME        "Home"
#define TITLE_SCREEN_VIEW_SELECT "System Apps"
#define TITLE_SCREEN_SPRINKLER   "Sprinkler Settings"

#define PROGRM_LABEL_NAME           "Name"
#define PROGRM_LABEL_ENABLE         "Enable"
#define PROGRM_LABEL_DESC           "Description"
#define PROGRM_LABEL_START          "Start Date"
#define PROGRM_LABEL_END            "End Date"
#define PROGRM_LABEL_INTERVAL       "Interval"
#define PROGRM_LABEL_INTERVAL_UNITS "Days"
#define PROGRM_LABEL_SCALE          "Scale"

#endif // english_H
