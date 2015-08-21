// FPS style control scehem (I think)
// Left stick for forward back
// Right stick for left right
// It will work as a throttle and rudder. (kinda)

// Setup pin numbers
// Note, these dont match the printed labels
// They match what actully happens when moving the joysticks
int LjoystickXpin = A6;
int LjoystickYpin = A7;
int LjoystickSWpin = A5;
int RjoystickXpin = A1;
int RjoystickYpin = A2;
int RjoystickSWpin = A3;

// Setup pin analog values
int LjoystickX = 0;
int LjoystickY = 0;
int LjoystickSW = 0;
int RjoystickX = 0;
int RjoystickY = 0;
int RjoystickSW = 0;

//Deadzone 40 wide
int deadHigh = 532;
int deadLow = 492;
//Maxzone 20 wide each
int maxHigh = 1018;
int maxLow = 5;


void setup() {
  Serial.begin(57600);
  TransmitterSetup(92);
}

void loop() {
  ReadJoysticks();
  PrepareData();
  delay(2);
  SendData();
  delay(1);
  //PrintData();
}

void ReadJoysticks() {
  LjoystickX = analogRead(LjoystickXpin);
  delay(1);
  LjoystickY = analogRead(LjoystickYpin);
  delay(1);
  LjoystickSW = analogRead(LjoystickSWpin);
  delay(1);
  RjoystickX = analogRead(RjoystickXpin);
  delay(1);
  RjoystickY = analogRead(RjoystickYpin);
  delay(1);
  RjoystickSW = analogRead(RjoystickSWpin);
  delay(1);

}

void PrepareData() {  
  // Random number for anti jank
  g = random(10000);

  /*
  Ok, planning here

  Left joystick will be used for forward back
  Aka throttle or power
  So, everything will scale with it?
  Kinda?

  Ok, first of all, data being transmitted:
  a-Left motor power (+255)
  b-Right motor power (+255)
  c-Left stick in
  d-Right stick in
  g-Button 1
  f-Button 2
  g-Button 3
  h-Button 4
  i-Nothing
  j-Nothing
  k-Nothing
  l-Nothing
  m-Nothing
  n-Nothing
  o-Nothing
  p-Authentication code

  Ok, now that that's all planned, how will we determine the values to send?
  I'll map out some cases:

  Left stick full forward, right stick middle:
  Both forward full power
  Left full back, right middle:
  Both back full power
  Left full forward, right full left:
  Right full power, left off?
  (And similar)
  Left middle, Right full left:
  Right full back, left full forward (Hopefully that will turn on the spot)
  Left half up, right middle:
  Both forward half power

  This is a design decision now.  I think i'll do this for now.
  It should be the easiest, and it should actually work fairly well!

  Basically, the left stick will just scale everything.
  Like, a scalar.  Can be negative.  (Or something)

  Left half up, right full left:
  Right half, left off
  Left half up, right half left:


   */

  // I'm going to modify the raw input data, based on the dead/max zones
  // This could be bad form, but I dont want to make more variables

  if (LjoystickY > maxHigh) {
    LjoystickY = maxHigh;
  } else if (LjoystickY > deadHigh) {
    //Leave it
  } else if (LjoystickY > deadLow) {
    LjoystickY = 512;
  } else if (LjoystickY > maxLow) {
    //Leave it
  } else {
    LjoystickY = maxLow;
  }

  if (RjoystickX > maxHigh) {
    RjoystickX = maxHigh;
  } else if (RjoystickX > deadHigh) {
    //Leave it
  } else if (RjoystickX > deadLow) {
    RjoystickX = 512;
  } else if (RjoystickX > maxLow) {
    //Leave it
  } else {
    RjoystickX = maxLow;
  }

  //Creating motor values.
  if (LjoystickY == 512) {
    if (RjoystickX == 512) {
      //Stopped
      a = 255;
      b = 255;
    } else {
      // Turning on the spot
      a = map (RjoystickX, maxLow, maxHigh, 0, 510);
      b = 510 - a;
    }
  } else {
    // Moving
    if (RjoystickX == 512) {
      // Straight
      a = map (LjoystickY, maxLow, maxHigh, 0, 510);
      b = a;
    } else if (RjoystickX < 512) {
      // Turing left
      b = map (LjoystickY, maxLow, maxHigh, 0, 510);
      a = map (RjoystickX, maxLow, maxHigh, 255, b);
    } else {
      // Turning right
      a = map (LjoystickY, maxLow, maxHigh, 0, 510);
      b = map (RjoystickX, maxLow, maxHigh, a, 255);
    }
  }

  // Now transmit them.  Why did they do this inline in the original code?
  // Remember, these are being transmitted raised by 255.
  // They go from 0 to 510, whereas they should go from -255 to 255
  // Negative values signify backwards
  // When the receiver receives the values, it will have to subtract them by 25
}


