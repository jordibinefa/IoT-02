// Latching relay on IoT-02 board needs 5V. It can be supplied through microUSB or J1 connector.

#define LED_W 19
#define LED_R 23
#define LED_Y 27
#define LED_G 32
#define SET_LATCHING_RELAY 2
#define RST_LATCHING_RELAY 4

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

  Serial.begin(115200);
  Serial.println(__FILE__);
}

void loop() {
  int nOutputs[] = {LED_G, LED_Y, LED_R, LED_W, SET_LATCHING_RELAY, RST_LATCHING_RELAY};
  static int nCmpt = 0;

  Serial.println("GPIO-" + String(nOutputs[nCmpt]));
  if (nCmpt < 4) {
    digitalWrite(nOutputs[nCmpt], HIGH);
    delay(500);
    digitalWrite(nOutputs[nCmpt], LOW);
  }
  if (nCmpt == 4) {
    vLatchingRelay(true);
    delay(500);
  }
  if (nCmpt == 5) {
    vLatchingRelay(false);
    delay(500);
  }

  nCmpt = (nCmpt < 5) ? nCmpt + 1 : 0;
  delay(500);
}
