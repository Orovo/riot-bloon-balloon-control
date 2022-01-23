#include "transferData.h"


#include <string.h>

#include "cbor_util.h"

static int personBuffer_size = 10;
static person_t personBuffer[10];
static int personBuffer_entries = 0;


void getPersonBuffer(person_t *destination) {
    memcpy(destination, &personBuffer, personBuffer_entries);
}

void addPersonToBuffer(person_t *person) {
    if(personBuffer_entries >= personBuffer_size) {
        for(int i = 1; i < personBuffer_size; i++) {
            memcpy(&personBuffer[i-1], &personBuffer[i], sizeof(*person));
        }
        personBuffer_entries--;
    }
    memcpy(&personBuffer[personBuffer_entries], person, sizeof(*person));
    personBuffer_entries++;
}

void printPerson(person_t* person) {
    printf("person {\n\tid: %s\n\tstatus: %i\n\tlat: %d\n\tlon: %d\n\ttimestamp: % u\n}",
        person->id, person->status, person->lat, person->lon, person->timestamp);
}

//To cbor conversion functions
void personToCbor(person_t *sourcePerson, uint8_t *destination, int destination_size, uint8_t *sizeOfCbor) {
    //TODO check destination_size for size - error if to large
    CborEncoder encoder;
    CborEncoder mapEncoder;
    cbor_encoder_init(&encoder, destination, destination_size, 0);
    cbor_encoder_create_map(&encoder, &mapEncoder, CborIndefiniteLength);

    addStringToMap("id", sourcePerson->id, &mapEncoder);
    addInt64ToMap("status", (int64_t) sourcePerson->status, &mapEncoder);
    addDoubleToMap("lat", sourcePerson->lat, &mapEncoder);
    addDoubleToMap("lon", sourcePerson->lon, &mapEncoder);
    addUInt64ToMap("timestamp", sourcePerson->timestamp, &mapEncoder);

    cbor_encoder_close_container_checked(&encoder, &mapEncoder);
    uint8_t buffer_size = cbor_encoder_get_buffer_size(&encoder, destination);

    *sizeOfCbor = buffer_size;
}

//From cbor conversion functions
/*void cborToPerson(uint8_t *sourceCbor, int source_length, person_t *destinationPerson) {

}*/