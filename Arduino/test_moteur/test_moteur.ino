int pinI1=13;//define I1 port
int pinI2=12;//define I2 port
int speedpinA=11;//define EA(PWM speed regulation)port

int pinI3=10;//define I1 port
int pinI4=9;//define I2 port
int speedpinB=8;//define EA(PWM speed regulation)port

void setup()
{
  pinMode(pinI1,OUTPUT);//define this port as output
  pinMode(pinI2,OUTPUT);
  pinMode(speedpinA,OUTPUT);

  pinMode(pinI3,OUTPUT);//define this port as output
  pinMode(pinI4,OUTPUT);
  pinMode(speedpinB,OUTPUT);

  Serial.begin(9600);
  /*
 digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
   digitalWrite(pinI2,LOW);
   analogWrite(speedpinA,100);
   digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
   digitalWrite(pinI4,LOW);
   analogWrite(speedpinB,100);
   */
}

void avance(int vit){
  vitesse = vit * 2.55;
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
  vitesse = vit * 2.55;
  if (angle <0) {
    digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
    digitalWrite(pinI2,LOW);
    digitalWrite(pinI3,LOW);// DC motor rotates clockwise
    digitalWrite(pinI4,HIGH);
    analogWrite(speedpinA,abs(vitesse));
    analogWrite(speedpinB,abs(vitesse));
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
    analogWrite(speedpinA,abs(vitesse));
    analogWrite(speedpinB,abs(vitesse));
  }
}

void loop()
{
  delay(2000);
  avance(255);
  delay(5000);
  avance(0);
  // delay(2000);
  // digitalWrite(pinI1,HIGH);// DC motor rotates anticlockwise
  // digitalWrite(pinI2,LOW);
  // analogWrite(speedpin,100);
  // delay(2000);
  // digitalWrite(pinI1,HIGH);// DC motor stop rotating
  // digitalWrite(pinI2,HIGH);
  // delay(2000);
  Serial.println("boucle");
}

