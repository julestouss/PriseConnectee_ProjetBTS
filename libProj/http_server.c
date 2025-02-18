#include "http_server.h"
#include "constants.h"
#include "wifi_ap_sta.h"

static const char *TAG = "WIFI :";  // Définir TAG comme une chaîne de caractères


static esp_err_t test_handler(httpd_req_t *req) {
    const char resp[] = "<h1>Hello World</h1>";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t SSID_PASS_handler(httpd_req_t *req) {
    const char wifi_config_page[] =  "<!DOCTYPE html>"
                                        "<html lang=\"fr\">"
                                        "<head>"
                                        "    <meta charset=\"UTF-8\">"
                                        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                                        "    <title>Configuration WiFi</title>"
                                        "    <style>"
                                        "        body {"
                                        "            font-family: Arial, sans-serif;"
                                        "            display: flex;"
                                        "            justify-content: center;"
                                        "            align-items: center;"
                                        "            height: 100vh;"
                                        "            margin: 0;"
                                        "            background-color: #f4f4f9;"
                                        "        }"
                                        "        .container {"
                                        "            background: #fff;"
                                        "            padding: 20px;"
                                        "            border-radius: 10px;"
                                        "            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);"
                                        "            text-align: center;"
                                        "            width: 100%;"
                                        "            max-width: 400px;"
                                        "        }"
                                        "        h1 {"
                                        "            font-size: 1.5rem;"
                                        "            color: #333;"
                                        "            margin-bottom: 20px;"
                                        "        }"
                                        "        form {"
                                        "            display: flex;"
                                        "            flex-direction: column;"
                                        "        }"
                                        "        label {"
                                        "            font-size: 1rem;"
                                        "            margin-bottom: 5px;"
                                        "            text-align: left;"
                                        "        }"
                                        "        input {"
                                        "            padding: 10px;"
                                        "            font-size: 1rem;"
                                        "            margin-bottom: 15px;"
                                        "            border: 1px solid #ccc;"
                                        "            border-radius: 5px;"
                                        "            width: 100%;"
                                        "            align-self: center;"
                                        "        }"
                                        "        input:focus {"
                                        "            border-color: #007BFF;"
                                        "            outline: none;"
                                        "        }"
                                        "        button {"
                                        "            background-color: #007BFF;"
                                        "            color: white;"
                                        "            padding: 10px;"
                                        "            font-size: 1rem;"
                                        "            border: none;"
                                        "            border-radius: 5px;"
                                        "            cursor: pointer;"
                                        "        }"
                                        "        button:hover {"
                                        "            background-color: #0056b3;"
                                        "        }"
                                        "    </style>"
                                        "</head>"
                                        "<body>"
                                        "    <div class=\"container\">"
                                        "        <h1>Configuration WiFi</h1>"
                                        "        <form action=\"http://192.168.4.1:8080/configure\" method=\"POST\">"
                                        "            <label for=\"ssid\">SSID :</label>"
                                        "            <input type=\"text\" id=\"ssid\" name=\"ssid\" placeholder=\"Entrez le SSID\" required>"
                                        "            <label for=\"password\">Mot de passe :</label>"
                                        "            <input type=\"password\" id=\"password\" name=\"password\" placeholder=\"Entrez le mot de passe\" required>"
                                        "            <button type=\"submit\">Valider</button>"
                                        "        </form>"
                                        "    </div>"
                                        "</body>"
                                        "</html>";
    httpd_resp_send(req, wifi_config_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t configure_handler(httpd_req_t *req) {
    char buf[128];
    int len = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (len <= 0) {
        if (len == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }
    buf[len] = '\0';

    char ssid[32] = {0};
    char password[64] = {0};
    sscanf(buf, "ssid=%31[^&]&password=%63s", ssid, password);

    ESP_LOGI("CONFIGURE_HANDLER", "Parsed SSID: %s", ssid);
    ESP_LOGI("CONFIGURE_HANDLER", "Parsed Password: %s", password);
    
    
    esp_err_t err = sta_conf_wifi((char *)ssid,(char *) password);
    if (err != ESP_OK) {
        httpd_resp_send(req, "Erreur: Connexion Wi-Fi échouée", HTTPD_RESP_USE_STRLEN);
        return ESP_FAIL;
    }else{
        const char *resp = "Configuration received!";
        httpd_resp_send(req, resp, strlen(resp));
        return ESP_OK;
    }
}

esp_err_t http_conf() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 8080;
    httpd_handle_t server = NULL;

    httpd_uri_t test_uri = {
        .uri      = "/test",
        .method   = HTTP_GET,
        .handler  = test_handler,
        .user_ctx = NULL
    };

    httpd_uri_t wifi_config_uri = {
        .uri      = "/",
        .method   = HTTP_GET,
        .handler  = SSID_PASS_handler,
        .user_ctx = NULL
    };

    httpd_uri_t configure_uri = {
        .uri      = "/configure",
        .method   = HTTP_POST,
        .handler  = configure_handler,
        .user_ctx = NULL
    };

    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start HTTP server");
        return ESP_FAIL;
    }

    httpd_register_uri_handler(server, &test_uri);
    httpd_register_uri_handler(server, &wifi_config_uri);
    httpd_register_uri_handler(server, &configure_uri);

    return ESP_OK;
}
