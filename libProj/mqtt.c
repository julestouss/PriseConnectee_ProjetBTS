#include "mqtt.h"
#include "constants.h"

static const char *TAG_MQTT = "MQTT";

uint32_t MQTT_CONNEECTED = 0;
typedef struct{
  char topic[128];
  char payload[128];
} mqtt_message_t;
QueueHandle_t mqtt_sub_message_queue;

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data){
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    if ((esp_mqtt_event_id_t)event_id == MQTT_EVENT_CONNECTED) {
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
        MQTT_CONNEECTED = 1;

        msg_id = esp_mqtt_client_subscribe(client, "batterie/pc-test3", 0);
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
        int topic_len = event->topic_len;
        int data_len = event->data_len;
        ESP_LOGI(TAG_MQTT, "TOPIC=%.*s\r\n", topic_len, event->topic);
        ESP_LOGI(TAG_MQTT, "DATA=%.*s\r\n", data_len, event->data);
        mqtt_message_t msg;

        memcpy(msg.topic, event->topic, sizeof(topic_len));
        msg.topic[topic_len] = '\0';

        memcpy(msg.payload, event->data, sizeof(data_len));
        msg.payload[data_len] = '\0';

        xQueueSend(mqtt_sub_message_queue, &msg, 0);
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
        .broker.address.uri = "mqtts://192.168.27.102:8883",
        .broker.verification.certificate = ca_cert_pem,  // Certificat CA
        .broker.verification.skip_cert_common_name_check = true,
        .credentials.authentication.certificate = client_cert_pem,  // Certificat client
        .credentials.authentication.key = client_key_pem  // Clé privée client

    };
    client = esp_mqtt_client_init(&mqttConfig);
    if (client == NULL) {
        ESP_LOGE(TAG_MQTT, "MQTT client initialization failed!"); // vérifier que on a bien initialiser le client
        return;
    }
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void Publisher_Task(void *param){
  int batt = *((int *)param);
  char payload[16];
  while(1){
    if(client!=NULL && MQTT_CONNEECTED){
      sprintf(payload, "%d", batt);
      esp_mqtt_client_publish(client, TOPIC_PC_CONS, payload, 0, 0, 0);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void mqtt_processing_bat(){
  mqtt_message_t msg;
  while(1){
    if(xQueueReceive(mqtt_sub_message_queue, &msg, portMAX_DELAY)){
      ESP_LOGI(TAG_MQTT, "Message recu : topic = %s, payload=%s", msg.topic, msg.payload);
      // TODO : traiter le payload -> en fonction de ce qu'il est on définie les gpio et on call directe la Publisher task depuis ici
      if(strcmp(msg.payload, TOPIC_PC_BATT) == 0){
        int batt;
        batt = atoi(msg.payload);
        // la batterie ne devrait jamais tomber a 0 sachant que la prise est branché on sait donc que si 0 est renvoyé c'est une erreur 
        if(batt == 0){
          ESP_LOGE(TAG_MQTT, "La valeur de la batterie n'est pas valide");
        }
        if(batt < 50){
          // lancer la charge et l'accisition de données tant que on est pas a 100%
          while(batt != 100){

          }
        }
        if(batt >= 50){
          // attendre que la batterie redécende en dessous de 50 et couper la charge
        }
      }
    }
  }
}
