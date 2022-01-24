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
#include <stdlib.h>


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
void printPerson(person_t* person);
void personToCbor(person_t *sourcePerson, uint8_t *destination, int destination_size, uint8_t *sizeOfCbor);
void cborToPerson(uint8_t* sourceCbor, int source_length, person_t* destinationPerson);

#endif
