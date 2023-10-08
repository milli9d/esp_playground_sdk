idf_component_register(SRCS "espp_wifi.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_event esp_netif wifi esp_wifi console smart_timer)
