const int ledPin = 9;
const int buttonPin = 2;

const uint8_t dutyTable[5] = {0, 64, 128, 192, 255};

int dutyIndex = 0;
int direction = 1;

bool lastButtonState = HIGH;

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
  Serial.println("------------Start------------");
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  analogWrite(ledPin, dutyTable[dutyIndex]);
}

void loop() {
  bool reading = digitalRead(buttonPin);

  if (reading == LOW && lastButtonState == HIGH) {
    Serial.println("BUTTON PRESS");

    dutyIndex += direction;
    if (dutyIndex == 4) direction = -1;
    if (dutyIndex == 0) direction = 1;

    analogWrite(ledPin, dutyTable[dutyIndex]);
    delay(300); // crude debounce for testing
  }

  lastButtonState = reading;
}
