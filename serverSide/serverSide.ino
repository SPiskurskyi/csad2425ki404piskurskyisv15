#include "include/constants.h"

void setup() {
  Serial.begin(BAUDRATE);
  while (!Serial) {
    ;
  }
  Serial.println("Arduino ready!");
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n');  // Read the incoming message until newline
    receivedMessage.trim();  // Remove any extra whitespace

    // Modify the message (for example, convert to uppercase)
    receivedMessage.toUpperCase();
    
    // Send the modified message back to the client
    Serial.println(receivedMessage);
  }
}
