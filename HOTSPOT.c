#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

void WIFI_INIT(void){

	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	esp_netif_create_default_wifi_ap();
	esp_netif_create_default_wifi_sta();

	wifi_config_t ap_conf = {
		.ap = {
			.ssid = "lite",
			.ssid_len = 4,
			.channel = 1,
			.password = "hellothere090",
			.max_connection = 4,
			.authmode = WIFI_AUTH_WPA_WPA2_PSK
		},
	};
	wifi_config_t sta_conf = {
		.sta = {
			.ssid = "lite",
			.password = "hellothere090",
		},
	} ;

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_conf));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_conf));
	ESP_ERROR_CHECK(esp_wifi_start());

}
void app_main(void){

	esp_err_t ret = nvs_flash_init();
	if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	WIFI_INIT();

}
