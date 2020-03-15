#define LED_W 19
#define LED_R 23
#define LED_Y 27
#define LED_G 32

void setup() {
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_W, OUTPUT);

  Serial.begin(115200);
  Serial.println(__FILE__);
}

void loop() {
  int nLeds[] = {LED_G, LED_Y, LED_R, LED_W};
  static int nCmpt = 0;

  Serial.println("GPIO-"+String(nLeds[nCmpt]));
  digitalWrite(nLeds[nCmpt], HIGH);
  delay(500);
  digitalWrite(nLeds[nCmpt], LOW);
  delay(500);
  nCmpt = (nCmpt < 3) ? nCmpt+1 : 0;
}
