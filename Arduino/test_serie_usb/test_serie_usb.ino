#define INPUT_SIZE 10

// Get next command from Serial (add 1 for final 0)
char input[INPUT_SIZE + 1];
byte size;
boolean command = false;

void serialEvent() {
  if(Serial.available()>0){
    Serial.readBytes(input,5);
    Serial.println(input);
    command = true;
  } 
}


void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if(command == true){
    Serial.println(input[0]);
    Serial.print(input[1]);
    Serial.print(input[2]);
    Serial.print(input[3]);
    Serial.println(input[4]);
    command = false;
  }
}

