// CLOCK - Language and Terminal for Microcontrollers
// Copyright (C) 2024 Agustin Do Canto
#ifndef CLOCKINTERPRETER_H
#define CLOCKINTERPRETER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdint>  // Para tipos como uint32_t
#include <iomanip>  // Para std::hex, etc.


#define BANK_A_LENGTH 64
#define BANK_B_LENGTH 32
#define DELETE_CODE 127
#define ENTER_CODE 13


// Tabla de conversion codigos de operacion   
enum Opcode {
    MEM = 0x01,
    ADD = 0x02,
    SUB = 0x03,
    MUL = 0x04,
    DIV = 0x05,
    SYS = 0x06,
    JNZ = 0x08,
    JEZ = 0x09
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
    PRT = 0x01,  // Imprime un valor en ASCII en el buffer de salida
    INP = 0x02,  // Recibe un caracter en ASCII desde el teclado/entrada
    VAL = 0x03,  // Imprime un valor NUMERICO en el buffer de salida
    FPO = 0x04,  // Open a file
    WRT = 0x05,  // Write in a file (ASCII)
    WRB = 0x06,  // Write in a file (BIN)
    SEK = 0x07,  // Moves file pointer = file.seek()
    RAD = 0x08,  // Read a byte of file (ASCII)
    RAB = 0x09,  // Read a byte of file (BIN)
    FPC = 0x0A,  // Close a file
    DIG = 0x0B,  // Escribe el valor de la entrada (en digital) desde un registro
    ANG = 0x0C,  // Escribe el valor de la entrada (en analogico) dentro de un registro
    IND = 0x0D,  // Lee el valor de la entrada (en digital) dentro de un registro
    ING = 0x0E,  // Lee el valor de la entrada (en analogico) dentro de un registro
    TIM = 0x0F,  // Guarda el valor de millis() dentro de un registro
    SLP = 0x10,  // Simula delay() en milliseconds
    SFA = 0x11,  // 1 if Still file available 0 if not = file.available() > 0
    FZE = 0x12,  // Put file.size() in a B bank
    CLS = 0x13   // Limpia la pantalla (solo para salidas soportadas)
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
    void run(std::string file_path);
};

// Funciones auxiliares
void handle_SYS_function(uint8_t buffer[4], Memory &memory, std::fstream& file);
void handle_JNZ_function(uint8_t buffer[4], Memory &memory, std::istream& file);
void handle_JEZ_function(uint8_t buffer[4], Memory &memory, std::istream& file);
void SYS_print(uint8_t buffer[4], Memory &memory);
void SYS_input(uint8_t buffer[4], Memory &memory);

// Funciones de Manejo de archivos
void SYS_file_open(uint8_t buffer[4], Memory &memory, std::fstream& file); 
void SYS_file_write(uint8_t buffer[4], Memory &memory, std::fstream& file, bool isASCII); 
void SYS_file_seek(uint8_t buffer[4], Memory &memory, std::fstream& file); 
void SYS_file_read(uint8_t buffer[4], Memory &memory, std::fstream& file); 
void SYS_still_file_available(uint8_t buffer[4], Memory &memory, std::fstream& file);
void SYS_file_size(uint8_t buffer[4], Memory &memory, std::fstream& file);
#endif
