// CLOCK - Language and Terminal for Microcontrollers
// Copyright (C) 2024 Agustin Do Canto
#include "clockInterpreter.h"

// Declarations for SD module:
#define SD_CS 4

ClockInterpreter interpreter;

String data = ""; // Aqui se almacenan todos los caracteres recibidos en el puerto Serial
bool data_received = false;

void setup() {  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.println("Loading SD module... ");

  delay(200);
  digitalWrite(SD_CS, LOW);
  bool wasInitialized = SD.begin(SD_CS);
  digitalWrite(SD_CS, HIGH);
  SPI.end();
  if (wasInitialized) {
    Serial.println("[OK]");
    Serial.println("DoC Serial");
    delay(1000);
  } else {
    Serial.println("[FAIL]");
    while (1);
  }
  
  Serial.print("> ");
}

void loop() {
  if(Serial.available() > 0){
      char c = Serial.read();
      run_cli(c);
   }
}

bool running_code = false;
void run_cli(char c) {
  data_received = true;
  if(!running_code){
      switch(c){
       case DELETE_CODE:
        data = data.substring(0, data.length() - 1);
       break;
  
       case '\n': 
        if (data.substring(0, 3).equals("RUN")) { 
          Serial.println();
          String filename = data.substring(4, data.length());         
          File file = SD.open(filename.c_str(), FILE_READ);
          running_code = true;
          interpreter.run(file); // Comienza a interpretar el programa indicado
          data="";
          while(Serial.available() == 0) {} // Deja el programa en pantalla hasta que cualquier tecla sea presionada  
          running_code = false;
          Serial.print("\n> ");
        }       
       break;
  
       default:
         if(c != '\t' && c != '\r' && c != ENTER_CODE){data+=c;}
         Serial.print(c);
      }
    }
}
