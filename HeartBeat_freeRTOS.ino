/**********************************************************************
  Filename    : HeartBeat_freeRTOS
  Target      : ESP32-S3-WROOM
  Author      : Martin Harnevie
  Description : Just does the heartbeat in freeRTOS
  Logbook     :
  20261026 000 - added another thread as well just to show a bit
  **********************************************************************/

const uint8_t   KPinLedInternal =            2; // internal LED pin

const uint32_t  KIntervalHeartBeat = 1000;
const uint32_t  KIntervalBlink = 58;

TaskHandle_t hHeartBeat = NULL;
TaskHandle_t hSomething = NULL;

void setup() {
  Serial.begin(115200);
  delay(1000);

  xTaskCreatePinnedToCore(
    heartBeatFreeRtos,  // callback to task function
    "Heart Beat",       // task name
    4096,               // stack size
    NULL,               // parameters
    1,                  // priority
    &hHeartBeat,        // handle
    1                   // Core 1
  );

  xTaskCreatePinnedToCore(
    justPrintSomething, // callback to task function
    "Something",        // task name
    4096,               // stack size
    NULL,               // parameters
    1,                  // priority
    &hSomething,        // handle
    1                   // Core 1
  );

  // heartbeat LED
  pinMode(KPinLedInternal,OUTPUT);
}

void loop() {

  /* nothing at this point */

}

void heartBeatFreeRtos(void *parameter) {
  for (;;) {
    digitalWrite(KPinLedInternal, HIGH);
    Serial.print("Heart beat: ");
    Serial.println(millis()/1000.0,3);
    vTaskDelay(KIntervalBlink / portTICK_PERIOD_MS); // 68 ms
    
    digitalWrite(KPinLedInternal, LOW);
    Serial.print("Heart has beaten on core ");
    Serial.println(xPortGetCoreID());
    vTaskDelay((KIntervalHeartBeat-KIntervalBlink) / portTICK_PERIOD_MS); // the period must be compensated with blink to ensure 1000 ms sharp
  }
}

void justPrintSomething(void *parameter) {
  for (;;) {
    Serial.print("---------------> Something at: ");
    Serial.print(millis()/1000.0,3);
    Serial.println();
    vTaskDelay(10000 / portTICK_PERIOD_MS); // 10000ms
  } 
}
