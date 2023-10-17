/**
 * @file main.cpp
 *
 * @brief ESP Playground test bench
 *
 */

#include <cstdio>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>
#include <esp_spi_flash.h>
#include <esp_system.h>

#include <driver/gpio.h>
#include <driver/uart.h>

#include "espp_esp_now.hpp"
#include "espp_wifi.hpp"
#include "gpio_base.hpp"

#include "espp_common.hpp"
#include "espp_shell.hpp"
#include "led_thread.hpp"
#include "smart_timer.hpp"

#include "lvgl.h"

#define MAIN_TAG "MAIN"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Application entry point
 *
 */
void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    /* pretty print intro */
    espp::_banner("WIFI TESTBENCH");

    /* print System information */
    espp::print_sys_info();

    /* init a debug LED */
    espp::led_thread_sys_t int_led = { .delay_ms = 100u, .pin = gpio_num_t::GPIO_NUM_4, .handle = NULL };
    xTaskCreate(espp::led_thread_run, "LED", 2048, (void*)&int_led, 2u, &int_led.handle);

    /* init shell */
    espp::espp_shell _shell;

    /* init wifi base */
    espp::wifi_base _wifi = espp::wifi_base();

    _wifi.start();
    espp::esp_now _esp_now = espp::esp_now();
    _wifi.stop();

    /* yield main task */
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    };
}

#ifdef __cplusplus
}
#endif