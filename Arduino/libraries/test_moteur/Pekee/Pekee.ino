// Moteur Droite
int pinI1=13;//define I1 port
int pinI2=12;//define I2 port
int speedpinA=11;//define EA(PWM speed regulation)port

//Moteur Gauche
int pinI3=10;//define I1 port
int pinI4=9;//define I2 port
int speedpinB=8;//define EA(PWM speed regulation)port

int test = -100;

void setup() {
 pinMode(pinI1,OUTPUT);//define this port as output
 pinMode(pinI2,OUTPUT);
 pinMode(speedpinA,OUTPUT);

 pinMode(pinI3,OUTPUT);//define this port as output
 pinMode(pinI4,OUTPUT);
 pinMode(speedpinB,OUTPUT);
 
 digitalWrite(pinI1,HIGH);
 digitalWrite(pinI2,HIGH);
 analogWrite(speedpinA,0);
 digitalWrite(pinI3,HIGH);
 digitalWrite(pinI4,HIGH);
 analogWrite(speedpinB,0);

 
}

void avancer(int vitesse){
  if(vitesse <-100 || vitesse > 100 || vitesse == 0){
   digitalWrite(pinI1,HIGH);
   digitalWrite(pinI2,HIGH);
   digitalWrite(pinI3,HIGH);
   digitalWrite(pinI4,HIGH);
   return;
  }
  if(vitesse<0){
   //Marche arrière
   digitalWrite(pinI1,LOW);
   digitalWrite(pinI2,HIGH);
   digitalWrite(pinI3,HIGH);
   digitalWrite(pinI4,LOW);
  }
  else{
   //Marche avant
   digitalWrite(pinI1,HIGH);
   digitalWrite(pinI2,LOW);
   digitalWrite(pinI3,LOW);
   digitalWrite(pinI4,HIGH);
  }
  analogWrite(speedpinA,abs(vitesse)*2.55);
  analogWrite(speedpinB,abs(vitesse)*2.55);
}

void tourner(int angle, int vitesse = 50){
  if(vitesse <-100 || vitesse > 100 || vitesse == 0){
   digitalWrite(pinI1,HIGH);
   digitalWrite(pinI2,HIGH);
   digitalWrite(pinI3,HIGH);
   digitalWrite(pinI4,HIGH);
   return;
  }
  if(angle<0){
   digitalWrite(pinI1,HIGH);
   digitalWrite(pinI2,LOW);
   digitalWrite(pinI3,HIGH);
   digitalWrite(pinI4,LOW);
  }
  else{
   digitalWrite(pinI1,LOW);
   digitalWrite(pinI2,HIGH);
   digitalWrite(pinI3,LOW);
   digitalWrite(pinI4,HIGH);
  }
  analogWrite(speedpinA,abs(vitesse)*2.55);
  analogWrite(speedpinB,abs(vitesse)*2.55);
}

void loop() {
  avancer(50);
  delay(2000);
  avancer(0);
  delay(10000);
  /*
  tourner(90);
  delay(1000);
  avancer(-50);
  delay(10000);
  tourner(-90);
  delay(1000);
  avancer(0);
  delay(20000);
  */
}

