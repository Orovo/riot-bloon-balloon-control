#include "heightControl.h"

#include <stdlib.h>
#include <stdio.h>

#include "xtimer.h"

#include "gps_data.h"
#include "dataAccess.h"

float _targetHeight = 0; //meters over mean sea level
bool _adjustHeight = true;

unsigned int _refresh_rate_ms = 0;

static char _height_control_stack[THREAD_STACKSIZE_DEFAULT];

int setTargetHeight(int argc, char **argv) {
    if(argc > 0) {
        _targetHeight = strtof(argv[0], NULL);
    }
    return 0;
}

int toggleHeightControl(int argc, char **argv) {
    if(argc > 0) {
        long temp = strtol(argv[0], NULL, 8);
        if(temp == 0) {
            _adjustHeight = false;
        }
        else if(temp == 1) {
            _adjustHeight = true;
        }
    }
    return 0;
}

void *heightControlThread(void *arg) {
    while(true) {
        if(_adjustHeight) {
            //height controll enabled
            //get current position (gps data)
            //specify relative height
            //operate valves
            //wait
            struct gps_data data = {0};
            accessGPSData(&data);
            if(DEBUG_HEIGHT_CONTROL) printf("lng: %f, lat: %f, vel: %f, hei: %f\n", data.gps.lng, data.gps.lat, data.gps.vel, data.gps.hei);
        }
        else {
            if(DEBUG_HEIGHT_CONTROL) puts("\nNo Height Controll\n");
            //height controll disabled
        }
        xtimer_sleep(_refresh_rate_ms);
    }
    return NULL;
}

void initializeHeightControl(float targetHeight, unsigned int refresh_rate_ms) {
    _targetHeight = targetHeight;
    _refresh_rate_ms = refresh_rate_ms;
    thread_create(_height_control_stack, sizeof(_height_control_stack), THREAD_PRIORITY_MAIN - 1, 0, heightControlThread, NULL, "Height_Control Thread");
}