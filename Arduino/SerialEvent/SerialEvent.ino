// Calculate based on max input size expected for one command
#define INPUT_SIZE 9
#define COMMAND_SIZE 4

char input[INPUT_SIZE];
boolean stringComplete = false;

char functionCalled;
int params[2];

int pinI1=13;//define I1 port
int pinI2=12;//define I2 port
int speedpinA=11;//define EA(PWM speed regulation)port

int pinI3=10;//define I3 port
int pinI4=9;//define I4 port
int speedpinB=8;//define EB(PWM speed regulation)port


void setup() {
  pinMode(pinI1,OUTPUT);//define this port as output
  pinMode(pinI2,OUTPUT);
  pinMode(speedpinA,OUTPUT);

  pinMode(pinI3,OUTPUT);//define this port as output
  pinMode(pinI4,OUTPUT);
  pinMode(speedpinB,OUTPUT);
  Serial.begin(115200); // initialize serial
}

void serialEvent() {
  while(Serial.available()>0){
    Serial.readBytes(input, INPUT_SIZE);
    stringComplete = true;
  }
}

void commandReceived(){
  functionCalled = input[0];
  char temp1[COMMAND_SIZE];
  char temp2[COMMAND_SIZE];
  memcpy(temp1, &input[1], COMMAND_SIZE*sizeof(char));
  memcpy(temp2, &input[5], COMMAND_SIZE*sizeof(char));
  params[0] = atoi(temp1);
  params[1] = atoi(temp2);
  stringComplete = false;
}

int sign ( int number){
  return (number >= 0 ? 1 : -1);
}

void avance(int vitesse){
  if (vitesse == 0){
    /*digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
     digitalWrite(pinI2,HIGH);
     digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
     digitalWrite(pinI4,HIGH); 
     analogWrite(speedpinA,0);
     analogWrite(speedpinB,0); */

    arretRobot();
  }
  else {
    vitesse = abs(vitesse) > 100 ? 100 * sign(vitesse) : vitesse;
    if (vitesse <0) {
      // Backward
      digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
      digitalWrite(pinI2,LOW);
      digitalWrite(pinI3,LOW);// DC motor rotates clockwise
      digitalWrite(pinI4,HIGH);
    }
    else{
      // Forward
      digitalWrite(pinI1,LOW);// DC motor rotates clockwise
      digitalWrite(pinI2,HIGH);
      digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
      digitalWrite(pinI4,LOW);
    }
    int vit = abs(vitesse*2.55);
    analogWrite(speedpinA,vit);
    analogWrite(speedpinB,vit);
  }
}

void rotate(int angle, int vit = 50){
  int vitesse = vit * 2.55;
  if (vitesse == 0){
    /*digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
     digitalWrite(pinI2,HIGH);
     digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
     digitalWrite(pinI4,HIGH);*/

    arretRobot();
  }
  else{
    if (angle <0) {
      digitalWrite(pinI1,LOW);// DC motor rotates clockwise
      digitalWrite(pinI2,HIGH);
      digitalWrite(pinI3,LOW);// DC motor rotates clockwise
      digitalWrite(pinI4,HIGH);
    }
    else{
      digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
      digitalWrite(pinI2,LOW);
      digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
      digitalWrite(pinI4,LOW);
    }
    analogWrite(speedpinA,abs(vitesse));
    analogWrite(speedpinB,abs(vitesse));
  }
}

void motorsSpeed(int speedLeft, int speedRight){
  if (speedLeft == 0 && speedRight == 0){
    /*digitalWrite(pinI1,HIGH);
    digitalWrite(pinI2,HIGH);
    digitalWrite(pinI3,HIGH);
    digitalWrite(pinI4,HIGH);
    analogWrite(speedpinA,0);
    analogWrite(speedpinB,0); */
   
   arretRobot(); 
  }
  else {
    speedLeft = abs(speedLeft) > 100 ? 100 * sign(speedLeft)*2.55 : speedLeft*2.55;
    speedRight = abs(speedRight) > 100 ? 100 * sign(speedRight)*2.55 : speedRight*2.55;
    if (speedLeft > 0){
      digitalWrite(pinI1,LOW);// DC motor rotates clockwise
      digitalWrite(pinI2,HIGH);
    }
    else {
      digitalWrite(pinI1,HIGH);// DC motor rotates anticlockwise
      digitalWrite(pinI2,LOW);
    }
    if (speedRight > 0){
      digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
      digitalWrite(pinI4,LOW);
    }
    else{
      digitalWrite(pinI3,LOW);// DC motor rotates anticlockwise
      digitalWrite(pinI4,HIGH);
    }
    analogWrite(speedpinA,abs(speedLeft));
    analogWrite(speedpinB,abs(speedRight));
  }
}

void arretRobot(){
  digitalWrite(pinI1,HIGH);
  digitalWrite(pinI2,HIGH);
  digitalWrite(pinI3,HIGH);
  digitalWrite(pinI4,HIGH);

  analogWrite(speedpinA,0);
  analogWrite(speedpinB,0);
}

void loop() {
  if(stringComplete){
    commandReceived();
    if(functionCalled == 'a'){
      //Serial.print("avancer..");
      //Serial.println(params[0]);
      avance(params[0]);
    }
    else if(functionCalled == 'r'){
      //Serial.print("tourner..");
      //Serial.println(params[0]);
      rotate(params[0]);
    }
    else if(functionCalled == 'm'){
      //Serial.print("motorsSpeed..");
      //Serial.print(params[0]);
      //Serial.print(params[1]);
      motorsSpeed(params[0],params[1]);
    }
    else if(functionCalled == 's'){
      //Serial.print("stop..");
      arretRobot();
    }
  }
}














