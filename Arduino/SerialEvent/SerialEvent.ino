// Calculate based on max input size expected for one command
#define INPUT_SIZE 10

// Get next command from Serial (add 1 for final 0)
char input[INPUT_SIZE + 1];
boolean stringComplete = false;
byte size;

char* functionCalled;
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
  Serial.begin(9600);
}

void serialEvent() {
  if(Serial.available()>0){
    size = Serial.readBytes(input, INPUT_SIZE);
    while(Serial.available() >0){
      char t = Serial.read();
    }
    stringComplete = true;
    Serial.println(input);
  }
}

void commandReceived(){
  input[size] = 0;
  int i = 0;
  memset(&params[0], 0 ,sizeof(params)/sizeof(int));
  // Read each command pair
  char* endTrame = strtok(input, "!"); 
  char* command = strtok(endTrame, ":");
  while (command != 0)
  {
    char* separator = strchr(command, '&');
    if (separator != 0)
    {
      *separator = 0;
      functionCalled = command;
      ++separator;
      params[i] = atoi(separator);
      i++;
    }
    else{
      params[i] = atoi(command);
      i++;
    }
    // Find the next command in input string
    command = strtok(0, ":");
  }
  stringComplete = false;
}

void avance(int vit){
  int vitesse = vit * 2.55;
  cli();
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
  sei();
}

void rotate(int angle, int vit = 50){
  cli();
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
  sei();
}

void loop() {
  if (stringComplete){// Add the final 0 to end the C string
    commandReceived();
    //Serial.println(functionCalled);
    //for (int i =0;i<sizeof(params)/sizeof(int);i++){
    //  Serial.println(params[i]);
    //}
    if(*functionCalled == 'a'){
      //Serial.print("avancer..");
      //Serial.println(params[0]);
      avance(params[0]);
    }
    else if(*functionCalled == 'r'){
      //Serial.print("tourner..");
      //Serial.println(params[0]);
      rotate(params[0]);
    }
    else{
      //Serial.println("erreur");
    }
  }
}




