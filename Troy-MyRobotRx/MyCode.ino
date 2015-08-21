// Motor driver H bridge direction is controlled through the shift registes
// Motor speed controlled through PWM, on digital pins

//PWM outputs (These are flipped because I soldered one "backwards")
int PWMB = 9;
int PWMA = 10;

int aVal = 0;
int bVal = 0;

int oldG;
int gCounter = 0;

void setup() {
  shiftSetup();
  ReceiverSetup(92);
}

void loop() {
  GetData();
  delay(5);
  AntiJank();
  delay(5);
  ProcessData();
  delay(5);
  //PrintData();
  //delay(5);

  

  analogWrite(PWMA, abs(aVal));
  analogWrite(PWMB, abs(bVal));

}



void ProcessData() {
  // First, take both values and bring it from 0 - 510 to -255 - 255
  aVal = a - 255;
  bVal = b - 255;

  
    // Then, set the motor directions
    // Create a couple of local variables just to make the code prettier
    int aDirection;
    int bDirection;

    if (aVal == 0) {
      aDirection = 0;
    } else if (aVal > 0) {
      aDirection = 1;
    } else {
      aDirection = 2;
    }

    if (bVal == 0) {
      bDirection = 0;
    } else if (bVal > 0) {
      bDirection = 1;
    } else {
      bDirection = 2;
    }

    motorDirection(aDirection, bDirection);

}

void AntiJank() {
  if (g != oldG) {
    oldG = g;
  } else {
    gCounter++;
    Serial.println("JANK--------------------------------------");
    if (gCounter > 10) {
      ReceiverSetup(92);
      gCounter = 0;
    }
  }


}




