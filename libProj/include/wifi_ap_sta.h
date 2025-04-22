#ifndef WIFI_AP_STA_H
#define WIFI_AP_STA_H

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "string.h"


// Wi-Fi AP configuration
esp_err_t ap_conf_wifi(void);

// Wi-Fi STA configuration
esp_err_t sta_conf_wifi(char* STA_SSID, char* STA_PASS);

// Wi-Fi network initialization
esp_err_t network_conf(void);

// Wi-Fi event handler
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

void wifi_init_softap(void);

extern EventGroupHandle_t wifi_event_group;
extern const int WIFI_CONNECTED_BIT;

#endif // WIFI_AP_STA_H
