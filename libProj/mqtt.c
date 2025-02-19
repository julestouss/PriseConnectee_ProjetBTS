#include "mqtt.h"
#include "constants.h"

static const char *TAG_MQTT = "MQTT";

uint32_t MQTT_CONNEECTED = 0;


void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data){
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    if ((esp_mqtt_event_id_t)event_id == MQTT_EVENT_CONNECTED) {
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
        MQTT_CONNEECTED = 1;

        msg_id = esp_mqtt_client_subscribe(client, "/topic/batterie", 0);
        ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);

    } 
    else if ((esp_mqtt_event_id_t)event_id == MQTT_EVENT_DISCONNECTED) {
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
        MQTT_CONNEECTED = 0;
    } 
    else if ((esp_mqtt_event_id_t)event_id == MQTT_EVENT_SUBSCRIBED) {
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
    } 
    else if ((esp_mqtt_event_id_t)event_id == MQTT_EVENT_UNSUBSCRIBED) {
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
    } 
    else if ((esp_mqtt_event_id_t)event_id == MQTT_EVENT_PUBLISHED) {
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
    } 
    else if ((esp_mqtt_event_id_t)event_id == MQTT_EVENT_DATA) {
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
    } 
    else if ((esp_mqtt_event_id_t)event_id == MQTT_EVENT_ERROR) {
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
    } 
    else {
        ESP_LOGI(TAG_MQTT, "Other event id:%d", event->event_id);
    }

}

esp_mqtt_client_handle_t client = NULL ;

void mqtt_app_start(void) {
    ESP_LOGI(TAG_MQTT, "STARTING MQTT");    
    esp_mqtt_client_config_t mqttConfig = {
        .broker = {
            .address.uri = "mqtt://192.168.160.1:1883",
        }
    };
    client = esp_mqtt_client_init(&mqttConfig);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void Publisher_Task(void *params){

  while (true){
    if(client!=NULL && MQTT_CONNEECTED){
        //la publication en elle meme et ce que on fait quand on publie --> modifiable avec des arguments pour personnaliser
        esp_mqtt_client_publish(client, "test", "Hello World", 0, 0, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
}
