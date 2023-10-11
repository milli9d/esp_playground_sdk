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

#include <esp_console.h>
#include <esp_wifi.h>

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

    /* ===================================================================== */
    /* Shell Commands */
    /* ===================================================================== */

    /**
     * @brief
     * @param argc
     * @param argv
     * @return
     */
    static int _scan_ap_shell(int argc, char** argv);

    /**
     * @brief Start AP shell command
     * @param argc
     * @param argv
     * @return
     */
    static int _start_wifi_shell(int argc, char** argv);

    /**
     * @brief Stop AP shell command
     * @param argc
     * @param argv
     * @return
     */
    static int _stop_wifi_shell(int argc, char** argv);

    /**
     * @brief Connect AP shell command
     * @param argc
     * @param argv      <SSID> <PSK>
     * @return
     */
    static int _connect_ap(int argc, char** argv);

    /**
     * @brief Init shell commands
     */
    void _init_shell();

  public:
    /* ===================================================================== */
    /* Constructors/Destructors */
    /* ===================================================================== */
    static wifi_config_t _s_wifi_config;
    static SemaphoreHandle_t _wifi_scan_complete_sem;
    static wifi_base* _this;

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


    /**
     * @brief 
     * @param args 
     */
    static void thread_run(void* args);

    /**
     * @brief Wifi Scan APs
     * @return
     */
    esp_err_t scan_ap(void);

    /**
     * @brief Wifi start
     * @return
     */
    esp_err_t start(void);

    /**
     * @brief Wifi stop
     * @return
     */
    esp_err_t stop(void);
};

} // namespace espp