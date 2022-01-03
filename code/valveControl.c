#include "valveControl.h"


static uint16_t _valve_open_time = 0;
static kernel_pid_t _valve_thread_pid = KERNEL_PID_UNDEF;

static char _valve_thread_stack[THREAD_STACKSIZE_SMALL];

int setValveOpenTime(uint16_t valve_open_time) {
    _valve_open_time = valve_open_time;
    return 0;
}

int wakeUpValveControl(void) {
    if(pid_is_valid(_valve_thread_pid)) return 1;
    thread_wakeup(_valve_thread_pid);
    return 0;
}

static void *valveControlThread(void *arg){
    (void)arg;
    while(1){
        thread_sleep();
        xtimer_msleep(_valve_open_time);
        printf("unset pint 0,2");
        gpio_clear(GPIO_PIN(0,2));
        printf("unset pint 0,4");
        gpio_clear(GPIO_PIN(0,4));
    }
    return NULL;
}

void initializeValveControl(void) {
    _valve_thread_pid = thread_create(_valve_thread_stack, sizeof(_valve_thread_stack), THREAD_PRIORITY_MAIN - 1, 0, valveControlThread, NULL, "Valve Control Thread");
}