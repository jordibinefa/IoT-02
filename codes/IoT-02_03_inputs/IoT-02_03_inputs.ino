// Latching relay on IoT-02 board needs 5V. It can be supplied through microUSB or J1 connector.

#define LED_W 19
#define LED_R 23
#define LED_Y 27
#define LED_G 32
#define SET_LATCHING_RELAY 2
#define RST_LATCHING_RELAY 4
#define BT_IO0 0
#define BT_I34 34
#define BT_I35 35
#define STATE_LATCHING_RELAY 39

void vLatchingRelay(bool bSet) {
  digitalWrite(bSet ? SET_LATCHING_RELAY : RST_LATCHING_RELAY, HIGH);
  delay(3);
  digitalWrite(bSet ? SET_LATCHING_RELAY : RST_LATCHING_RELAY, LOW);
}

void setup() {
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_W, OUTPUT);
  pinMode(SET_LATCHING_RELAY, OUTPUT);
  pinMode(RST_LATCHING_RELAY, OUTPUT);
  pinMode(BT_IO0, INPUT);
  pinMode(BT_I34, INPUT);
  pinMode(BT_I35, INPUT);
  pinMode(STATE_LATCHING_RELAY, INPUT);

  Serial.begin(115200);
  Serial.println(__FILE__);
}

void loop() {
  digitalWrite(LED_G, !digitalRead(BT_I35));
  digitalWrite(LED_Y, !digitalRead(BT_I34));
  digitalWrite(LED_R, !digitalRead(BT_IO0));
  //digitalWrite(LED_W, digitalRead(STATE_LATCHING_RELAY));
  digitalWrite(LED_W, !digitalRead(BT_I35) && !digitalRead(BT_I34));
  delay(10);
}
