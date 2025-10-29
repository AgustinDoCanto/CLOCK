// CLOCK - Language and Terminal for Microcontrollers
// Copyright (C) 2024 Agustin Do Canto
#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "Arduino.h"

#include <SPI.h>
#include "U8glib.h"

// Declarations for OLED Display:
#define OLED_CLK 13  
#define OLED_MOSI 11
#define OLED_RESET 9
#define OLED_DC 8
#define OLED_CS 7

// Basado en la fuente u8g_font_6x10 para la consola => 6 filas x 21 caracteres

class DisplayManager{
  private:
    // Configuraciones
    String textBuffer; 
  public:
    DisplayManager();
    void init();
    void showLogo();
    void clearScreen();
    void updateText(); // Imprime el contenido de text buffer borrando la pantalla
    void print(const String &text);
    void print(char c);
    void printv(int value);
    void delOneOnBuffer();
  };


#endif
