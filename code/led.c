#include "led.h"

apa102_t dev;
color_rgba_t leds[N_LEDS];

int setLEDColor(uint8_t led, my_color color){
    if(led >= N_LEDS){
        return -1;
    }
    leds[led].alpha = LED_BRIGHTNESS;
    switch(color) {
        case OFF: {
            leds[led].color.b = 0;
            leds[led].color.g = 0;
            leds[led].color.r = 0;
            break;
        }
        case WHITE: {
            leds[led].color.b = 255;
            leds[led].color.g = 255;
            leds[led].color.r = 255;
            break;
        }
        case RED: {
            leds[led].color.b = 0;
            leds[led].color.g = 0;
            leds[led].color.r = 255;
            break;
        }
        case BLUE: {
            leds[led].color.b = 255;
            leds[led].color.g = 0;
            leds[led].color.r = 0;
            break;
        }
        case GREEN: {
            leds[led].color.b = 0;
            leds[led].color.g = 255;
            leds[led].color.r = 0;
            break;
        }
        case YELLOW: {
            leds[led].color.b = 0;
            leds[led].color.g = 255;
            leds[led].color.r = 255;
            break;
        }
        case ORANGE: {
            leds[led].color.b = 0;
            leds[led].color.g = 128;
            leds[led].color.r = 255;
            break;
        }
        case PURPLE: {
            leds[led].color.b = 255;
            leds[led].color.g = 0;
            leds[led].color.r = 128;
            break;
        }
        default: break;
    }
    apa102_load_rgba(&dev, leds);
    return 0;
}

my_color getLEDColor(uint8_t led) {
    if(led < N_LEDS) {
        if(leds[led].color.b == 0 && leds[led].color.g == 0 && leds[led].color.r == 0) return OFF;
        if(leds[led].color.b == 255 && leds[led].color.g == 255 && leds[led].color.r == 255) return WHITE;
        if(leds[led].color.b == 0 && leds[led].color.g == 0 && leds[led].color.r == 255) return RED;
        if(leds[led].color.b == 255 && leds[led].color.g == 0 && leds[led].color.r == 0) return BLUE;
        if(leds[led].color.b == 0 && leds[led].color.g == 255 && leds[led].color.r == 0) return GREEN;
        if(leds[led].color.b == 0 && leds[led].color.g == 255 && leds[led].color.r == 255) return YELLOW;
        if(leds[led].color.b == 0 && leds[led].color.g == 128 && leds[led].color.r == 255) return ORANGE;
        if(leds[led].color.b == 255 && leds[led].color.g == 0 && leds[led].color.r == 128) return PURPLE;
    }
    return -1;
}

void initLEDs(gpio_t data_pin, gpio_t clk_pin){
    /* initialize all LED color values to black (off) */
    memset(leds, 0, sizeof(color_rgba_t) * N_LEDS);
    // Set all LEDs to maximum brightness
    for(uint8_t i=0; i<N_LEDS; i++){
        leds[i].alpha = LED_BRIGHTNESS;
    }
    dev.led_numof = N_LEDS;
    dev.data_pin = data_pin;
    dev.clk_pin = clk_pin;
    gpio_init(dev.data_pin, GPIO_OUT);
    gpio_init(dev.clk_pin, GPIO_OUT);
    gpio_clear(dev.data_pin);
    gpio_clear(dev.clk_pin);
    apa102_load_rgba(&dev, leds);
}