// Creado por Agustin Do Canto 2024
#include "DisplayManager.h"
#define SD_CS 4


U8GLIB_SH1106_128X64 u8g(OLED_CLK, OLED_MOSI, OLED_CS, OLED_DC, OLED_RESET);

DisplayManager::DisplayManager(){
  this->textBuffer = "";
  }

void DisplayManager::init(){
  // Setting the u8g display:
  if (u8g.getMode() == U8G_MODE_R3G3B2) {
    u8g.setColorIndex(255);
  } else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
    u8g.setColorIndex(3);
  } else if (u8g.getMode() == U8G_MODE_BW) {
    u8g.setColorIndex(1);
  } else if (u8g.getMode() == U8G_MODE_HICOLOR) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
  }

void DisplayManager::clearScreen(){
  this->textBuffer = "";
  u8g.setFont(u8g_font_6x10);
  u8g.firstPage();
  do {
  } while (u8g.nextPage());
  }

void DisplayManager::showLogo(){
  digitalWrite(OLED_CS, LOW);
  delay(1000);
  u8g.setFont(u8g_font_ncenR18);
  u8g.firstPage();
  do {
    u8g.drawStr(38, 38, "DoC");
   } while (u8g.nextPage());
   delay(1000);
   digitalWrite(OLED_CS, HIGH);
   SPI.end();
  }

void DisplayManager::updateText(){
    digitalWrite(OLED_CS, LOW);
    digitalWrite(SD_CS, HIGH);
    u8g.setFont(u8g_font_6x10);
    u8g.firstPage();
    do {
        byte longitudCadena = this->textBuffer.length();
        byte inicio = 0;
        byte largoSubcadena = 21; // Depende de cuantos caracteres entren en el display, en este caso entran 21
        byte cantSubcadenas = 1;
       
        while (inicio < longitudCadena) {
          byte longitudSubcadena = min(largoSubcadena, longitudCadena - inicio);
          u8g.drawStr(0, 10*cantSubcadenas, this->textBuffer.substring(inicio, inicio + longitudSubcadena).c_str());
          inicio += longitudSubcadena;
          cantSubcadenas++;
        }
     } while (u8g.nextPage());
    SPI.end();
    digitalWrite(OLED_CS, HIGH);
  }

void DisplayManager::print(const String &text){
  if ((this->textBuffer.length() + text.length()) <= 126) {// Si no excede la longitud máxima, agrega el texto a la cadena 
    this->textBuffer += text;
  }else{
    this->textBuffer = "";
    this->textBuffer += text;
    }
  }

void DisplayManager::printv(int value){
  String aux = "";
  aux = value;
  if ((this->textBuffer.length() + aux.length()) <= 126) {// Si no excede la longitud máxima, agrega el texto a la cadena 
    this->textBuffer += aux;
  }else{
    this->textBuffer = "";
    this->textBuffer += aux;
    }
  }

void DisplayManager::print(char c){
  if ((this->textBuffer.length() + 1) <= 126) { // Si no excede la longitud máxima, agrega el texto a la cadena
    this->textBuffer += c;
  }else{
    this->textBuffer = "";
    this->textBuffer += c;
    }
}

void DisplayManager::delOneOnBuffer(){
  this->textBuffer = this->textBuffer.substring(0, this->textBuffer.length() - 1); 
}
  

  
