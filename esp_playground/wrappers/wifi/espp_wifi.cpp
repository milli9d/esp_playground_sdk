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
#define ESPP_WIFI_BASE_TAG "ESPP Wifi Base"

#include "espp_wifi.hpp"
#include "smart_timer.hpp"

namespace espp {

/* @todo figure out a better way to do this */
SemaphoreHandle_t wifi_base::_wifi_scan_complete_sem = xSemaphoreCreateBinary();
wifi_base* espp::wifi_base::_this{};
wifi_config_t espp::wifi_base::_s_wifi_config{};

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

        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGW(ESPP_WIFI_BASE_TAG, "WiFi Station disconnected from AP!");
            esp_wifi_set_event_mask(WIFI_EVENT_STA_START);
            break;

        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(ESPP_WIFI_BASE_TAG, "WiFi Station connected to AP!");
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
    switch (event_id) {
        case IP_EVENT_STA_GOT_IP:
            /* give control back to scan handler */
            ESP_LOGI(ESPP_WIFI_BASE_TAG, "WiFi connected!");
            break;

        default:
            ESP_LOGE(ESPP_WIFI_BASE_TAG, "%s: Not implemented event_base = %s event_id = 0x%x", __func__, event_base,
                     event_id);
            break;
    }
}

/* ========================================================================= */
/* WiFi Thread */
/* ========================================================================= */

void wifi_base::thread_run(void* args)
{
    while (1) {
        vTaskDelay(500u / portTICK_PERIOD_MS);
    }
}

/* ========================================================================= */
/* Shell Commands */
/* ========================================================================= */

/**
 * @brief Start AP shell command
 * @param argc
 * @param argv
 * @return
 */
int wifi_base::_start_wifi_shell(int argc, char** argv)
{
    ESP_ERROR_CHECK(_this == NULL);
    _this->start();
    return 0;
}

/**
 * @brief Stop AP shell command
 * @param argc
 * @param argv
 * @return
 */
int wifi_base::_stop_wifi_shell(int argc, char** argv)
{
    ESP_ERROR_CHECK(_this == NULL);
    _this->stop();
    return 0;
}

/**
 * @brief Connect AP shell command
 * @param argc
 * @param argv      <SSID> <PSK>
 * @return
 */
int wifi_base::_connect_ap(int argc, char** argv)
{
    ESP_ERROR_CHECK(_this == NULL);

    if (argc == 3u) {

        strcpy((char*)&_s_wifi_config.sta.ssid[0u], argv[1]);
        strcpy((char*)&_s_wifi_config.sta.password[0u], argv[2]);

        /* Setting a password implies station will connect to all security modes including WEP/WPA.
         * However these modes are deprecated and not advisable to be used. Incase your Access point
         * doesn't support WPA2, these mode can be enabled by commenting below line */

        if (strlen((char*)_s_wifi_config.sta.password)) {
            _s_wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        }

        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_set_config(static_cast<wifi_interface_t>(ESP_IF_WIFI_STA), &_s_wifi_config));
    }

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_start());
    esp_wifi_connect();

    return 0;
}

/**
 * @brief Scan AP shell command
 * @param argc
 * @param argv
 * @return
 */
int wifi_base::_scan_ap_shell(int argc, char** argv)
{
    ESP_ERROR_CHECK(_this == NULL);
    _this->scan_ap();
    return 0;
}

/**
 * @brief Init shell commands
 */
void wifi_base::_init_shell()
{
    /* register wifi_scan command */
    esp_console_cmd_t cmd{};

    cmd.command = "wifi_scan";
    cmd.help = "Scan available APs";
    cmd.func = &wifi_base::_scan_ap_shell;

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_console_cmd_register(&cmd));

    cmd.command = "wifi_start";
    cmd.help = "Start WiFi";
    cmd.func = &wifi_base::_start_wifi_shell;

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_console_cmd_register(&cmd));

    cmd.command = "wifi_stop";
    cmd.help = "Stop WiFi";
    cmd.func = &wifi_base::_stop_wifi_shell;

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_console_cmd_register(&cmd));

    cmd.command = "wifi_connect";
    cmd.help = "Wifi connect AP : wifi_connect <SSID> <PSK>";
    cmd.func = &wifi_base::_connect_ap;

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_console_cmd_register(&cmd));
}

/* ========================================================================= */
/* Public API */
/* ========================================================================= */

/**
 * @brief Wifi start
 * @return
 */
esp_err_t wifi_base::start()
{
    esp_err_t ret = 0u;
    ESP_ERROR_CHECK_WITHOUT_ABORT(ret = esp_wifi_start());
    return ret;
}

/**
 * @brief Wifi stop
 * @return
 */
esp_err_t wifi_base::stop()
{
    esp_err_t ret = 0u;
    ESP_ERROR_CHECK_WITHOUT_ABORT(ret = esp_wifi_stop());
    return ret;
}

/**
 * @brief Wifi Scan APs
 * @return
 */
esp_err_t wifi_base::scan_ap()
{
    /* start WiFi */
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_start());

    /* configure scan */
    wifi_scan_config_t cfg{};
    cfg.ssid = NULL;
    cfg.bssid = NULL;
    cfg.show_hidden = false;

    /* set active scan time threshold */
    cfg.scan_type = WIFI_SCAN_TYPE_ACTIVE;
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
    /* bind static reference for shell command usage */
    _this = this;

    /* bind default config to an rvalue */
    wifi_init_config_t conf = WIFI_INIT_CONFIG_DEFAULT();

    /* init WiFi Event Handlers group */
    _s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

#ifdef CONFIG_IDF_TARGET_ESP8266
    /* init TCP/IP adapter */
    tcpip_adapter_init();
#endif

    /* init wifi */
    ESP_ERROR_CHECK(esp_wifi_init(&conf));

    /* init WiFi event handlers */
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, _wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, _ip_event_handler, NULL));

    /* initialize shell commands */
    _init_shell();

    TaskHandle_t handle;
    xTaskCreate(&this->thread_run, "Wifi", 1024u, NULL, 1u, &handle);

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