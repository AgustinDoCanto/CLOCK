// Creado por Agustin Do Canto 2024
#ifndef CLOCKINTERPRETER_H
#define CLOCKINTERPRETER_H

#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include "DisplayManager.h"

#define BANK_A_LENGTH 64
#define BANK_B_LENGTH 32

// Cambiar el valor de debug a 0 para dejar de imprimir mensajes por serial
#define DEBUG 0

#if DEBUG == 1

// Sobrecarga del operador << para Serial
template <typename T>
inline Print& operator<<(Print& serial, T value) {
    serial.print(value);
    return serial;
}

// Declarar endl como extern
struct Endl {};
extern Endl endl;
inline Print& operator<<(Print& serial, Endl) {
    serial.println();
    return serial;
}

#define debug(x) Serial<<x
#define debugln(x) Serial<<x<<endl

#else 

#define debug(x)
#define debugln(x) 

#endif

// Tabla de conversion codigos de operacion   
enum Opcode {
    MEM = 0x01,
    ADD = 0x02,
    SUB = 0x03,
    MUL = 0x04,
    DIV = 0x05,
    SYS = 0x06,
    JNZ = 0x08
};

enum RegisterMap {
    A = 0x01,
    B = 0x02,
    C = 0x03,
    N = 0x04
};

enum RegisterPair {
    AA = 0x01,
    AB = 0x02,
    BA = 0x03,
    BB = 0x04,
    AC = 0x05,
    BC = 0x06,
    AN = 0x07,
    BN = 0x08
};

enum SysOpcode {
    PRT = 0x01,
    INP = 0x02
};

class Memory {
  public:
    Memory();

    uint8_t bank_A[BANK_A_LENGTH];
    int bank_B[BANK_B_LENGTH];
         
    bool pos_is_zero(uint8_t register_map, uint8_t pos);
    void set_position_value(uint8_t register_map, uint8_t pos, int value);
    int get_position_value(uint8_t register_map, uint8_t pos);
    
    void mem(uint8_t fst_val, uint8_t snd_val, uint8_t register_pair);
    void aritmethic_sub(int fst_val, int snd_val, uint8_t register_pair);
    void aritmethic_add(int fst_val, int snd_val, uint8_t register_pair);
    void aritmethic_mul(int fst_val, int snd_val, uint8_t register_pair);
    void aritmethic_div(int fst_val, int snd_val, uint8_t register_pair);
};

class ClockInterpreter {
  public:
    ClockInterpreter();
    Memory memory;
    void run(File &filename, DisplayManager &displayManager); // Objeto Serial disponible de manera global
};

// Funciones auxiliares
void handle_SYS_function(uint8_t buffer[4], Memory &memory, DisplayManager &console);
void handle_JNZ_function(uint8_t buffer[4], Memory &memory, File &file);
void SYS_print(uint8_t buffer[4], Memory &memory, DisplayManager &console);
void SYS_input(uint8_t buffer[4], Memory &memory); // Objeto Serial disponible de manera global
#endif
