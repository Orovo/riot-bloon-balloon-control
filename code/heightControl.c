#include "heightControl.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "xtimer.h"

#include "gps_data.h"
#include "dataAccess.h"

float targetHeight = 0; //meters over mean sea level
bool adjustHeight = true;


int setTargetHeight(int argc, char **argv) {
    if(argc > 0) {
        targetHeight = strtof(argv[0], NULL);
    }
    return 0;
}


int toggleHeightControl(int argc, char **argv) {
    if(argc > 0) {
        long temp = strtol(argv[0], NULL, 8);
        if(temp == 0) {
            adjustHeight = false;
        }
        else if(temp == 1) {
            adjustHeight = true;
        }
    }
    return 0;
}
/*
void *heightControlLoop(void *arg) {
    while(true) {
        if(adjustHeight) {
            //height controll enabled
            //get current position (gps data)
            //specify relative height
            //operate valves
            //wait
            struct gps_data data = {0}
            accessGPSData(&data);
            printf("lng: %f, lat: %f, vel: %f, hei: %f\n", data.gps.lng, data.gps.lat, data.gps.vel, data.gps.hei);
        }
        else {
            puts("\nNo Height Controll\n");
            //height controll disabled
        }
        xtimer_sleep(10);
    }
    return NULL;
}
*/