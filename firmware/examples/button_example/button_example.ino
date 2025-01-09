/*
  Octopus Button Example
  
  - The sketch continuously reads the state of the button.
  - If the button is pressed (LOW state), it prints "Button pressed" to the Serial Monitor.
  - If the button is not pressed (HIGH state), it prints "Button not pressed" to the Serial Monitor.
  - A delay of 500 milliseconds is added between reads to avoid overwhelming the Serial Monitor 
    with too many messages.
*/

const int buttonPin = 7;  // Pin connected to the button

void setup() {
  Serial.begin(9600);         // Start the serial communication
  pinMode(buttonPin, INPUT_PULLUP);  // Set the button pin as an input with internal pull-up resistor
}

void loop() {
  int buttonState = digitalRead(buttonPin);  // Read the state of the button

  if (buttonState == LOW) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button not pressed");
  }

  delay(500);  // Delay for half a second
}


