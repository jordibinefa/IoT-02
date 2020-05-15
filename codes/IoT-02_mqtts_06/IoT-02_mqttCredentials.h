#ifndef IOT_02_MQTT_CREDENTIALS
#define IOT_02_MQTT_CREDENTIALS

#define NODE_UNIQUE_NAME szOwnMac /* Directly embedded in the code*/

const char* mqtt_server = "iot.siarq.net"; // Your MQTTS broker
const int mqtt_port = 8883; 
const char* mqtt_user = "upc";
const char* mqtt_password = "school";

const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDtTCCAp2gAwIBAgIUdVu53YlKp/Tb9f2ZLOX1EEBCzwowDQYJKoZIhvcNAQEN\n" \
"BQAwajEXMBUGA1UEAwwOQW4gTVFUVCBicm9rZXIxFjAUBgNVBAoMDU93blRyYWNr\n" \
"cy5vcmcxFDASBgNVBAsMC2dlbmVyYXRlLUNBMSEwHwYJKoZIhvcNAQkBFhJub2Jv\n" \
"ZHlAZXhhbXBsZS5uZXQwHhcNMTgxMjIzMTkzNTAxWhcNMzIxMjE5MTkzNTAxWjBq\n" \
"MRcwFQYDVQQDDA5BbiBNUVRUIGJyb2tlcjEWMBQGA1UECgwNT3duVHJhY2tzLm9y\n" \
"ZzEUMBIGA1UECwwLZ2VuZXJhdGUtQ0ExITAfBgkqhkiG9w0BCQEWEm5vYm9keUBl\n" \
"eGFtcGxlLm5ldDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKd3emAE\n" \
"dmizeQM59PH5hGUeUgmA/IVq5jhwZjWZHPTcOPSMI1e5iSU2m6QYV3xfMIkv3LH0\n" \
"9DWU6cgo9wOqr6EzbTgDhsVoUUOYoDUgbaYyR3VRA6nd1BEYXF5q/YWvQbb1bRw2\n" \
"jpsxDqQPWQ5c1B7oZCgS/bAahI387h8vEGbuvPMGC5O+bJuCU8wnfJvvUU0Z1yqM\n" \
"n3HNApIrAIjvSGQLiLUzX65SHuDZ0q1P49Yw0MUswQxZNLOE5qdxSot7CSS5whVi\n" \
"gr1NcsZo/OEBht/FBxdrBiNcrC4UPOS9J8/n6RYliYc2NPiUgnp19N1F2irwa9bJ\n" \
"I4sY8zG7O4bodz8CAwEAAaNTMFEwHQYDVR0OBBYEFGcSOKfT0lO5uFJbqTKNbkvt\n" \
"9iJvMB8GA1UdIwQYMBaAFGcSOKfT0lO5uFJbqTKNbkvt9iJvMA8GA1UdEwEB/wQF\n" \
"MAMBAf8wDQYJKoZIhvcNAQENBQADggEBAFo9KIemclnw8VD9YaAxfXIOk+a1g0eK\n" \
"7nKo1x9ubhXHdbfImGg7eEe7Ab8vcu9z5YCSoSSQAKonle7MCuM8jMcdQN/G/1qU\n" \
"kE5ZGXQmKeyNS2RAja4lhMmDcMXL28RRpmarZu/woLC5QnHATuQOKCC2vgDO6uW2\n" \
"RsKGHPEn0BqcYNkIopfSwqkzi4yhwLx4H1PL+ckwhp1lv3mJQW7xZEkDW25j0aJw\n" \
"SFRr6tlwACQvykIan+a/wF0S5zUPNMy9FTsaHSp89CC2ajeS1SGjJu2iOWrymWq7\n" \
"B6dNTB+hY0PzLbJhLq/7Mud6G8czvR2SWV2mnVHmFjnhs3MxbKhIoZc=\n" \
"-----END CERTIFICATE-----\n";



#endif // IOT_02_MQTT_CREDENTIALS
