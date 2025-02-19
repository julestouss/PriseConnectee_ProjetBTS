cmake_minimum_required(VERSION 3.25)

idf_component_register(SRCS "projetE6.c" "../libProj/wifi_ap_sta.c" "../libProj/http_server.c" "../libProj/mqtt.c"
                            REQUIRES driver
                            REQUIRES esp_wifi
                            REQUIRES nvs_flash
                            REQUIRES esp_http_server
                            REQUIRES mqtt
                            INCLUDE_DIRS "."
                            INCLUDE_DIRS "../libProj/include"
                            )
                    
