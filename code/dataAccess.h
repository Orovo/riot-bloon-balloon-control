/**
 * @file
 * @brief       Methods to get access to gps and other sensor data
 *
 * @author      Niklas Mollerus
 *
 */

#ifndef DATA_ACCESS_H
#define DATA_ACCESS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "xtimer.h"
#include "phydat.h"
#include "saul_reg.h"

#include "gps_data.h"

struct atmospheric_data {
    phydat_t humidity;
    phydat_t temperature;
    phydat_t pressure;
};

struct access_data {
    struct gps_data gps;
    struct atmospheric_data atmospheric;
};

int accessGPSData(struct gps_data *other);
int accessAtmosphericData(struct atmospheric_data *other);
int accessTotalData(struct access_data *other);

void initializeDataAccess(unsigned int microseconds);

#endif