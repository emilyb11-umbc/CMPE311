//Set led variables to corresponding pin number
const int led1 = 2;   
const int led2 = 3;  
const int led3 = 4;
const int led4 = 5;
const int led5 = 6;
const int led6 = 7;
const int led7 = 8;
const int led8 = 9;

//Initialize variables for the interval between blinkes for 8 leds
int led1Interval = 0; 
int led2Interval = 0; 
int led3Interval = 0; 
int led4Interval = 0; 
int led5Interval = 0; 
int led6Interval = 0; 
int led7Interval = 0;
int led8Interval = 0;  

//Initialize varibales for the last time the state has been switched for 8 leds
unsigned long lastTimeLed1 = 0; 
unsigned long lastTimeLed2 = 0; 
unsigned long lastTimeLed3 = 0; 
unsigned long lastTimeLed4 = 0; 
unsigned long lastTimeLed5 = 0; 
unsigned long lastTimeLed6 = 0; 
unsigned long lastTimeLed7 = 0; 
unsigned long lastTimeLed8 = 0;  

//Initialize variables for current led state (off)
bool led1State = false; 
bool led2State = false; 
bool led3State = false; 
bool led4State = false; 
bool led5State = false; 
bool led6State = false; 
bool led7State = false; 
bool led8State = false; 

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led7, LOW);
  digitalWrite(led8, LOW);

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
}

void loop() {
  static int currentLed = 0;
  static int currentInterval = 0;
  Serial.println();
  currentLed = getUserInput("What LED? (1 through 8)");
  currentInterval = getUserInput("What interval (in msec)?");
  currentInterval = currentInterval / 2;

//Sets the interval of inputted led number to the inputted interval, prints out statement of inputted result for clarity
  if (currentLed == 1) {
    led1Interval = currentInterval;
    Serial.print("LED 1 interval set to: ");
    Serial.println(led1Interval);
  } else if (currentLed == 2) {
    led2Interval = currentInterval;
    Serial.print("LED 2 interval set to: ");
    Serial.println(led2Interval);
  } else if (currentLed == 3) {
    led3Interval = currentInterval;
    Serial.print("LED 3 interval set to: ");
    Serial.println(led3Interval);
  } else if (currentLed == 4) {
    led4Interval = currentInterval;
    Serial.print("LED 4 interval set to: ");
    Serial.println(led4Interval);
  } else if (currentLed == 5) {
    led5Interval = currentInterval;
    Serial.print("LED 5 interval set to: ");
    Serial.println(led5Interval);
  } else if (currentLed == 6) {
    led6Interval = currentInterval;
    Serial.print("LED 6 interval set to: ");
    Serial.println(led6Interval);
  } else if (currentLed == 7) {
    led7Interval = currentInterval;
    Serial.print("LED 7 interval set to: ");
    Serial.println(led7Interval);
  } else if (currentLed == 8) {
    led8Interval = currentInterval;
    Serial.print("LED 8 interval set to: ");
    Serial.println(led8Interval);
  } else {
    Serial.println("Not a valid led");
  }

  blinking();
}


void blinking() {
  //continuously runs if statements that will check if it has been longer than the inputted interval since last time the led switched states
  //if so, will switch the led state and reset the last time it has been switched
  if (millis() - lastTimeLed1 >= led1Interval) {
    lastTimeLed1 = millis();
    led1State = !led1State; // Toggle state
    digitalWrite(led1, led1State ? HIGH : LOW);
  }
    if (millis() - lastTimeLed2 >= led2Interval) {
    lastTimeLed2 = millis();
    led2State = !led2State; // Toggle state
    digitalWrite(led2, led2State ? HIGH : LOW);
  }
  if (millis() - lastTimeLed3 >= led3Interval) {
    lastTimeLed3 = millis();
    led3State = !led3State; // Toggle state
    digitalWrite(led3, led3State ? HIGH : LOW);
  }
  if (millis() - lastTimeLed4 >= led4Interval) {
    lastTimeLed4 = millis();
    led4State = !led4State; // Toggle state
    digitalWrite(led4, led4State ? HIGH : LOW);
  }
  if (millis() - lastTimeLed5 >= led5Interval) {
    lastTimeLed5 = millis();
    led5State = !led5State; // Toggle state
    digitalWrite(led5, led5State ? HIGH : LOW);
  }
  if (millis() - lastTimeLed6 >= led6Interval) {
    lastTimeLed6 = millis();
    led6State = !led6State; // Toggle state
    digitalWrite(led6, led6State ? HIGH : LOW);
  }
  if (millis() - lastTimeLed7 >= led7Interval) {
    lastTimeLed7 = millis();
    led7State = !led7State; // Toggle state
    digitalWrite(led7, led7State ? HIGH : LOW);
  }
  if (millis() - lastTimeLed8 >= led8Interval) {
    lastTimeLed8 = millis();
    led8State = !led8State; // Toggle state
    digitalWrite(led8, led8State ? HIGH : LOW);
  }

}

int getUserInput(const char* prompt) {
  int value = 0;

    Serial.println(prompt);
    while (Serial.available() == 0) {
    blinking(); // Keep LEDs blinking while waiting for input
    }

    String input = Serial.readStringUntil('\n'); // Read user input
    input.trim(); 
    value = input.toInt();

  
  return value;
}
