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
 digitalWrite(pinI1,HIGH);// DC motor rotates clockwise
 digitalWrite(pinI2,LOW);
 analogWrite(speedpinA,50);
 digitalWrite(pinI3,HIGH);// DC motor rotates clockwise
 digitalWrite(pinI4,LOW);
 analogWrite(speedpinB,50);
}
void loop()
{
 delay(2000);
 
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
