#include "inttypes.h"
#include "float.h"

struct date {
    uint8_t d;
    uint8_t m;
    uint16_t y;
};

struct time {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint32_t mic;
};

struct gps {
    float lng;
    float lat;
    float vel;
};

struct gps_data {
    struct gps gps;
    struct time time;
    struct date date;
};

struct gps_data getGPSData(void);
void initGPSData(void);