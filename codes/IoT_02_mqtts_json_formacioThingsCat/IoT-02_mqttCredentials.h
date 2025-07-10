#ifndef IOT_02_MQTT_CREDENTIALS
#define IOT_02_MQTT_CREDENTIALS

#define NODE_UNIQUE_NAME szOwnMac /* Directly embedded in the code*/

const char* mqtt_server = "formacio.things.cat"; // Your MQTTS broker
const int mqtt_port = 8883; 
const char* mqtt_user = "ecat";
const char* mqtt_password = "clotClot";

const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIF5zCCA8+gAwIBAgIUOufTMisziR4uy5scDPYZS2vwsH8wDQYJKoZIhvcNAQEL\n" \
"BQAwgYExCzAJBgNVBAYTAkVTMRIwEAYDVQQIDAlDYXRhbHVueWExEjAQBgNVBAcM\n" \
"CUJhcmNlbG9uYTEaMBgGA1UECgwRRm9ybWFjaW9UaGluZ3NDYXQxEDAOBgNVBAsM\n" \
"B01RVFQtQ0ExHDAaBgNVBAMME2Zvcm1hY2lvLnRoaW5ncy5jYXQwIBcNMjUwNzEw\n" \
"MTcxMzQ0WhgPMjEyNTA2MTYxNzEzNDRaMIGBMQswCQYDVQQGEwJFUzESMBAGA1UE\n" \
"CAwJQ2F0YWx1bnlhMRIwEAYDVQQHDAlCYXJjZWxvbmExGjAYBgNVBAoMEUZvcm1h\n" \
"Y2lvVGhpbmdzQ2F0MRAwDgYDVQQLDAdNUVRULUNBMRwwGgYDVQQDDBNmb3JtYWNp\n" \
"by50aGluZ3MuY2F0MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAzDXq\n" \
"xtuqDUvnEGv6SZNnu24BR3TXUXrTdGlI+uQaVMR/2brWiaWvmVEkyz7JVAPJ8erS\n" \
"t7vOYog8LVh0Yg7Gc3wvHPfJO+XsWecaale0aJyC1hq2K/5wdmGR4MXsEQal2PmV\n" \
"qmz0vN/G1Z8iC0GpJQ6CGvtWpOgEpPJgo7/Yw2N5Mr6ipUH4xC5QC2+jq/MN0msK\n" \
"Ry8I0qF6Txp4kPzxkYVUTktmY9SKI879vUmmywaeGoKetsWJDo/5P/VJeuI8zey4\n" \
"nL0izj6p9I5SlgaUw20KNN4amiGmIauHkdQYpDZt/PxqgHFWwJroPeml2LZkgXoL\n" \
"M+rRiwoUYZV8FNwcEhn14tRSVQWraAoOO3eyKOYVduiWpoDowtC8r0/ry1qNPB1r\n" \
"w1QP55S2k5J+9OVbZidapXEY60mRP5vkX57HgyLc9g4YfTxOyJYHpghzBEqCFX4n\n" \
"WgBJlCbLOU/iYVHmi7VS7Ab7bXPsCnW/fukITlAWaL5v7K6ySRZwKerXxhf0fg6P\n" \
"WfEHp58zkeBfQzfHzOBoHA7JDqSNaaPs7txZz/b0zDSFQ99i5k7fwLG7p+VX8hXD\n" \
"L7ANOdxR5ko5d3tfsHGlJz31DByIqj17tr4sE1refUE3an6KBo58rPhat0/chiFq\n" \
"TMGi/aiUUKrFe8sXFjYJ7D+qOBTtxe8z4xO3XaUCAwEAAaNTMFEwHQYDVR0OBBYE\n" \
"FPBPx5HufdcibKkqs5La2TlbBkHzMB8GA1UdIwQYMBaAFPBPx5HufdcibKkqs5La\n" \
"2TlbBkHzMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggIBACozPIHo\n" \
"NJUkOgZK4cN3tUCYVgXKkx76yGLtNCu8YXy78CSaNlMPXcn+kqew4xWTAqOqr6Ju\n" \
"uxPsRNkknl2cuCzrqcilwhy810X3s1Pcr+AMzHGC8uRvR5yEkKcttTFKDETaU8Co\n" \
"+Q8iCA+2p64KKWgMxMYsosC5RFScq7NSp8nIFm9MIDjm98ID7z2zTi6P6JBwB79k\n" \
"qqZFXNrwS+R/TiIxPOmNqJYwd/Tne8RHRXHXKmriuUqkE2lmfpbwfPWe4rg4h+2x\n" \
"gJRMtwM/ZO6T0sgPmKk1yaLCAgABWOmFCI3vt71XxrTa5I9RuRL+tVl/4Zon8T7D\n" \
"gS/qoV//hwh308TIK+S+b5yKY29UHoZNuLsnTizMg85oYAaBNqB/9+pe1u8Et5oI\n" \
"3FxWKtDzOSml1dkFloM7uOZ664DEMs3pug6RSlxJrrvwwiMcoU+Lw+uSCztUvRn2\n" \
"KYIRqFdfXEeeIzdTQRkbjm60wm5/nWR8gCtUJfkGYI5Bw01wmvHl7vUlhX4QA/Oe\n" \
"mDm95qyi9HhAIsPnUwADyiNpUYZmMMXoDX4cxADEbhObd+dBZGcG+QOZ4bihE++G\n" \
"eR/g5BH8lmG8liVeVVQL1xNK9Mc7g5JGWQch9CCTyyaJ1KHLRf2eOdY9R6xfFk0R\n" \
"xircHfxzKWDGGhIThBAk9be+hipw3jTjV5/8\n" \
"-----END CERTIFICATE-----\n";


#endif // IOT_02_MQTT_CREDENTIALS
