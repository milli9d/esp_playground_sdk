idf_component_register(SRCS "espp_esp_now.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_event esp_netif esp_wifi wifi)
