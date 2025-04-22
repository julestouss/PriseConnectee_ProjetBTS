#ifndef CONSTANTS_H
#define CONSTANTS_H

extern const char *ca_cert_pem;
extern const char *client_cert_pem;
extern const char *client_key_pem;

#define ESP_WIFI_SSID "EspSSID"
#define ESP_WIFI_PASS "testesp1"
#define ESP_WIFI_CHANNEL 1
#define ESP_WIFI_MAX_CONN 4
#define WIFI_FAIL_BIT BIT1

#define GPIO_AMP 4
#define GPIO_REL 5

#define SUCCESS 0
#define FAILURE -1

#define HOSTENAME "pc-test3"
#define ID_PRISE 3
#define TOPIC_PC_BATT "batterie/pc-test3"
#define TOPIC_PC_CONS "conso/pc-test3"

#endif // CONSTANTS_H
