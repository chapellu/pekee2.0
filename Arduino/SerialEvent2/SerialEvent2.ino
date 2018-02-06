// Calculate based on max input size expected for one command
#define INPUT_SIZE 6
#define MESSAGE_SIZE 5
#define PARAMETER_SIZE

char input[INPUT_SIZE];
boolean stringComplete = false;

char functionCalled;
int params[5];

int pinI1=13;//define I1 port
int pinI2=12;//define I2 port
int speedpinA=11;//define EA(PWM speed regulation)port

int pinI3=10;//define I1 port
int pinI4=9;//define I2 port
int speedpinB=8;//define EA(PWM speed regulation)port


void setup() {
  // initialize serial:
  pinMode(pinI1,OUTPUT);//define this port as output
  pinMode(pinI2,OUTPUT);
  pinMode(speedpinA,OUTPUT);

  pinMode(pinI3,OUTPUT);//define this port as output
  pinMode(pinI4,OUTPUT);
  pinMode(speedpinB,OUTPUT);
  Serial.begin(115200);
}

void serialEvent() {
  if(Serial.available()>0){
    Serial.readBytes(input, MESSAGE_SIZE);
    stringComplete = true;
  }
}

void commandReceived(){
  functionCalled = input[0];
  char temp[4];
  for (int i = 1; i<5; i++){
    temp[i-1] = input[i];
  }
  params[0] = atoi(temp);
  
 
}

void avance(int vit){
  int vitesse = vit * 2.55;
  if (vitesse <0) {
    digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
    digitalWrite(pinI2,LOW);
    digitalWrite(pinI3,LOW);// DC motor rotates clockwise
    digitalWrite(pinI4,HIGH);
  }
  else if (vitesse == 0){
    digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
    digitalWrite(pinI2,HIGH);
    digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
    digitalWrite(pinI4,HIGH);
  }
  else{
    digitalWrite(pinI1,LOW);// DC motor rotates clockwise
    digitalWrite(pinI2,HIGH);
    digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
    digitalWrite(pinI4,LOW);

  }
  analogWrite(speedpinA,abs(vitesse));
  analogWrite(speedpinB,abs(vitesse));
}

void rotate(int angle, int vit = 50){
  int vitesse = vit * 2.55;
  if (angle <0) {
    digitalWrite(pinI1,LOW);// DC motor rotates clockwise
    digitalWrite(pinI2,HIGH);
    digitalWrite(pinI3,LOW);// DC motor rotates clockwise
    digitalWrite(pinI4,HIGH);
  }
  else if (vitesse == 0){
    digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
    digitalWrite(pinI2,HIGH);
    digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
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

void arretRobot(){
  int vitesse = 0;
  digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
  digitalWrite(pinI2,HIGH);
  digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
  digitalWrite(pinI4,HIGH);

  analogWrite(speedpinA,abs(vitesse));
  analogWrite(speedpinB,abs(vitesse));
}

void loop() {
  
  if(stringComplete){// Add the final 0 to end the C string
    commandReceived();
    //Serial.println(functionCalled);
    //for (int i =0;i<sizeof(params)/sizeof(int);i++){
    //  Serial.println(params[i]);
    //}
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
    else if(functionCalled == 's'){
      arretRobot();
    }
  }
}











