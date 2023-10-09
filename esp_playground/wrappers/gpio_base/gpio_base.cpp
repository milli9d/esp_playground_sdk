/**
 * @file gpio_base.cpp
 * @brief GPIO Base Class
 *
 * This wrapper provides
 */
#include <cstdbool>
#include <cstdint>
#include <cstdio>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "gpio_base.hpp"

namespace espp {
gpio_pin::gpio_pin(gpio_num_t pin, gpio_mode_t mode, espp::gpio_level_e value)
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

espp::gpio_level_e gpio_pin::toggle(void)
{
    espp::gpio_level_e out = static_cast<espp::gpio_level_e>(!gpio_get_level(_pin));

    gpio_set_level(_pin, out);
    return out;
}
} // namespace espp