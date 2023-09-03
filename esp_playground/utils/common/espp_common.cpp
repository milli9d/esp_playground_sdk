/**
 * @file espp_common.hpp
 *
 * @brief ESP Common definitions
 *
 */
#include <cstdio>

#include <esp_log.h>
#include <esp_system.h>
#include <esp_spi_flash.h>

#define ESPP_COMMON_TAG "ESP Playground"

namespace espp {

void print_sys_info(void)
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    ESP_LOGI(ESPP_COMMON_TAG, "This is ESP8266 chip with %d CPU cores, WiFi, ", chip_info.cores);
    ESP_LOGI(ESPP_COMMON_TAG, "silicon revision %d, ", chip_info.revision);
    ESP_LOGI(ESPP_COMMON_TAG, "%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
             (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}

/**
 * @brief Pretty print banner
 */
void _banner(const char* name)
{
    printf("\n\n\n\n======================================================================================\n"
           " _______  ______ ______     ______  _                                               _ \n"
           "(_______)/ _____|_____ \\   (_____ \\| |                                             | |\n"
           " _____  ( (____  _____) )   _____) ) | _____ _   _  ____  ____ ___  _   _ ____   __| |\n"
           "|  ___)  \\____ \\|  ____/   |  ____/| |(____ | | | |/ _  |/ ___) _ \\| | | |  _ \\ / _  |\n"
           "| |_____ _____) ) |        | |     | |/ ___ | |_| ( (_| | |  | |_| | |_| | | | ( (_| |\n"
           "|_______|______/|_|8266    |_|      \\_)_____|\\__  |\\___ |_|   \\___/|____/|_| |_|\\____|\n"
           "                                            (____/(_____|%29s\n"
           "=======================================================================================\n\n\n\n",
           name);
}

} // namespace espp