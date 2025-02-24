// Creado por Agustin Do Canto 2024
#include "DisplayManager.h"
#include "clockInterpreter.h"

// Declarations for SD module:
#define SD_CS 4

DisplayManager console;// Inicializo el Display Manager
ClockInterpreter interpreter;

String data = ""; // Aqui se almacenan todos los caracteres recibidos en el puerto Serial
bool data_received = false;

void setup() {
  // Setting the u8g display:
  console.init();
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  console.print("Loading SD module... ");
  console.updateText();

  delay(200);
  digitalWrite(SD_CS, LOW);
  bool wasInitialized = SD.begin(SD_CS);
  digitalWrite(SD_CS, HIGH);
  SPI.end();
  if (wasInitialized) {
    console.print("[OK]");
    console.updateText();
    delay(1000);
    console.clearScreen();
  } else {
    console.print("[FAIL]");
    console.updateText();
    while (1);
  }

  console.showLogo();
  console.clearScreen();
  console.updateText();
  
  console.print('>');
  console.updateText();
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
       case 8:
        console.delOneOnBuffer();
        console.updateText();
        data = data.substring(0, data.length() - 1);
       break;
  
       case '\n': 
        if (data.substring(0, 3).equals("RUN")) { 
          String filename = data.substring(4, data.length());        
          console.clearScreen();
          console.updateText();
          SPI.end();
          digitalWrite(OLED_CS, HIGH);
          digitalWrite(SD_CS, LOW);      
          File file = SD.open(filename.c_str(), FILE_READ);
          running_code = true;
          interpreter.run(file, console); // Comienza a interpretar el programa indicado
          data="";
          while(Serial.available() == 0) {} // Deja el programa en pantalla hasta que cualquier tecla sea presionada
          SPI.end();
          digitalWrite(OLED_CS, LOW);
          digitalWrite(SD_CS, HIGH);
          console.clearScreen();
          console.updateText();         
          running_code = false;
          console.print('>');
          console.updateText();
        }       
       break;
  
       default:
         if(c != '\t' && c != '\r' && c != '\n'){data+=c;}
         console.print(c);
         console.updateText();
      }
    }
}
