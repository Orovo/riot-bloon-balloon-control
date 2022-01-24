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
    printf("person {\n\tid: %s\n\tstatus: %i\n\tlat: %f\n\tlon: %f\n\ttimestamp: %llu\n}",
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
void cborToPerson(uint8_t *sourceCbor, int source_length, person_t *destinationPerson) {
    CborParser parser;
    CborValue value;
    cbor_parser_init(sourceCbor, sizeof(sourceCbor), 0, &parser, &value);

    CborValue map;
    cbor_value_enter_container(&value, &map);
    if (!cbor_value_is_map(&map)) {//TODO Testoutput
        printf("ERROR - CborValue map is not of type map\n");
        return;
    }
   
    CborValue element;

    cbor_value_map_find_value(&map, "id", &element);
    if (!cbor_value_is_text_string(&element)) {//TODO Testoutput
        printf("ERROR - CborValue element is not text string - id\n");
        return;
    }
    size_t length = 14;
    cbor_value_copy_text_string(&element, destinationPerson->id, &length, NULL);

    cbor_value_map_find_value(&map, "status", &element);
    if (!cbor_value_is_integer(&element)) {//TODO Testoutput
        printf("ERROR - CborValue element is not integer - status\n");
        return;
    }
    cbor_value_get_int(&element, &(destinationPerson->status));

    cbor_value_map_find_value(&map, "lat", &element);
    if (!cbor_value_is_double(&element)) {//TODO Testoutput
    //if (!cbor_value_is_float(&element) {//TODO Testoutput
        //printf("ERROR - CborValue element is not float - lat\n");
        printf("ERROR - CborValue element is not double - lat\n");
        return;
    }
    //cbor_value_get_float(&element, destinationPerson->lat);
    cbor_value_get_double(&element, &(destinationPerson->lat));

    cbor_value_map_find_value(&map, "lon", &element);
    if (!cbor_value_is_double(&element)) {//TODO Testoutput
    //if (!cbor_value_is_float(&element) {//TODO Testoutput
        //printf("ERROR - CborValue element is not float - lon\n");
        printf("ERROR - CborValue element is not double - lon\n");
        return;
    }
    //cbor_value_get_float(&element, destinationPerson->lon);
    cbor_value_get_double(&element, &(destinationPerson->lon));

    cbor_value_map_find_value(&map, "timestamp", &element);
    if (!cbor_value_is_unsigned_integer(&element)) {//TODO Testoutput
        printf("ERROR - CborValue element is not simple type - timestamp\n");
        return;
    }
    cbor_value_get_uint64(&element, &(destinationPerson->timestamp));
}