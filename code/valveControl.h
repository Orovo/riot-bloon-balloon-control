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

#define VALVE_UP_PIN GPIO_PIN(0,4)
#define VALVE_DOWN_PIN GPIO_PIN(0,2)


void wakeUpValveControlFor(uint16_t valve_open_time);

void initializeValveControl(void);

#endif