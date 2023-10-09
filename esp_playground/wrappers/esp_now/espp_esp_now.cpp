/**
 * @file espp_wifi.cpp
 *
 * @brief ESP WiFi Base Class
 *
 * This inits the Wifi TCP/IP and Wi-Fi interface as well as provides
 * common functionalities between the possible WiFi modes.
 *
 */
#include <stdio.h>

#include <esp_err.h>
#include <esp_event.h>
#include <esp_netif.h>
#include <esp_now.h>
#include <esp_system.h>
#include <esp_wifi.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#define ESPP_esp_now_TAG "ESP Wifi Base"

#include "espp_esp_now.hpp"

namespace espp {

/**
 * @brief Handles wifi events
 *
 * This is a virtual function that needs to be overloaded in derived class
 *
 * @param arg
 * @param event_base
 * @param event_id
 * @param event_data
 */
void esp_now::_wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ESP_LOGE(ESPP_esp_now_TAG, "%s: Not implemented", __func__);
}

/**
 * @brief Handles IP events
 *
 * This is a virtual function that needs to be overloaded in derived class
 *
 * @param arg
 * @param event_base
 * @param event_id
 * @param event_data
 */
void esp_now::_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ESP_LOGE(ESPP_esp_now_TAG, "%s: Not implemented", __func__);
}

/* ========================================================================= */
/* Constructors/Destructors */
/* ========================================================================= */

/**
 * @brief Default constructor
 */
esp_now::esp_now()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_start());
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_now_init());
}

/**
 * @brief Default destructor
 */
esp_now::~esp_now()
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_stop());
}

} // namespace espp