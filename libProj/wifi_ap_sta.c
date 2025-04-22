#include "wifi_ap_sta.h"
#include "constants.h"

static const char *TAG = "WIFI :";  // Définir TAG comme une chaîne de caractères
EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

esp_err_t ap_conf_wifi() {
    wifi_config_t ap_wifi_config = {
        .ap = {
            .ssid = ESP_WIFI_SSID,
            .ssid_len = strlen(ESP_WIFI_SSID),
            .password = ESP_WIFI_PASS,
            .channel = ESP_WIFI_CHANNEL,
            .max_connection = ESP_WIFI_MAX_CONN,
            .authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "WiFi AP configuration completed.");
    return ESP_OK;
}

esp_err_t sta_conf_wifi(char* STA_SSID, char* STA_PASS) {
    esp_err_t err;
    
    wifi_config_t sta_wifi_config = {};
    strcpy((char *)sta_wifi_config.sta.ssid, STA_SSID);
    strcpy((char *)sta_wifi_config.sta.password, STA_PASS);
    sta_wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    sta_wifi_config.sta.pmf_cfg.capable = true;
    sta_wifi_config.sta.pmf_cfg.required = false;

    err = esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_wifi_config);
    if (err != ESP_OK) {
        ESP_LOGE("STA_CONF", "Error in esp_wifi_set_config: %s", esp_err_to_name(err));
        return err;
    }
    
    ESP_LOGI("STA_CONF", "Configuration appliquée, tentative de connexion...");
    
    err = esp_wifi_connect();
    if (err != ESP_OK) {
        ESP_LOGE("STA_CONF", "Erreur lors de esp_wifi_connect: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "fin de la fonction de connexion");

    return ESP_OK;
}

esp_err_t network_conf() {
    esp_err_t err;

    if ((err = esp_netif_init()) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize network stack: %s", esp_err_to_name(err));
        return err;
    }

    if ((err = esp_event_loop_create_default()) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create default event loop: %s", esp_err_to_name(err));
        return err;
    }
    if ((err = esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL)) != ESP_OK){
        ESP_LOGE(TAG, "Failed to register the event handler");
        return err;
    }
    if (!esp_netif_create_default_wifi_sta() || !esp_netif_create_default_wifi_ap()) {
        ESP_LOGE(TAG, "Failed to create Wi-Fi interfaces");
        return ESP_FAIL;
    }

    wifi_event_group = xEventGroupCreate();
    if (wifi_event_group == NULL) {
        ESP_LOGE(TAG, "Erreur : Impossible de créer l'EventGroup Wi-Fi !");
        return ESP_FAIL;
    }
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    if ((err = esp_wifi_init(&cfg)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize Wi-Fi: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Network configuration completed successfully");
    return ESP_OK;
}

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Disconnected. Reconnecting...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}
