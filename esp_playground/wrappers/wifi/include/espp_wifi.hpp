/**
 * @file espp_wifi.hpp
 *
 * @brief ESP WiFi Base Class
 *
 * This inits the Wifi TCP/IP and Wi-Fi interface as well as provides
 * common functionalities between the possible WiFi modes.
 *
 */
#pragma once

#include <stdio.h>

#include "esp_wifi.h"
#include "freertos/event_groups.h"

namespace espp {

class wifi_base
{
  private:
    /* ===================================================================== */
    /* Static Data Members */
    /* ===================================================================== */
    EventGroupHandle_t _s_wifi_event_group{};

    /* ===================================================================== */
    /* Private API */
    /* ===================================================================== */

    /**
     * @brief Handles wifi events
     * @param arg
     * @param event_base
     * @param event_id
     * @param event_data
     */
    static void _wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

    /**
     * @brief Handles IP events
     * @param arg
     * @param event_base
     * @param event_id
     * @param event_data
     */
    static void _ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

  public:
    /* ===================================================================== */
    /* Constructors/Destructors */
    /* ===================================================================== */
    static SemaphoreHandle_t _wifi_scan_complete_sem;

    /**
     * @brief
     */
    wifi_base();

    /**
     * @brief
     */
    ~wifi_base();

    /* ===================================================================== */
    /* Public API */
    /* ===================================================================== */

    esp_err_t scan_ap(void);
};

} // namespace espp