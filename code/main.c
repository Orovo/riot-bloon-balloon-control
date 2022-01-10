 /**
 * @file
 * @brief       Stuff
 *
 * @author      Michael Mylius, Karl Klemann, Johannes Nodop
 *
 * @}
 */

#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "embUnit.h"
#include "embUnit/embUnit.h"
#include "fmt.h"
#include "cbor.h"
#include "phydat.h"
#include "saul_reg.h"
#include "net/loramac.h"     /* core loramac definitions */
#include "semtech_loramac.h" /* package API */
#include "thread.h"
#include "xtimer.h"
#include "bmx280.h"
#include "time.h"
#include "msg.h"
#include "led.h"

#include "net/gcoap.h"
//#include "kernel_types.h"
#include "shell.h"

#include "heightControl.h"
#include "dataAccess.h"
#include "transferData.h"

#include "gps_data.h"
#include "periph/uart.h"
#include "cbor_util.h"

#define BUFSIZE 128
#define MSG_LENGTH 31
#define MAIN_QUEUE_SIZE (4)

bool DEBUG_GPS = true;
bool DEBUG_LORA = false;
bool DEBUG_ACCESS = false;
bool DEBUG_HEIGHT_CONTROL = true;

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

int debug_toggle(int argc, char **argv);
extern int gcoap_cli_cmd(int argc, char **argv);
extern void gcoap_cli_init(void);

static const shell_command_t shell_commands[] = {
    { "coap", "CoAP example", gcoap_cli_cmd },
    { "debug", "Toggle debug prints", debug_toggle },
    { "heightControl/set", "Sets the target height variable", setTargetHeight },
    { "heightControl/toggle", "Toggles the height control", toggleHeightControl },
    { NULL, NULL, NULL }
};

struct gps_data gps_data = {0};

bool join_procedure_succeeded = false;

//#define RECV_MSG_QUEUE                   (4U)
//static msg_t _recv_queue[RECV_MSG_QUEUE];
//static char _recv_stack[THREAD_STACKSIZE_DEFAULT];
// static char _test_stack[THREAD_STACKSIZE_DEFAULT];
// static char _height_control_stack[THREAD_STACKSIZE_DEFAULT];

// Downlink-Simulator EVERYTHING: 866474656D7064707265736367707364646174656474696D656368756D
// Downlink-Simulator TEMP & HUM: 826474656D706368756D   


semtech_loramac_t loramac;  /* The loramac stack device descriptor */
/* define the required keys for OTAA, e.g over-the-air activation (the
   null arrays need to be updated with valid LoRa values) */
static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = { 0x00, 0xC4, 0x96, 0xFE, 0x9F, 0x57, 0x28, 0x26 };
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x03, 0x7B, 0xCE };
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = { 0x5B, 0xF7, 0xA8, 0x26, 0x77, 0x3B, 0xE8, 0x15, 0x65, 0x55, 0x04, 0x06, 0xD8, 0xB3, 0xB6, 0xDD };

void sendMessage(char* message) {
    if (semtech_loramac_send(&loramac,
                             (uint8_t *)message, strlen(message)) != SEMTECH_LORAMAC_TX_DONE) {
        printf("Cannot send message '%s'\n", message);
    } else  {
        printf("Message '%s' was send!\n", message);
    }
}

int debug_toggle(int argc, char **argv)
{
    if (strcmp(argv[1], "gps") == 0) {
        printf("Debug toggle: gps => ");
        if (DEBUG_GPS) {
            printf("false");
            DEBUG_GPS = false;
        } else {
            printf("true");
            DEBUG_GPS = true;
        }
        printf("\n");
    } else if (strcmp(argv[1], "lora") == 0) {
        printf("Debug toggle: lora => ");
        if (DEBUG_LORA) {
            printf("false");
            DEBUG_LORA = false;
        } else {
            printf("true");
            DEBUG_LORA = true;
        }
        printf("\n");
    } else if (strcmp(argv[1], "access") == 0) {
        printf("Debug toggle: access => ");
        if (DEBUG_ACCESS) {
            printf("false");
            DEBUG_LORA = false;
        } else {
            printf("true");
            DEBUG_LORA = true;
        }
        printf("\n");
    } else if (strcmp(argv[1], "height_control") == 0) {
        printf("Debug toggle: height_control => ");
        if (DEBUG_HEIGHT_CONTROL) {
            printf("false");
            DEBUG_HEIGHT_CONTROL = false;
        } else {
            printf("true");
            DEBUG_HEIGHT_CONTROL = true;
        }
        printf("\n");
    }
    return 0;
}
/*
static void *_test_thread(void *arg){
    puts("Start of Test Thread");
    struct access_data data = {0};
    while(true) {
        int tmp = accessTotalData(&data);
        printf("accessTotalData = %s\n", (tmp == 0 ? "true" : "false"));
        printf("access_data {\n\tgps_data {\n\t\tgps {\n\t\t\tlng: %f\n\t\t\tlat: %f\n\t\t\tvel: %f\n\t\t\thei: %f\n\t\t}\n\t\ttime {\n\t\t\thour: %u\n\t\t\tmin: %u\n\t\t\tsec: %u\n\t\t\tmic: %u\n\t\t}\n\t\tdate {\n\t\t\td: %u\n\t\t\tm: %u\n\t\t\ty: %u\n\t\t}\n\t}\n\tatmospheric_data {\n\t\thum: %i\n\t\ttemp: %i\n\t\tpres: %i\n\t}\n}\n",
        data.gps.gps.lng, data.gps.gps.lat, data.gps.gps.vel, data.gps.gps.hei,
        data.gps.time.hour, data.gps.time.min, data.gps.time.sec, data.gps.time.mic,
        data.gps.date.d, data.gps.date.m, data.gps.date.y,
        data.atmospheric.humidity.val[0], data.atmospheric.temperature.val[0], data.atmospheric.pressure.val[0]);
        xtimer_sleep(10);
    }
    return NULL;
}
*/

person_t person = {
    .id = "0123456789ABC",
    .status = 42,
    .lat = 47,
    .lon = 11,
    .timestamp = 31415
    };
    

int main(void)
{

    gpio_init(GPIO_PIN(0,2), GPIO_OUT);
    gpio_init(GPIO_PIN(0,4), GPIO_OUT);

    initLEDs(GPIO_PIN(0,23), GPIO_PIN(0,18));
    setLEDColor(0, ORANGE);


    /* for the thread running the shell */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    gcoap_cli_init();
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    /* 1. initialize the LoRaMAC MAC layer */
    semtech_loramac_init(&loramac);
    /* 2. set the keys identifying the device */
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);
    /* 3. join the network */
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("LoRa join procedure failed");
        setLEDColor(0, RED);
        //return 1;
    } else {
        puts("LoRa join procedure succeeded");
        join_procedure_succeeded = true;
        setLEDColor(0, GREEN);
    }
    /* 3.5 Join succeded, create thread */


    // INIT GPS
    // kernel_pid_t lora_tid = thread_create(_send_stack, sizeof(_send_stack), THREAD_PRIORITY_MAIN - 1, 0, _periodic_send, NULL, "Send Thread");
    // initGPSData(lora_tid);
    // thread_create(_send_stack, sizeof(_send_stack), THREAD_PRIORITY_MAIN - 1, 0, _periodic_send, NULL, "Send Thread");
    //thread_create(_send_stack, sizeof(_send_stack), THREAD_PRIORITY_MAIN - 1, 0, _test_thread, NULL, "Test Thread");
    
    // thread_create(_height_control_stack, sizeof(_height_control_stack), THREAD_PRIORITY_MAIN - 1, 0, heightControLoop, NULL, "Height Control Loop");
    // hightest recordes windspeed 408km/h | using 450km/h = 125 m/s | accuracy of gps ~5m | therefore movement by 5m takes 0.04s = 40000 microseconds | is this even usefull?
    //initializeDataAccess(40000);
    //initializeHeightControl(100, 10000000);

    
    int bufsize = 128;
    uint8_t buffer[bufsize];
    uint8_t size;
    personToCbor(&person, buffer, bufsize, &size);
    for(int i = 0; i < size; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
