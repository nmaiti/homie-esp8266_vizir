#pragma once

#include "../Constants.hpp"
#include "../Limits.hpp"

namespace HomieInternals {
  struct ConfigStruct {
    char name[MAX_FRIENDLY_NAME_LENGTH];
    char deviceId[MAX_DEVICE_ID_LENGTH];

    struct WiFi {
      char ssid[MAX_WIFI_SSID_LENGTH];
      char password[MAX_WIFI_PASSWORD_LENGTH];
    } wifi;

    struct Server {
      char host[MAX_HOSTNAME_LENGTH];
      unsigned int port;
      struct mDNS {
        bool enabled;
        char service[MAX_HOSTNAME_LENGTH];
      } mdns;
      struct SSL {
        bool enabled;
        char fingerprint[MAX_FINGERPRINT_LENGTH];
      } ssl;
    };

    int total_nodes;
	struct nodem {
		char name[20];
		char pin;
		char  mode[15]; // gpio i/p mode [0] / gpio o/p mode/ pwm mode
		char type[15];
	} nodes[5];

    struct MQTT {
      Server server;
      char baseTopic[MAX_MQTT_BASE_TOPIC_LENGTH];
      bool auth;
      char username[MAX_MQTT_CREDS_LENGTH];
      char password[MAX_MQTT_CREDS_LENGTH];
    } mqtt;

    struct OTA {
      bool enabled;
      Server server;
      char path[MAX_OTA_PATH_LENGTH];
    } ota;
  };
}
