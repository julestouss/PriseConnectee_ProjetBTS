#include <stdio.h>
#include <esp_log.h>
#include <esp_check.h>
#include <nvs_flash.h> // to flash the memory (Non volatile sorage = NVS)
#include <string.h>
#include "constants.h"
#include "wifi_ap_sta.h"
#include "http_server.h"
#include "mqtt.h"



void app_main(void) {
    // initialise nvs 
    esp_err_t ret = nvs_flash_init(); // is use to check if an error occure and then init the nvs flash
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
            // if the nvs is not blank erase what is written in it 
            }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(network_conf());

    //define the wifi mod, here because it will be called first
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    

    // check if something is going wronge while connecting to the ap
    esp_err_t wifi_status = FAILURE;

    wifi_status = ap_conf_wifi();
    if (SUCCESS != wifi_status){
        ESP_LOGI("WIFI ", "Failed to connect to the sta, dying ....");
        return;
    }

    //ESP_ERROR_CHECK(http_conf());
    // commenté pour tester le wifi et mqtt sans avoir a passer par la page web a chaque fois
    sta_conf_wifi("Altice_F524", "tscp1065");

//    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    mqtt_app_start();

    xTaskCreate(Publisher_Task, "Publisher_Task", 1024 * 5, NULL, 5, NULL);

}
