// Latching relay on IoT-02 board needs 5V. It can be supplied through microUSB or J1 connector.
// Pressing button I35 resets latching relay
// Pressing button I34 sets latching relay
// Pressing button IO0 toggles latching relay

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

boolean bPressedButton(int nWhichOne) {
  if (digitalRead(nWhichOne))
    return false;
  return true;
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
  static boolean bIO0wasPressed = false;
  static boolean bI35wasPressed = false;
  static boolean bI34wasPressed = false;
  boolean bLatchingRelayState = digitalRead(STATE_LATCHING_RELAY);
  boolean bIO0currentState = bPressedButton(BT_IO0);
  boolean bI34currentState = bPressedButton(BT_I34);
  boolean bI35currentState = bPressedButton(BT_I35);

  if (bIO0wasPressed != bIO0currentState) {
    delay(2);
    if (bIO0currentState) {
      bLatchingRelayState = !bLatchingRelayState;
      vLatchingRelay(bLatchingRelayState);
      Serial.print("bLatchingRelayState: ");
      (bLatchingRelayState) ? Serial.println("ON") : Serial.println("OFF");
    }
    bIO0wasPressed = bIO0currentState;
  }
  if (bI34wasPressed != bI34currentState) {
    delay(2);
    if (bI34currentState) {
      bLatchingRelayState = HIGH;
      vLatchingRelay(bLatchingRelayState);
      Serial.print("bLatchingRelayState: ");
      (bLatchingRelayState) ? Serial.println("ON") : Serial.println("OFF");
    }
    bI34wasPressed = bI34currentState;
  }
  if (bI35wasPressed != bI35currentState) {
    delay(2);
    if (bI35currentState) {
      bLatchingRelayState = LOW;
      vLatchingRelay(bLatchingRelayState);
      Serial.print("bLatchingRelayState: ");
      (bLatchingRelayState) ? Serial.println("ON") : Serial.println("OFF");
    }
    bI35wasPressed = bI35currentState;
  }
  digitalWrite(LED_G, bPressedButton(BT_I35));
  digitalWrite(LED_Y, bLatchingRelayState);
  digitalWrite(LED_R, bPressedButton(BT_I34));
  digitalWrite(LED_W, bPressedButton(BT_IO0));
  delay(10);
}
