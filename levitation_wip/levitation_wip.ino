/*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01

    by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#define enA 9
#define in1 6
#define in2 7
#define button 4

int movementDirection = 0;
int pressed = false;

//int positionBuffer = 0;
int magicPosition = 185;

void setup() {
  Serial.begin(9600);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  // Set initial movementDirection direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void loop() {
  int buttonValue = digitalRead(button);
  
  //int potValue = analogRead(A1); // Read potentiometer value
  
  int fieldStrength = analogRead(A0); // Read hall sensor value
  //Serial.println(fieldStrength);

  //if button is pressed, store value of field strength as magic value for floating position reference
  if(buttonValue == 0){
    pressed = true;
    magicPosition = fieldStrength;
  }
  
  //Serial.println(magicPosition);
  
  //retrieve previous position from memory
  //int previousPosition = positionBuffer;

  //save new position to memory for later use, now that you've already retrieved the old value
  //positionBuffer = fieldStrength;

  //int currentVelocity = fieldStrength - previousPosition;

  int positionError = magicPosition - fieldStrength;

  //int counterFieldStrenth = positionError;

  Serial.println(positionError);

  //reverse counter-field depending on if the target is located too close or far from the sensor
  if(positionError < 0){
    //reverse the counter-field direction
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    
    movementDirection = 0;

    //flip the sign to positive so it can be mapped properly for intensity
    positionError = -positionError;
  } else{
    //reverse the counter-field direction
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    
    movementDirection = 1;
  }

  if(pressed){
    //int pwmOutput = 255;
    int pwmOutput = map(positionError, 0, 3, 0 , 100); // Map the sensor value from 0 to 255
    analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
  }
  
  //guess at good max value for sensor range, needs revising
  //int pwmOutput = map(positionError, 0, 100, 0 , 255); // Map the sensor value from 0 to 255
  //analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin

  //todo: account for the counter-field affecting the sensor. probably could have it calibrate itself by trying different intensities and recording the resulting field. could either use those recorded values directly as new reference points, or could derive a function from it to use later. especially if that's too much memory.

  //direct-feed velocity into field intensity. probably not a good idea, velocity technically independent of relative position

  //reverse counter-field depending on if the target is moving towards or away from the sensor
  //if(currentVelocity < 0){
  //  digitalWrite(in1, LOW);
  //  digitalWrite(in2, HIGH);
  //  rotDirection = 0;
  //  //delay(20);
  //}

  //int pwmOutput = map(currentVelocity, 0, 1023, 0 , 255); // Map the sensor value from 0 to 255
  //analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
}
