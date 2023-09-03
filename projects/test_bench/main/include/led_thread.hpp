/**
 * @file main.cpp
 *
 * @brief ESP Playground test bench
 *
 */
#include <cstdio>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_spi_flash.h>
#include <esp_system.h>

#include <driver/gpio.h>
#include <driver/uart.h>

#include "espp_wifi.hpp"
#include "gpio_base.hpp"

#include "smart_timer.hpp"

#define LED_THREAD_TAG "Led Thread"

namespace espp {

typedef struct led_thread_sys_s {
    uint64_t delay_ms;
    gpio_num_t pin;
    TaskHandle_t handle;
} led_thread_sys_t;

void led_thread_run(void* args)
{
    led_thread_sys_t* sys = (led_thread_sys_t*)args;

    espp::gpio_pin led(sys->pin, GPIO_MODE_OUTPUT, espp::gpio_level_e::GPIO_LEVEL_HIGH);

    do {
        led.toggle();
        vTaskDelay(sys->delay_ms / portTICK_PERIOD_MS);
    }
    while (1);
}

} // namespace espp
