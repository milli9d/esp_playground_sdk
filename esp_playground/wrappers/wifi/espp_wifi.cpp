/**
 * @file espp_wifi.cpp
 *
 * @brief ESP WiFi Base Class
 *
 * This inits the Wifi TCP/IP and Wi-Fi interface as well as provides
 * common functionalities between the possible WiFi modes.
 *
 */
#include <cstdio>
#include <cstring>

#include <esp_err.h>
#include <esp_event.h>
#include <esp_netif.h>
#include <esp_system.h>
#include <esp_wifi.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#define ESPP_WIFI_BASE_TAG "ESP Wifi Base"

#include "espp_wifi.hpp"
#include "smart_timer.hpp"

namespace espp {

SemaphoreHandle_t wifi_base::_wifi_scan_complete_sem = xSemaphoreCreateBinary();

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
    switch (event_id) {
        case WIFI_EVENT_SCAN_DONE:
            /* give control back to scan handler */
            ESP_LOGI(ESPP_WIFI_BASE_TAG, "WiFi Scan Complete!");
            xSemaphoreGive(_wifi_scan_complete_sem);
            break;

        case WIFI_EVENT_STA_START:
            /* give control back to scan handler */
            ESP_LOGI(ESPP_WIFI_BASE_TAG, "WiFi STA Mode started!");
            break;

        case WIFI_EVENT_STA_STOP:
            /* give control back to scan handler */
            ESP_LOGI(ESPP_WIFI_BASE_TAG, "WiFi STA Mode stopped!");
            break;

        default:
            ESP_LOGE(ESPP_WIFI_BASE_TAG, "%s: Not implemented event_base = %s event_id = 0x%x", __func__, event_base,
                     event_id);
            break;
    }
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
    ESP_LOGE(ESPP_WIFI_BASE_TAG, "%s: Not implemented event_base = %s event_id = 0x%x", __func__, event_base, event_id);
}

/* ========================================================================= */
/* Public API */
/* ========================================================================= */

/**
 * @brief
 * @return
 */
esp_err_t wifi_base::scan_ap()
{
    /* start WiFi */
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_start());

    /* configure scan */
    wifi_scan_config_t cfg = {
        .ssid = NULL,
        .bssid = NULL,
        .show_hidden = false,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
    };

    /* set active scan time threshold */
    cfg.scan_time.active.max = 250u;
    cfg.scan_time.active.min = 100u;

    /* wait for scan to complete [NON-BLOCKING] */
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_scan_start(&cfg, false));

    /* stop scan */
    xSemaphoreTake(_wifi_scan_complete_sem, portMAX_DELAY);
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_scan_stop());

    /* count APs */
    uint16_t num_ap_found = 0u;
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_scan_get_ap_num(&num_ap_found));
    printf("Found %d APs!\n", num_ap_found);

    /* get all records */
    wifi_ap_record_t records[num_ap_found];
    memset(records, 0u, sizeof(records));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_scan_get_ap_records(&num_ap_found, records));

    /* print all found records */
    for (int i = 0u; i < num_ap_found; i++) {
        printf("[#%-2d] | SSID : %-33s | RSSI : %-2d db | Chan : %-2d |\n", i, (char*)records[i].ssid, records[i].rssi,
               records[i].primary);
    }

    /* stop WiFi */
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_stop());
    return 0;
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
    tcpip_adapter_init();

    /* init wifi */
    ESP_ERROR_CHECK(esp_wifi_init(&conf));

    /* init WiFi event handlers */
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, _wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, _ip_event_handler, NULL));

    ESP_LOGI(ESPP_WIFI_BASE_TAG, "Wifi subsystem up!");
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

    ESP_LOGI(ESPP_WIFI_BASE_TAG, "Wifi subsystem down!");
}

} // namespace espp