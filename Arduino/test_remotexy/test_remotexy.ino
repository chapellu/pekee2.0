/* 
   -- New project -- 
    
   This source code of graphical user interface  
   has been generated automatically by RemoteXY editor. 
   To compile this code using RemoteXY library 2.3.3 or later version  
   download by link http://remotexy.com/en/library/ 
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                    
     - for ANDROID 4.1.1 or later version; 
     - for iOS 1.2.1 or later version; 
     
   This source code is free software; you can redistribute it and/or 
   modify it under the terms of the GNU Lesser General Public 
   License as published by the Free Software Foundation; either 
   version 2.1 of the License, or (at your option) any later version.     
*/ 

////////////////////////////////////////////// 
//        RemoteXY include library          // 
////////////////////////////////////////////// 

// RemoteXY select connection mode and include library  
#define REMOTEXY_MODE__SOFTSERIAL
#include <SoftwareSerial.h> 

#include <RemoteXY.h> 

// RemoteXY connection settings  
#define REMOTEXY_SERIAL_RX 2 
#define REMOTEXY_SERIAL_TX 3 
#define REMOTEXY_SERIAL_SPEED 9600 


// RemoteXY configurate   
#pragma pack(push, 1) 
uint8_t RemoteXY_CONF[] = 
  { 255,4,0,0,0,43,0,8,13,0,
  1,1,43,18,12,12,2,31,88,0,
  1,1,29,27,12,12,2,31,88,0,
  1,1,43,36,12,12,2,31,88,0,
  1,1,57,27,12,12,2,31,88,0 }; 
   
// this structure defines all the variables of your control interface  
struct { 

    // input variable
  uint8_t button_1; // =1 if button pressed, else =0 
  uint8_t button_2; // =1 if button pressed, else =0 
  uint8_t button_3; // =1 if button pressed, else =0 
  uint8_t button_4; // =1 if button pressed, else =0 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY; 
#pragma pack(pop) 

///////////////////////////////////////////// 
//           END RemoteXY include          // 
///////////////////////////////////////////// 

#define PIN_BUTTON_1 13


void setup()  
{ 
  RemoteXY_Init ();  
   
  pinMode (PIN_BUTTON_1, OUTPUT);
   
  // TODO you setup code 
   
} 

void loop()  
{  
  RemoteXY_Handler (); 
   
  digitalWrite(PIN_BUTTON_1, (RemoteXY.button_1==0)?LOW:HIGH);
   
  // TODO you loop code 
  // use the RemoteXY structure for data transfer 


}
