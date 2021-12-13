/**
 * @file
 * @brief       Methods to controll the height of the baloon
 *
 * @author      Niklas Mollerus
 *
 */

#ifndef HEIGHT_CONTROL_H
#define HEIGHT_CONTROL_H

#include <stdbool.h>

extern bool DEBUG_HEIGHT_CONTROL;

int setTargetHeight(int argc, char **argv);
int toggleHeightControl(int argc, char **argv);

void initializeHeightControl(float targetHeight, unsigned int refresh_rate_ms);

#endif