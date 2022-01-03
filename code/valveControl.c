#include "valveControl.h"

#include <stdbool.h>

static uint16_t _valve_open_time = 0;
static kernel_pid_t _valve_thread_pid = KERNEL_PID_UNDEF;

static char _valve_thread_stack[THREAD_STACKSIZE_SMALL];


void wakeUpValveControlFor(uint16_t valve_open_time) {
    _valve_open_time = valve_open_time;
    thread_wakeup(_valve_thread_pid);
}

static void *valveControlThread(void *arg){
    (void)arg;
    while(true){
        thread_sleep();
        xtimer_msleep(_valve_open_time);
        gpio_clear(VALVE_DOWN_PIN);
        gpio_clear(VALVE_UP_PIN);
    }
    return NULL;
}

void initializeValveControl(void) {
    _valve_thread_pid = thread_create(_valve_thread_stack, sizeof(_valve_thread_stack), THREAD_PRIORITY_MAIN - 1, 0, valveControlThread, NULL, "Valve_Control_Thread");
    if(!pid_is_valid(_valve_thread_pid)) printf("Valve_Control_Thread creation failed");
}