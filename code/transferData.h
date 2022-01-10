/**
 * @file
 * @brief       Methods and structs to define and handle data transfered between baloons
 *
 * @author      Niklas Mollerus
 * @author      Youssef Benlemlih
 * @author      Philip Gisella
 */

#ifndef TRANSFER_DATA_H
#define TRANSFER_DATA_H


#include <stdint.h>


struct person
{
    char id[14];
    int status;
    double lat;
    double lon;
    uint64_t timestamp;
};
typedef struct person person_t;

void getPersonBuffer(person_t *destination);
void addPersonToBuffer(person_t *person);
void personToCbor(person_t *sourcePerson, uint8_t *destination);


#endif
