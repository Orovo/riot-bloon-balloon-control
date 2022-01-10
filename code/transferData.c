#include "transferData.h"


#include <string.h>

#include "cbor_util"


static person_t personBuffer = [10];

void personToCbor(person_t sourcePerson, uint8_t *destination, int dest_length) {


    uint8_t buffer[dest_length] = {0};
    CborEncoder* encoder
    CborEncoder* mapEncoder;
    cbor_encoder_init(encoder, buffer, sizeof(buffer), 0);
    cbor_encoder_create_map(encoder, mapEncoder, CborIndefiniteLength);

    addCharArrayToMap("id", sourcePerson.id, 14/*TODO - size of id*/, mapEncoder);
    addInt64ToMap("status", (int64_t) sourcePerson.status, mapEncoder);
    addDoubleToMap("lat", sourcePerson.lat, mapEncoder);
    addDoubleToMap("lon", sourcePerson.lon, mapEncoder);
    addUInt64ToMap("timestamp", sourcePerson.timestamp, mapEncoder);

    cbor_encoder_close_container_checked(encoder, mapEncoder);
    uint8_t buffer_size = cbor_encoder_get_buffer_size(encoder, buffer);

    memcpy(destination, buffer, buffer_size);
}

void cborToPerson() {

}