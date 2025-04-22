#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "esp_http_server.h"
#include "esp_log.h"

extern char ssid[32];
extern char password[64];

esp_err_t http_conf(void);

#endif // HTTP_SERVER_H
