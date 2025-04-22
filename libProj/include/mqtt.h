#ifndef MQTT_H
#define MQTT_H

#include "string.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "esp_event.h"
#include "esp_system.h"

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

void mqtt_app_start(void);

void Publisher_Task(void *param);

#endif
