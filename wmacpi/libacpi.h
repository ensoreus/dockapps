#ifndef _LIBACPI_H_
#define _LIBACPI_H_


#define LIBACPI_VER "0.90"

/* Here because we need it for definitions in this file . . . */
#define MAX_NAME 128
#define MAXBATT 8
#define SAMPLES 50

typedef enum {
    REMAIN,
    TIMER
} DspMode;

typedef enum {
    AC,
    BATT,
    PS_ERR,
} power_state_t;
    
typedef enum {
    HIGH,
    MED,
    LOW,
    CRIT,
    HARD_CRIT,
    BS_ERR,
} batt_state_t;

typedef enum {
    CHARGE,
    DISCHARGE,
    CH_ERR,
} charge_state_t;

typedef enum {
    OK,
    CRITICAL,
    CS_ERR,
} cap_state_t;

typedef struct {
    /* general info */
    char name[MAX_NAME];
    /* these two are conveniences */
    char info_file[MAX_NAME];
    char state_file[MAX_NAME];
    int present; 
    int design_cap;		/* assuming mAh */
    int last_full_cap;
    int design_voltage;		/* in mV */
    /* state info */
    cap_state_t capacity_state;
    charge_state_t charge_state;
    int present_rate;		/* in mAh */
    int remaining_cap;		/* in mAh */
    int present_voltage;	/* in mV */
    /* calculated states */
    batt_state_t state;
    int percentage;		/* stored here because this is a per battery thing */
    int charge_time;		/* time left to charge this battery */
    /* and a flag to indicate that this is valid . . . */
    int valid;
} battery_t;
    
typedef struct {
    char *name;
    char state_file[MAX_NAME];
    power_state_t power;
} adapter_t;

typedef struct {
    adapter_t adapter;
    int rtime;			/* remaining time */
    int timer;			/* how long been on battery? */
    int crit_level;		/* anything below this is critical low */
    battery_t *binfo;		/* pointer to the battery being monitored */
} APMInfo;

/*
 * Note that there are some serious problems with this: firstly, handling of
 * multiple batteries sucks. I've cleaned it up a reasonable amount so far,
 * but I don't know enough about how multiple batteries are handled in the
 * actual power management code to be able to do it right. I need to plug
 * in the second battery for this LifeBook to see how it goes . . .
 *
 * Moving percentage to the battery is right, but I think we need a global
 * remaining capacity somewhere, too . . . 
 */

/*
 * To provide a convenient debugging function . . . 
 */

static int verbosity = 0;

#define eprint(level, fmt, arg...)					\
    do {								\
	if (level > verbosity) {					\
	    switch (level) {						\
	    case 0:							\
		break;							\
	    case 1:							\
		fprintf(stderr, fmt, ##arg);				\
		 break;							\
	    default:							\
		fprintf(stderr, "%s: " fmt, __FUNCTION__, ##arg);	\
		fprintf(stderr, "\n");					\
		break;							\
	    }								\
	}								\
    } while (0)

/* since these /are/ needed here . . . */
battery_t batteries[MAXBATT];
int batt_count;

/* check if apm/acpi is enabled, etc */
int power_init(void);
/* fill APMInfo with data */
void acquire_batt_info(int);
void acquire_all_batt_info(void);
void acquire_global_info(void);
void acquire_all_info(void);

#endif /* _WMACPI_H_ */
