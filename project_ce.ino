/*
  Project-CE: Cyclic Executive Task Manager
  Author: Emily Bearden
  Date: November 11, 2025

  Description:
  Implements a cyclic executive that manages two LED blink tasks
  and a non-blocking serial input task using a function pointer array.
  Each task is called in round-robin order.
*/

const int led1 = 9;
const int led2 = 8;

int led1Interval = 1000; // default period
int led2Interval = 1000;
bool led1State = LOW;
bool led2State = LOW;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

// Serial input management
int selectedLED = 0;
int inputInterval = 0;
String inputBuffer = "";
bool waitingForLED = true;

// -------- Function Declarations --------
void ReadUserInputTask();
void UpdateLED1Task();
void UpdateLED2Task();

// -------- Function Pointer Array (Cyclic Executive) --------
typedef void (*TaskFn)();
TaskFn tasks[] = { ReadUserInputTask, UpdateLED1Task, UpdateLED2Task };
const int NUM_TASKS = sizeof(tasks) / sizeof(tasks[0]);

// -------- Setup --------
void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  Serial.begin(9600);
  Serial.println();
  Serial.println("------------------------------------");
  Serial.println("Project-CE: Cyclic Executive LED Control");
  Serial.println("What LED? (1 or 2)");
}

// -------- Main Loop: Round-Robin Cyclic Executive --------
void loop() {
  for (int i = 0; i < NUM_TASKS; i++) {
    tasks[i](); // Dispatch task
  }
}

// -------- Task 1: Handle User Input (Non-blocking) --------
void ReadUserInputTask() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    // Ignore carriage returns
    if (c == '\r') continue;

    // Newline = user finished typing
    if (c == '\n') {
      inputBuffer.trim();
      if (inputBuffer.length() > 0) {
        if (waitingForLED) {
          // Expect 1 or 2
          if (inputBuffer == "1" || inputBuffer == "2") {
            selectedLED = inputBuffer.toInt();
            waitingForLED = false;
            Serial.print("Selected LED ");
            Serial.println(selectedLED);
            Serial.println("What interval (in ms)?");
          } else {
            Serial.println("Invalid input. Enter 1 or 2:");
          }
        } else {
          // Expect interval value
          int val = inputBuffer.toInt();
          if (val > 0) {
            inputInterval = val;

            if (selectedLED == 1) {
              led1Interval = inputInterval;
            } else if (selectedLED == 2) {
              led2Interval = inputInterval;
            }

            Serial.print("LED ");
            Serial.print(selectedLED);
            Serial.print(" interval set to ");
            Serial.print(inputInterval);
            Serial.println(" ms");

            // Reset for next input sequence
            waitingForLED = true;
            Serial.println("What LED? (1 or 2)");
          } else {
            Serial.println("Invalid interval. Enter a positive integer:");
          }
        }
      }
      inputBuffer = ""; // reset buffer
    } else {
      inputBuffer += c; // build input string
    }
  }
}

// -------- Task 2: LED1 Update --------
void UpdateLED1Task() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis1 >= (unsigned long)led1Interval / 2) {
    previousMillis1 = currentMillis;
    led1State = !led1State;
    digitalWrite(led1, led1State);
  }
}

// -------- Task 3: LED2 Update --------
void UpdateLED2Task() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis2 >= (unsigned long)led2Interval / 2) {
    previousMillis2 = currentMillis;
    led2State = !led2State;
    digitalWrite(led2, led2State);
  }
}

