#pragma once

#include <cstdio>
#include <cstdint>
#include <cstdbool>


#include "driver/gpio.h"

namespace ESPP
{

    enum gpio_level_e : uint32_t
    {
        GPIO_LEVEL_LOW = 0u,
        GPIO_LEVEL_HIGH = 1u
    };

    class gpio_pin
    {
    public:
        gpio_pin() = delete;
        ~gpio_pin();

        gpio_pin(gpio_num_t pin, gpio_mode_t mode, ESPP::gpio_level_e value);

        gpio_level_e toggle();

    private:
        gpio_config_t _sys{};
        gpio_num_t _pin{};
    };

}