/**
 * Copyright 2023
 *
 * Author: Milind Singh
 *
 * @brief GPIO Base Class
 *
 * This wrapper provides
 *
 */

#include <cstdio>
#include <cstdint>
#include <cstdbool>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gpio_base.hpp"

namespace ESPP
{
    gpio_pin::gpio_pin(gpio_num_t pin, gpio_mode_t mode, ESPP::gpio_level_e value)
    {
        _pin = pin;
        _sys.pin_bit_mask = BIT(pin);
        _sys.mode = mode;
        _sys.intr_type = gpio_int_type_t::GPIO_INTR_DISABLE;
        _sys.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE;
        _sys.pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE;

        if (gpio_config(&_sys) != ESP_OK) {
            printf("GPIO set up failed.\n");
        } else {
            gpio_set_level(_pin, value);
        }
    }

    gpio_pin::~gpio_pin()
    {
        printf("Wrecked GPIO Base\n");
    }

    ESPP::gpio_level_e gpio_pin::toggle(void)
    {
        ESPP::gpio_level_e out = static_cast<ESPP::gpio_level_e>(!gpio_get_level(_pin));

        gpio_set_level(_pin, out);
        return out;
    }
}