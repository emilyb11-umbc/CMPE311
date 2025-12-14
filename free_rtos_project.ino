#include <Arduino_FreeRTOS.h>

// --------------------
// Pin Definitions
// --------------------
const int ledPin = 9;       // PWM LED output
const int buttonPin = 2;    // Jumper wire "button"

// --------------------
// Duty Cycle Table
// --------------------
const uint8_t dutyTable[5] = {0, 64, 128, 192, 255};
volatile int dutyIndex = 0;
volatile int direction = 1;

// --------------------
// Task Handles
// --------------------
TaskHandle_t ButtonTaskHandle;
TaskHandle_t PWMTaskHandle;

// --------------------
// Button Task
// --------------------
void ButtonTask(void *pvParameters) {
  bool lastState = HIGH;

  for (;;) {
    bool currentState = digitalRead(buttonPin);

    // Detect button press (HIGH → LOW)
    if (currentState == LOW && lastState == HIGH) {
      xTaskNotifyGive(PWMTaskHandle);
    }

    lastState = currentState;
    vTaskDelay(50 / portTICK_PERIOD_MS); // debounce
  }
}

// --------------------
// PWM Duty Cycle Task
// --------------------
void PWMTask(void *pvParameters) {
  for (;;) {
    // Wait for button event
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    dutyIndex += direction;
    if (dutyIndex == 4) direction = -1;
    if (dutyIndex == 0) direction = 1;

    uint8_t pwmValue = dutyTable[dutyIndex];
    analogWrite(ledPin, pwmValue);

    // ----- Serial Output -----
    Serial.print("Duty Index: ");
    Serial.print(dutyIndex);
    Serial.print(" | PWM Value: ");
    Serial.print(pwmValue);
    Serial.print(" | Duty Cycle: ");
    Serial.print((pwmValue * 100) / 255);
    Serial.println("%");
  }
}

// --------------------
// Setup
// --------------------
void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("--------Start--------");

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  analogWrite(ledPin, dutyTable[dutyIndex]);

  xTaskCreate(ButtonTask, "Button Task", 128, NULL, 1, &ButtonTaskHandle);
  xTaskCreate(PWMTask, "PWM Task", 128, NULL, 1, &PWMTaskHandle);
}

// --------------------
// Loop (Unused)
// --------------------
void loop() {
  // FreeRTOS scheduler runs tasks
}
