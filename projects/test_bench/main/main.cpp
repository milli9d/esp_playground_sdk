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

#include "espp_common.hpp"
#include "led_thread.hpp"
#include "smart_timer.hpp"

#define MAIN_TAG "MAIN"

#define SLEEP_PERIOD   2u
#define RESTART_PERIOD 10u

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Application entry point
 *
 */
void app_main()
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    /* pretty print intro */
    espp::_banner("WIFI TESTBENCH");
    /* print System information */
    espp::print_sys_info();

    /* init a debug LED */
    espp::led_thread_sys_t int_led = { .delay_ms = 100u, .pin = gpio_num_t::GPIO_NUM_4, .handle = NULL };
    xTaskCreate(espp::led_thread_run, "LED", 1024, (void*)&int_led, 2u, &int_led.handle);

    {
        /* init wifi base */
        espp::wifi_base _wifi = espp::wifi_base();

        _wifi.scan_ap();

        /* sleep for a while */
        for (int i = RESTART_PERIOD; i >= 0; i -= SLEEP_PERIOD) {
            ESP_LOGI(MAIN_TAG, "Restarting in %d seconds...\n", i);
            sleep(SLEEP_PERIOD);
        }
    }

    /* remove threads */
    vTaskDelete(int_led.handle);

    esp_restart();
}

#ifdef __cplusplus
}
#endif