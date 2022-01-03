/**
 * @file
 * @brief       Methods to controll the valves of the baloon
 *
 * @author      Niklas Mollerus
 *
 */

#ifndef VALVE_CONTROL_H
#define VALVE_CONTROL_H

#include <stdint.h>
#include "thread.h"
#include "periph/gpio.h"
#include "xtimer.h"

int setValveOpenTime(uint16_t valve_open_time);

int wakeUpValveControl(void);

void initializeValveControl(void);

#endif