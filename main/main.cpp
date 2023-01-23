/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#include "esp_spi_flash.h"
#include "gpio_base.hpp"
#include "smart_timer.hpp"
#include "inky_eeprom.hpp"

#define MAIN_TAG "MAIN"

#define INKY_EEPROM_I2C_SCL           GPIO_NUM_5          /* gpio number for I2C master clock */
#define INKY_EEPROM_I2C_SDA           GPIO_NUM_4          /* gpio number for I2C master data  */
#define INKY_EEPROM_I2C_NUM           I2C_NUM_0           /* I2C port number for master dev */



namespace ESPP
{
    typedef struct led_thread_sys_s
    {
        uint64_t delay_ms;
        gpio_num_t pin;
        TaskHandle_t handle;
    } led_thread_sys_t;

    void print_sys_info(void)
    {
        /* Print chip information */
        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);
        printf("This is ESP8266 chip with %d CPU cores, WiFi, ",
               chip_info.cores);

        printf("silicon revision %d, ", chip_info.revision);

        printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
               (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    }

    void led_thread_run(void *args)
    {
        led_thread_sys_t *sys = (led_thread_sys_t *)args;

        ESPP::gpio_pin led(sys->pin, GPIO_MODE_OUTPUT, ESPP::gpio_level_e::GPIO_LEVEL_HIGH);

        do
        {
            led.toggle();
            vTaskDelay(sys->delay_ms / portTICK_PERIOD_MS);
        } while (1);
    }
}

#ifdef __cplusplus
extern "C"
{
#endif

    void app_main()
    {
        printf("Hello world!\n");

        ESPP::print_sys_info();

        ESPP::inky_eeprom_sys_t eeprom = {
            .scl = INKY_EEPROM_I2C_SCL,
            .sda = INKY_EEPROM_I2C_SDA,
            .i2c_port = INKY_EEPROM_I2C_NUM
        };

        ESPP::inky_eeprom reader(eeprom);


        ESPP::led_thread_sys_t ext_led = {
            .delay_ms = 50u,
            .pin = gpio_num_t::GPIO_NUM_16,
            .handle = NULL};

        xTaskCreate(ESPP::led_thread_run, "LED_ext", 1024u, (void *)&ext_led, 3u, &ext_led.handle);

        for (int i = 5; i >= 0; i--)
        {
            ESP_LOGI(MAIN_TAG, "Restarting in %d seconds...\n", i);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        vTaskDelete(ext_led.handle);
        esp_restart();
    }

#ifdef __cplusplus
}
#endif