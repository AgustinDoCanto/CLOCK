// Creado por Agustin Do Canto 2024
#include "DisplayManager.h"
#include "clockInterpreter.h"

// Declarations for SD module:
#define SD_CS 4

// Declarations for PS2 keyboard
const int DataPin = 6;
const int IRQpin =  3;

DisplayManager console;// Inicializo el Display Manager
ClockInterpreter interpreter;

PS2Keyboard keyboard;

String data = ""; // Aqui se almacenan todos los caracteres recibidos en el puerto Serial
bool data_received = false;

void setup() {
  // Setting the u8g display:
  console.init();
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  keyboard.begin(DataPin, IRQpin);

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
   if(keyboard.available()){
      char c = keyboard.read();
      keyboardCLI(c);
    }
}

bool running_code = false;
void keyboardCLI(char c) {
  data_received = true;
  if(!running_code){
    switch(c){
       case PS2_DELETE: // Borrrar
        console.delOneOnBuffer();
        console.updateText();
        data = data.substring(0, data.length() - 1);
       break;
    
       case PS2_ENTER: // '\n'
        if (data.substring(0, 3).equals("RUN")) { 
          String filename = data.substring(4, data.length());
    
          console.clearScreen();
          console.updateText();
          SPI.end();
          digitalWrite(OLED_CS, HIGH);
          digitalWrite(SD_CS, LOW);      
          File file = SD.open(filename.c_str(), FILE_READ);
          running_code = true;
          interpreter.run(file, console, keyboard); // Comienza a interpretar el programa indicado
          data="";
          while(!keyboard.available()) {} // Deja el programa en pantalla hasta que cualquier tecla sea presionada
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
