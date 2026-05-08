// IoT-02_seq01.ino
// Igual que seq00 + toggle del missatge "IoT-02_seq01" amb I34 quan la seqüència és aturada
// Autor: Jordi Binefa
// Placa: IoT-02 (ESP32 DevKit Module v2)

#include <Wire.h>
#include "SSD1306Wire.h"

// ── Pinout ───────────────────────────────────────────────────────────────────
#define LED_W   19
#define LED_R   23
#define LED_Y   27
#define LED_G   32
#define BT_IO0   0
#define BT_I34  34
#define BT_I35  35
#define LDR     36
#define I2C_SDA 21
#define I2C_SCL 22

// ── OLED ─────────────────────────────────────────────────────────────────────
SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL);

// ── Estat global compartit entre tasques ─────────────────────────────────────
volatile bool bSeqRunning   = false;  // seqüència en marxa
volatile bool bSeqEverRan   = false;  // ha arrencat alguna vegada
volatile int  iSeqStep      = 0;      // pas actual (0..3)
volatile bool bScreenReady  = false;  // false = pantalla inicial visible
volatile bool bShowSeq01Msg = false;  // toggle del missatge IoT-02_seq01

SemaphoreHandle_t xSeqSemaphore = NULL;
SemaphoreHandle_t xDisplayMutex = NULL;

// ── Seqüència ─────────────────────────────────────────────────────────────────
const int   SEQ_LEDS[]  = { LED_W, LED_R, LED_Y, LED_G };
const char* SEQ_NAMES[] = { "W",   "R",   "Y",   "G"   };
const char* LED_LABELS[]= { "Led W", "Led R", "Led Y", "Led G" };
const int   SEQ_LEN     = 4;
const int   SEQ_DELAY_MS= 750;

// ── Helpers ───────────────────────────────────────────────────────────────────
bool bPressedButton(int pin) {
  return (digitalRead(pin) == LOW);
}

void vAllLedsOff() {
  digitalWrite(LED_W, LOW);
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_Y, LOW);
  digitalWrite(LED_G, LOW);
}

void vSetLed(int step) {
  vAllLedsOff();
  digitalWrite(SEQ_LEDS[step], HIGH);
}

// ── Tasca FreeRTOS: seqüència de leds ────────────────────────────────────────
void vTaskSequence(void* pvParameters) {
  for (;;) {
    if (!bSeqRunning) {
      xSemaphoreTake(xSeqSemaphore, portMAX_DELAY);
    }
    iSeqStep = (iSeqStep + 1) % SEQ_LEN;
    vSetLed(iSeqStep);
    Serial.println(SEQ_NAMES[iSeqStep]);

    int elapsed = 0;
    while (elapsed < SEQ_DELAY_MS) {
      vTaskDelay(10 / portTICK_PERIOD_MS);
      elapsed += 10;
      if (!bSeqRunning) break;
    }
  }
}

// ── Pantalla OLED ─────────────────────────────────────────────────────────────

void vScreenTitle() {
  if (xSemaphoreTake(xDisplayMutex, pdMS_TO_TICKS(20)) == pdTRUE) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "IoT-02_seq00");
    display.display();
    xSemaphoreGive(xDisplayMutex);
  }
}

void vScreenSeq01Msg() {
  if (xSemaphoreTake(xDisplayMutex, pdMS_TO_TICKS(20)) == pdTRUE) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "IoT-02_seq01");
    display.display();
    xSemaphoreGive(xDisplayMutex);
  }
}

void vScreenMain(bool bI0, bool bI34, bool bI35) {
  int  ldrPct = (100 * analogRead(LDR)) / 4095;
  int  step   = iSeqStep;
  bool running= bSeqRunning;

  if (xSemaphoreTake(xDisplayMutex, pdMS_TO_TICKS(20)) == pdTRUE) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0,  0, String("I0 ")  + (bI0  ? "premut" : "no premut"));
    display.drawString(0, 12, String("I34 ") + (bI34 ? "premut" : "no premut"));
    display.drawString(0, 24, String("I35 ") + (bI35 ? "premut" : "no premut"));
    display.drawString(0, 36, "LDR: " + String(ldrPct) + "%");
    if (running || bSeqEverRan) {
      display.drawString(0, 48, LED_LABELS[step]);
    }
    display.display();
    xSemaphoreGive(xDisplayMutex);
  }
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);

  pinMode(LED_W,  OUTPUT);
  pinMode(LED_R,  OUTPUT);
  pinMode(LED_Y,  OUTPUT);
  pinMode(LED_G,  OUTPUT);
  pinMode(BT_IO0, INPUT);
  pinMode(BT_I34, INPUT);
  pinMode(BT_I35, INPUT);
  vAllLedsOff();

  Wire.begin(I2C_SDA, I2C_SCL);
  display.init();
  display.flipScreenVertically();

  xSeqSemaphore = xSemaphoreCreateBinary();
  xDisplayMutex = xSemaphoreCreateMutex();

  // vScreenTitle();
  vScreenSeq01Msg();

  xTaskCreatePinnedToCore(vTaskSequence, "SeqTask", 2048, NULL, 1, NULL, 0);
}

// ── Loop ──────────────────────────────────────────────────────────────────────
void loop() {
  static bool bIO0wasPressed = false;
  static bool bI35wasPressed = false;
  static bool bI34wasPressed = false;

  bool bIO0currentState = bPressedButton(BT_IO0);
  bool bI34currentState = bPressedButton(BT_I34);
  bool bI35currentState = bPressedButton(BT_I35);

  // ── Botó I0 ──
  if (bIO0wasPressed != bIO0currentState) {
    delay(10);
    if (bIO0currentState) {
      Serial.println("I0 premut");
      bScreenReady  = true;
      bShowSeq01Msg = false;  // la seqüència reprèn: desapareix el missatge
      if (!bSeqRunning) {
        bSeqRunning  = true;
        bSeqEverRan  = true;
        xSemaphoreGive(xSeqSemaphore);
      }
    } else {
      Serial.println("I0 alliberat");
    }
    bIO0wasPressed = bIO0currentState;
  }

  // ── Botó I34 ──
  if (bI34wasPressed != bI34currentState) {
    delay(10);
    if (bI34currentState) {
      Serial.println("I34 premut");
      bScreenReady = true;
      // Toggle del missatge només si la seqüència ha arrencat i està aturada
      if (bSeqEverRan && !bSeqRunning) {
        bShowSeq01Msg = !bShowSeq01Msg;
      }
    } else {
      Serial.println("I34 alliberat");
    }
    bI34wasPressed = bI34currentState;
  }

  // ── Botó I35 ──
  if (bI35wasPressed != bI35currentState) {
    delay(10);
    if (bI35currentState) {
      Serial.println("I35 premut");
      bScreenReady = true;
      bSeqRunning  = false;  // pausa; el led actiu queda encès
    } else {
      Serial.println("I35 alliberat");
    }
    bI35wasPressed = bI35currentState;
  }

  // ── Refresc de pantalla ──
  if (bScreenReady) {
    if (bShowSeq01Msg) {
      vScreenSeq01Msg();
    } else {
      vScreenMain(bIO0currentState, bI34currentState, bI35currentState);
    }
  }

  delay(10);
}
