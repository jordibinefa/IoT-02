#ifndef IOT_02_MQTT_CREDENTIALS
#define IOT_02_MQTT_CREDENTIALS

#define NODE_UNIQUE_NAME szOwnMac /* Directly embedded in the code*/

const char* mqtt_server = "broker.mqtts.cat"; // Your MQTTS broker
const int mqtt_port = 8883; 
const char* mqtt_user = "usuari";
const char* mqtt_password = "contrasenya";

// Aquí va el vostre certificat
const char* ca_cert = \
                      "-----BEGIN CERTIFICATE-----\n" \
                      "MIIDqjCCApKgAwIBAgIJAKwjtvZmANs9MA0GCSqGSIb3DQEBDQUAMGoxFzAVBgNV\n" \
                      "BAMMDkFuIE1RVFQgYnJva2VyMRYwFAYDVQQKDA1Pd25UcmFja3Mub3JnMRQwEgYD\n" \
                      "VQQLDAtnZW5lcmF0ZS1DQTEhMB8GCSqGSIb3DQEJARYSbm9ib2R5QGV4YW1wbGUu\n" \
                      "bmV0MB4XDTE4MDgwMTE5MzExOVoXDTMyMDcyODE5MzExOVowajEXMBUGA1UEAwwO\n" \
                      "QW4gTVFUVCBicm9rZXIxFjAUBgNVBAoMDU93blRyYWNrcy5vcmcxFDASBgNVBAsM\n" \
                      "C2dlbmVyYXRlLUNBMSEwHwYJKoZIhvcNAQkBFhJub2JvZHlAZXhhbXBsZS5uZXQw\n" \
                      "fNaaQyFpUg97+5tXEtACi+AzFTQ/gIkGbIc5fNRCX6f66rcXmAtK3BriuKzywfM3\n" \
                      "TNLf8x2vbjbRM0ilWPJ3Q5cxASFItBbS1/VOoqKhtrlT56Nm1Xz+mO0dw4LE8ww6\n" \
                      "qBmtH31pd8DhtAYBp/jNzkp7qlot9NzAg9mytZex0S6i4b7nJDUrzbOTa7tkJyHe\n" \
                      "SptqLqotBKo7cyi2ZpCpuPD5vpYt59lEUuX9MaeffD+4zX7HhFCTCgruTyLJRQsL\n" \
                      "tPPWGPeg1ve25fp3G19dD2wxkwawPNktfHHt4P2THQfbNB7nj6BNH3hxkrQk2z3h\n" \
                      "AgMBAAGjUzBRMB0GA1UdDgQWBBS4+Mr6juV9HufTIqh4MKGnakulQTAfBgNVHSME\n" \
                      "GDAWgBS4+Mr6juV9HufTIqh4MKGnakulQTAPBgNVHRMBAf8EBTADAQH/MA0GCSqG\n" \
                      "SIb3DQEBDQUAA4IBAQAXbxaCSvnEiQgkLd0xV1tdlTiEMoG1Jk33RLqBqSSfPfgN\n" \
                      "jXQ0nJn4E2DtEnkQBgRBL08jMpujJuw3sjWhqX+0Gnw+rMrPqFl1oV/fFs6D3t+8\n" \
                      "BAMMDkFuIE1RVFQgYnJva2VyMRYwFAYDVQQKDA1Pd25UcmFja3Mub3JnMRQwEgYD\n" \
                      "T/XOacELpXLv5Ri95FTb91NIu8a1TXopbwufzDGZGDgtFv4pys96DL4J1dc0BMFp\n" \
                      "9bFclEq4aIUHw4DOhm0BSK9YA8NCF1zTv3ujr4LSHUNTlODUQaK0E6ASKnzGA1WM\n" \
                      "NLgpPXiTd2SNnM6GuHnhjJW6xo0pxSht72cQHjDf\n" \
                      "-----END CERTIFICATE-----\n";


#endif // IOT_02_MQTT_CREDENTIALS
