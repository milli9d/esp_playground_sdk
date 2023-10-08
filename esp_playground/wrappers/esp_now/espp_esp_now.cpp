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
#include <esp_system.h>
#include <esp_wifi.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#define ESPP_WIFI_BASE_TAG "ESP Wifi Base"

#include "espp_wifi.hpp"

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
void wifi_base::_wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ESP_LOGE(ESPP_WIFI_BASE_TAG, "%s: Not implemented", __func__);
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
void wifi_base::_ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ESP_LOGE(ESPP_WIFI_BASE_TAG, "%s: Not implemented", __func__);
}

/* ========================================================================= */
/* Constructors/Destructors */
/* ========================================================================= */

/**
 * @brief Default constructor
 */
wifi_base::wifi_base()
{
    /* bind default config to an rvalue */
    wifi_init_config_t conf = WIFI_INIT_CONFIG_DEFAULT();

    /* init WiFi Event Handlers group */
    _s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* init TCP/IP adapter */
    // tcpip_adapter_init();

    /* init wifi */
    ESP_ERROR_CHECK(esp_wifi_init(&conf));

    /* init WiFi event handlers */
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, _wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, _ip_event_handler, NULL));

    ESP_LOGI(ESPP_WIFI_BASE_TAG, "Wifi subsystem up!\n");
}

/**
 * @brief Default destructor
 */
wifi_base::~wifi_base()
{
    /* disconnect wifi */
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_disconnect());

    /* de-init wifi resources */
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_deinit());

    /* remove event group */
    vEventGroupDelete(_s_wifi_event_group);

    ESP_LOGI(ESPP_WIFI_BASE_TAG, "Wifi subsystem down!\n");
}

} // namespace espp