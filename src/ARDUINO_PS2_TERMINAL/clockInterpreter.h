#ifndef CLOCKINTERPRETER_H
#define CLOCKINTERPRETER_H

#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <PS2Keyboard.h>
#include "DisplayManager.h"
#include <stdint.h>

#define BANK_A_LENGTH 64
#define BANK_B_LENGTH 32
#define MAX_FILENAME_LENGTH 20

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
    N = 0x04,
    SA = 0x05,
    SB = 0x06
};

enum RegisterPair {
    AA = 0x01,
    AB = 0x02,
    BA = 0x03,
    BB = 0x04,
    AC = 0x05,
    BC = 0x06,
    AN = 0x07,
    BN = 0x08,
    SASA = 0x09,
    SASB = 0x0A,
    SBSA = 0x0B,
    SBSB = 0x0C,
    ASA = 0x0D,
    ASB = 0x0E,
    BSA = 0x0F,
    BSB = 0x10,
    SAA = 0x11,
    SAB = 0x12,
    SBA = 0x13,
    SBB = 0x14,
    SAC = 0x15,
    SBC = 0x16,
    SAN = 0x17,
    SBN = 0x18
};

enum SysOpcode {
    PRT = 0x01,
    INP = 0x02,
    VAL = 0x03,
    FPO = 0x04,  // Open a file
    WRT = 0x05,  // Write in a file (ASCII)
    WRB = 0x06,  // Write in a file (BIN)
    SEK = 0x07,  // Moves file pointer = file.seek()
    RAD = 0x08,  // Read a byte of file (ASCII)
    RAB = 0x09,  // Read a byte of file (BIN)
    FPC = 0x0A,  // Close a file
    DIG = 0x0B, 
    ANG = 0x0C,
    IND = 0x0D,
    ING = 0x0E,
    TIM = 0x0F,
    SLP = 0x10,
    SFA = 0x11,  // 1 if Still file available 0 if not = file.available() > 0
    FZE = 0x12,  // Put file.size() in a B bank
    CLS = 0x13
};

template <typename T1, typename T2>
void apply_operation(T1 &dst, const T2 &src, Opcode opcode);

class Memory {
  public:
    Memory();

    uint8_t bank_A[BANK_A_LENGTH];
    int32_t bank_B[BANK_B_LENGTH];  // Cambiado de int a int32_t
    
    bool pos_is_zero(uint8_t register_map, uint8_t pos);
    void bank_op(uint8_t fst_pos, uint8_t snd_pos, uint8_t register_pair, Opcode opcode); // Engloba las operaciones MEM, ADD, MUL, SUB y DIV
};

class ClockInterpreter {
  public:
    ClockInterpreter();
    Memory memory;
    void run(File &filename, DisplayManager &displayManager, PS2Keyboard &keyboard);
};

// Funciones auxiliares
void handle_SYS_function(uint8_t buffer[4], Memory &memory, DisplayManager &console, PS2Keyboard &keyboard, File &file);
void handle_JNZ_function(uint8_t buffer[4], Memory &memory, File &file);
void SYS_print(uint8_t buffer[4], Memory &memory, DisplayManager &console);
void SYS_input(uint8_t buffer[4], Memory &memory, PS2Keyboard &keyboard);

// Funciones de Manejo de archivos
void SYS_file_open(uint8_t buffer[4], Memory &memory, File &file); 
void SYS_file_write(uint8_t buffer[4], Memory &memory, File &file, bool isASCII); 
void SYS_file_seek(uint8_t buffer[4], Memory &memory, File &file); 
void SYS_file_read(uint8_t buffer[4], Memory &memory, File &file); 
void SYS_still_file_available(uint8_t buffer[4], Memory &memory, File &file);
void SYS_file_size(uint8_t buffer[4], Memory &memory, File &file);

#endif
