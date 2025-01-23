#include <PS2X_lib.h>  //for v1.6

PS2X ps2x;  // create PS2 Controller Class

const int MRFA = 5;
const int MRFB = 4;
const int MRBA = 3;
const int MRBB = 2;
// const int MLFA = 6;
// const int MLFB = 7;
// const int MLBA = 8;
// const int MLBB = 9;
#define MLFA A0
#define MLFB A1
#define MLBA A2
#define MLBB A3
int error = 0;
byte type = 0;
byte vibrate = 0;

void setup() {
  Serial.begin(57600);
  pinMode(MRFA, OUTPUT);
  pinMode(MRFB, OUTPUT);
  pinMode(MRBA, OUTPUT);
  pinMode(MRBB, OUTPUT);
  pinMode(MLFA, OUTPUT);
  pinMode(MLFB, OUTPUT);
  pinMode(MLBA, OUTPUT);
  pinMode(MLBB, OUTPUT);

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  error = ps2x.config_gamepad(13, 11, 10, 12, true, true);  //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if (error == 0) {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }
}

void loop() {
  if (error == 1)  //skip loop if no controller found
    return;

  if (type == 2) {  //Guitar Hero Controller

    ps2x.read_gamepad();  //read controller

    if (ps2x.ButtonPressed(GREEN_FRET))
      Serial.println("Green Fret Pressed");
    if (ps2x.ButtonPressed(RED_FRET))
      Serial.println("Red Fret Pressed");
    if (ps2x.ButtonPressed(YELLOW_FRET))
      Serial.println("Yellow Fret Pressed");
    if (ps2x.ButtonPressed(BLUE_FRET))
      Serial.println("Blue Fret Pressed");
    if (ps2x.ButtonPressed(ORANGE_FRET))
      Serial.println("Orange Fret Pressed");


    if (ps2x.ButtonPressed(STAR_POWER))
      Serial.println("Star Power Command");

    if (ps2x.Button(UP_STRUM))  //will be TRUE as long as button is pressed
      Serial.println("Up Strum");
    if (ps2x.Button(DOWN_STRUM))
      Serial.println("DOWN Strum");


    if (ps2x.Button(PSB_START))  //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if (ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");


    if (ps2x.Button(ORANGE_FRET))  // print stick value IF TRUE
    {
      Serial.print("Wammy Bar Position:");
      Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);
    }
  }

  else {                                //DualShock Controller
    ps2x.read_gamepad(false, vibrate);  //read controller and set large motor to spin at 'vibrate' speed

    if (ps2x.Button(PSB_START))  //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if (ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");


    // if (ps2x.Button(PSB_PAD_UP)) {  //will be TRUE as long as button is pressed
    //   forward();
    // }
    if (ps2x.Analog(PSS_LY) >= 0 && ps2x.Analog(PSS_LY) < 127) {
      forward();
    } else if (ps2x.Analog(PSS_LY) > 127 && ps2x.Analog(PSS_LY) <= 255) {
      backward();
    } else {
      if (ps2x.Analog(PSS_LX) > 128) {
        turnright();
      } else if (ps2x.Analog(PSS_LX) < 128) {
        turnleft();
      } else if (ps2x.Analog(PSS_RX) > 128) {
        slideright();
      } else if (ps2x.Analog(PSS_RX) < 128) {
        slideleft();
      } else {
        stop();
      }
    }

    vibrate = ps2x.Analog(PSAB_BLUE);  //this will set the large motor vibrate speed based on
                                       //how hard you press the blue (X) button
    if (ps2x.NewButtonState())  //will be TRUE if any button changes state (on to off, or off to on)
    {
      if (ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if (ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if (ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if (ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if (ps2x.Button(PSB_GREEN))
        Serial.println("Triangle pressed");
    }

    if (ps2x.NewButtonState(PSB_BLUE))  //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");


    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))  // print stick values if either is TRUE
    {
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC);  //Left stick, Y axis. Other options: LX, RY, RX
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC);
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC);
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
    }
  }


  delay(50);
}

void forward() {
  Serial.println("Forward !");
  digitalWrite(MRFA, HIGH);
  digitalWrite(MRFB, LOW);

  digitalWrite(MRBA, HIGH);
  digitalWrite(MRBB, LOW);

  digitalWrite(MLFA, HIGH);
  digitalWrite(MLFB, LOW);

  digitalWrite(MLBA, HIGH);
  digitalWrite(MLBB, LOW);
}

void backward() {
  Serial.println("Backward !");
  digitalWrite(MRFA, LOW);
  digitalWrite(MRFB, HIGH);

  digitalWrite(MRBA, LOW);
  digitalWrite(MRBB, HIGH);

  digitalWrite(MLFA, LOW);
  digitalWrite(MLFB, HIGH);

  digitalWrite(MLBA, LOW);
  digitalWrite(MLBB, HIGH);
}

void turnleft() {
  Serial.println("Turn Left");
  digitalWrite(MRFA, HIGH);
  digitalWrite(MRFB, LOW);

  digitalWrite(MRBA, HIGH);
  digitalWrite(MRBB, LOW);

  digitalWrite(MLFA, LOW);
  digitalWrite(MLFB, HIGH);

  digitalWrite(MLBA, LOW);
  digitalWrite(MLBB, HIGH);
}

void turnright() {
  Serial.println("Turn Right");
  digitalWrite(MRFA, LOW);
  digitalWrite(MRFB, HIGH);

  digitalWrite(MRBA, LOW);
  digitalWrite(MRBB, HIGH);

  digitalWrite(MLFA, HIGH);
  digitalWrite(MLFB, LOW);

  digitalWrite(MLBA, HIGH);
  digitalWrite(MLBB, LOW);
}

void slideright() {
  Serial.println("Slide Right");
  digitalWrite(MRFA, LOW);
  digitalWrite(MRFB, HIGH);

  digitalWrite(MRBA, HIGH);
  digitalWrite(MRBB, LOW);

  digitalWrite(MLFA, HIGH);
  digitalWrite(MLFB, LOW);

  digitalWrite(MLBA, LOW);
  digitalWrite(MLBB, HIGH);
}

void slideleft() {
  Serial.println("Slide Left");
  digitalWrite(MRFA, HIGH);
  digitalWrite(MRFB, LOW);

  digitalWrite(MRBA, LOW);
  digitalWrite(MRBB, HIGH);

  digitalWrite(MLFA, LOW);
  digitalWrite(MLFB, HIGH);

  digitalWrite(MLBA, HIGH);
  digitalWrite(MLBB, LOW);
}

void stop() {
  Serial.println("Stop !");
  digitalWrite(MRFA, LOW);
  digitalWrite(MRFB, LOW);

  digitalWrite(MRBA, LOW);
  digitalWrite(MRBB, LOW);

  digitalWrite(MLFA, LOW);
  digitalWrite(MLFB, LOW);

  digitalWrite(MLBA, LOW);
  digitalWrite(MLBB, LOW);
}
