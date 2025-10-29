// CLOCK - Language and Terminal for Microcontrollers
// Copyright (C) 2024 Agustin Do Canto
#include "clockInterpreter.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdint>  // Para tipos como uint32_t
#include <iomanip>  // Para std::hex, etc.

using namespace std;

ClockInterpreter::ClockInterpreter(){}

void ClockInterpreter::run(std::string file_path) {
    uint8_t buffer[4];  // Buffer para almacenar 4 bytes
    fstream opened_file;

    // Abro el archivo para lectura binaria
    fstream current_file(file_path, std::ios::in | std::ios::binary);
    if (!current_file.is_open()) {
        cout << "[ERR]: Couldn't open file " << file_path << endl;
        return;
    }

    while (current_file.read(reinterpret_cast<char*>(buffer), 4)) {
        switch (buffer[0]) {
            case MEM: this->memory.bank_op(buffer[1], buffer[2], buffer[3], static_cast<Opcode>(buffer[0])); break;
            case ADD: this->memory.bank_op(buffer[1], buffer[2], buffer[3], static_cast<Opcode>(buffer[0])); break;
            case SUB: this->memory.bank_op(buffer[1], buffer[2], buffer[3], static_cast<Opcode>(buffer[0])); break;
            case MUL: this->memory.bank_op(buffer[1], buffer[2], buffer[3], static_cast<Opcode>(buffer[0])); break;
            case DIV: this->memory.bank_op(buffer[1], buffer[2], buffer[3], static_cast<Opcode>(buffer[0])); break;
            case SYS:
                handle_SYS_function(buffer, this->memory, opened_file);
                break;
            case JNZ:
                handle_JNZ_function(buffer, this->memory, current_file);
                break;
            case JEZ:
                handle_JEZ_function(buffer, this->memory, current_file);
                break;
        }
    }

    current_file.close(); // Cierro el archivo
}

Memory::Memory(){
  for(uint8_t i=0; i < BANK_A_LENGTH; i++) this->bank_A[i] = 0;
  for(uint8_t i=0; i < BANK_B_LENGTH; i++) this->bank_B[i] = 0;
}

bool Memory::pos_is_zero(uint8_t bank, uint8_t pos) {
  switch(bank){
    case A: return this->bank_A[pos] == 0; break;
    case B: return this->bank_B[pos] == 0; break;
    case C: return pos == 0;
    case SA: return this->bank_A[this->bank_A[pos]] == 0; break;
    case SB: return this->bank_B[this->bank_B[pos]] == 0; break;
  }
  return false;
}


template <typename T1, typename T2>
void apply_operation(T1 &dst, const T2 &src, Opcode opcode) {
    if (opcode == MEM) {
        dst = src; // Si la operación es MEM, solo asigna
    } else {
        switch (opcode) {
            case ADD: dst += src; break;
            case SUB: dst -= src; break;
            case MUL: dst *= src; break;
            case DIV: dst /= src; break; 
        }
    }
}

void Memory::bank_op(uint8_t fst_pos, uint8_t snd_pos, uint8_t register_pair, Opcode opcode) {
    switch (register_pair) {
        case AA: apply_operation(this->bank_A[fst_pos], this->bank_A[snd_pos], opcode); break;
        case AB: apply_operation(this->bank_A[fst_pos], this->bank_B[snd_pos], opcode); break;
        case BA: apply_operation(this->bank_B[fst_pos], this->bank_A[snd_pos], opcode); break;
        case BB: apply_operation(this->bank_B[fst_pos], this->bank_B[snd_pos], opcode); break;
        case AC: apply_operation(this->bank_A[fst_pos], snd_pos, opcode); break;
        case BC: apply_operation(this->bank_B[fst_pos], snd_pos, opcode); break;
        case AN: apply_operation(this->bank_A[fst_pos], -snd_pos, opcode); break;
        case BN: apply_operation(this->bank_B[fst_pos], -snd_pos, opcode); break;
        case SASA: apply_operation(this->bank_A[this->bank_A[fst_pos]], this->bank_A[this->bank_A[snd_pos]], opcode); break;
        case SASB: apply_operation(this->bank_A[this->bank_A[fst_pos]], this->bank_B[this->bank_B[snd_pos]], opcode); break;
        case SBSA: apply_operation(this->bank_B[this->bank_B[fst_pos]], this->bank_A[this->bank_A[snd_pos]], opcode); break;
        case SBSB: apply_operation(this->bank_B[this->bank_B[fst_pos]], this->bank_B[this->bank_B[snd_pos]], opcode); break;
        case ASA: apply_operation(this->bank_A[fst_pos], this->bank_A[this->bank_A[snd_pos]], opcode); break;
        case ASB: apply_operation(this->bank_A[fst_pos], this->bank_B[this->bank_B[snd_pos]], opcode); break;
        case BSA: apply_operation(this->bank_B[fst_pos], this->bank_A[this->bank_A[snd_pos]], opcode); break;
        case BSB: apply_operation(this->bank_B[fst_pos], this->bank_B[this->bank_B[snd_pos]], opcode); break;
        case SAA: apply_operation(this->bank_A[this->bank_A[fst_pos]], this->bank_A[snd_pos], opcode); break;
        case SAB: apply_operation(this->bank_A[this->bank_A[fst_pos]], this->bank_B[snd_pos], opcode); break;
        case SBA: apply_operation(this->bank_B[this->bank_B[fst_pos]], this->bank_A[snd_pos], opcode); break;
        case SBB: apply_operation(this->bank_B[this->bank_B[fst_pos]], this->bank_B[snd_pos], opcode); break;
        case SAC: apply_operation(this->bank_A[this->bank_A[fst_pos]], snd_pos, opcode); break;
        case SBC: apply_operation(this->bank_B[this->bank_B[fst_pos]], snd_pos, opcode); break;
        case SAN: apply_operation(this->bank_A[this->bank_A[fst_pos]], -snd_pos, opcode); break;
        case SBN: apply_operation(this->bank_B[this->bank_B[fst_pos]], -snd_pos, opcode); break;
    }
}

void handle_SYS_function(uint8_t buffer[4], Memory& memory, std::fstream& file) {
    switch (buffer[1]) {
        case PRT:
            SYS_print(buffer, memory); // Imprime el valor como ASCII
            break;
        case INP:
            SYS_input(buffer, memory); // Ingresa valor ASCII al banco
            break;
        case VAL:
            SYS_print(buffer, memory); // Imprime valor numérico del banco
            break;
        case FPO:
            SYS_file_open(buffer, memory, file);
            break;
        case WRT:
            SYS_file_write(buffer, memory, file, true);  // ASCII
            break;
        case WRB:
            SYS_file_write(buffer, memory, file, false); // BINARIO
            break;
        case SEK:
            SYS_file_seek(buffer, memory, file);
            break;
        case RAD:
            SYS_file_read(buffer, memory, file);  // ASCII o binario, depende de implementación interna
            break;
        case RAB:
            SYS_file_read(buffer, memory, file);
            break;
        case FPC:
            if (file.is_open()) file.close();  // Cierra archivo si está abierto
            break;
        case SFA:
            SYS_still_file_available(buffer, memory, file);
            break;
        case FZE:
            SYS_file_size(buffer, memory, file);
            break;
        case CLS:
            // No hacer nada
            break;
    }
}

void handle_JNZ_function(uint8_t buffer[4], Memory& memory, std::istream& file) {
    if (!memory.pos_is_zero(buffer[3], buffer[1])) {  // JNZ <value> <file position to jump> <bank>
        file.clear(); // Limpiar errores antes de usar seek
        file.seekg(buffer[2] * 4, std::ios::beg);
    }
}

void handle_JEZ_function(uint8_t buffer[4], Memory& memory, std::istream& file) {
    if (memory.pos_is_zero(buffer[3], buffer[1])) {
        file.clear(); // Limpiar errores antes de usar seek
        file.seekg(buffer[2] * 4, std::ios::beg);
    }
}

void SYS_print(uint8_t buffer[4], Memory &memory) {
    switch (buffer[3]) { // 0: SYS 1: PRT/VAL 2: <value> 3: <bank>
        case A:
            if (buffer[1] == PRT && std::isprint(memory.bank_A[buffer[2]])) {
                std::cout << static_cast<char>(memory.bank_A[buffer[2]]);
            } else if (buffer[1] == VAL) {
                std::cout << static_cast<int>(memory.bank_A[buffer[2]]);
            }
            break;
        case B:
            if (buffer[1] == PRT && std::isprint(memory.bank_B[buffer[2]])) {
                std::cout << static_cast<char>(memory.bank_B[buffer[2]]);
            } else if (buffer[1] == VAL) {
                std::cout << memory.bank_B[buffer[2]];
            }
            break;
        case C:
            if (buffer[1] == PRT && std::isprint(buffer[2])) {
                std::cout << static_cast<char>(buffer[2]);
            } else if (buffer[1] == VAL) {
                std::cout << static_cast<int>(buffer[2]);
            }
            break;
        case SA:
            if (buffer[1] == PRT && std::isprint(memory.bank_A[memory.bank_A[buffer[2]]])) {
                std::cout << static_cast<char>(memory.bank_A[memory.bank_A[buffer[2]]]);
            } else if (buffer[1] == VAL) {
                std::cout << static_cast<int>(memory.bank_A[memory.bank_A[buffer[2]]]);
            }
            break;
        case SB:
            if (buffer[1] == PRT && std::isprint(memory.bank_B[memory.bank_B[buffer[2]]])) {
                std::cout << static_cast<char>(memory.bank_B[memory.bank_B[buffer[2]]]);
            } else if (buffer[1] == VAL) {
                std::cout << memory.bank_B[memory.bank_B[buffer[2]]];
            }
            break;
    }
}


void SYS_input(uint8_t buffer[4], Memory& memory) {
    char received_character;
    std::cin.get(received_character); // Lee un carácter desde entrada estándar (bloqueante)

    switch (buffer[3]) { // SYS INP <pos> <bank>
        case A:
            memory.bank_A[buffer[2]] = static_cast<uint8_t>(received_character);
            break;
        case B:
            memory.bank_B[buffer[2]] = static_cast<int32_t>(received_character);
            break;
        case SA:
            memory.bank_A[memory.bank_A[buffer[2]]] = static_cast<uint8_t>(received_character);
            break;
        case SB:
            memory.bank_B[memory.bank_B[buffer[2]]] = static_cast<int32_t>(received_character);
            break;
    }
}


void SYS_file_open(uint8_t buffer[4], Memory &memory, std::fstream &file) { // 0: SYS 1: FPO 2: <VALUE> 3: <BANK>
    if (file.is_open()) {
        file.close();
    }

    std::string filename = "";
    uint8_t end = 0;

    switch (buffer[3]) {
        case A:
            end = buffer[2];
            while (memory.bank_A[end] != 0 && end < BANK_A_LENGTH) {
                filename += static_cast<char>(memory.bank_A[end]);
                end++;
            }
            break;
        case B:
            end = buffer[2];
            while (memory.bank_B[end] != 0 && end < BANK_B_LENGTH) {
                filename += static_cast<char>(memory.bank_B[end]);
                end++;
            }
            break;
        case SA:
            end = memory.bank_A[buffer[2]];
            while (memory.bank_A[memory.bank_A[end]] != 0 && end < BANK_A_LENGTH) {
                filename += static_cast<char>(memory.bank_A[memory.bank_A[end]]);
                end++;
            }
            break;
        case SB:
            end = memory.bank_B[buffer[2]];
            while (memory.bank_B[memory.bank_B[end]] != 0 && end < BANK_B_LENGTH) {
                filename += static_cast<char>(memory.bank_B[memory.bank_B[end]]);
                end++;
            }
            break;
    }

    // Abre en modo lectura-escritura, crea si no existe, no trunca si ya existe
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        // Si no existe, intenta crear el archivo
        file.open(filename, std::ios::out | std::ios::binary);
        file.close();
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }
}


void SYS_file_write(uint8_t buffer[4], Memory &memory, std::fstream &file, bool isASCII) {
    if (!file.is_open()) return;

    uint8_t value = 0;

    // Obtener valor desde el banco correspondiente
    switch (buffer[3]) {
        case A:
            value = memory.bank_A[buffer[2]];
            break;
        case B:
            value = memory.bank_B[buffer[2]];
            break;
        case C:
            value = buffer[2];
            break;
        case SA:
            value = memory.bank_A[memory.bank_A[buffer[2]]];
            break;
        case SB:
            value = memory.bank_B[memory.bank_B[buffer[2]]];
            break;
    }

    // Escritura según modo
    if (isASCII) {
        if (value >= 32 && value <= 126) {
            file << static_cast<char>(value);
        } else {
            file << '?';  // Carácter de reemplazo
        }
    } else {
        file.write(reinterpret_cast<const char*>(&value), sizeof(uint8_t));
    }

    file.flush();  // Asegura que se escriba inmediatamente
}


void SYS_file_seek(uint8_t buffer[4], Memory &memory, std::fstream &file) {  // 0: SYS 1: SEK 2: <VALUE> 3: <BANK>
    if (!file.is_open()) return;

    std::streampos value = 0;

    // Obtener la posición deseada
    switch (buffer[3]) {
        case A:
            value = memory.bank_A[buffer[2]];
            break;
        case B:
            value = memory.bank_B[buffer[2]];
            break;
        case C:
            value = buffer[2];
            break;
        case SA:
            value = memory.bank_A[memory.bank_A[buffer[2]]];
            break;
        case SB:
            value = memory.bank_B[memory.bank_B[buffer[2]]];
            break;
    }

    // Mover ambos punteros de lectura y escritura
    file.seekg(value, std::ios::beg);
    file.seekp(value, std::ios::beg);
}


void SYS_file_read(uint8_t buffer[4], Memory &memory, std::fstream &file) { // 0: SYS 1: RAD 2: <VALUE> 3: <BANK>

    if (!file.is_open() || file.eof()) return;

    char raw_value;
    file.read(&raw_value, 1); // Leer un byte del archivo
    uint8_t value = static_cast<uint8_t>(raw_value);

    // Guardar el valor en el banco correspondiente
    switch (buffer[3]) {
        case A:
            memory.bank_A[buffer[2]] = value;
            break;
        case B:
            memory.bank_B[buffer[2]] = static_cast<int32_t>(value);
            break;
        case SA:
            memory.bank_A[memory.bank_A[buffer[2]]] = value;
            break;
        case SB:
            memory.bank_B[memory.bank_B[buffer[2]]] = static_cast<int32_t>(value);
            break;
    }
}


void SYS_still_file_available(uint8_t buffer[4], Memory &memory, std::fstream &file) { // 0: SYS 1: SFA 2: <VALUE> 3: <BANK>
    if (!file.is_open()) {
        return;  // Salir si el archivo no está abierto
    }

    // Determinar si hay datos disponibles para leer
    bool available = !file.eof();

    switch(buffer[3]) {
        case A:
            memory.bank_A[buffer[2]] = available ? 1 : 0;
            break;
        case B:
            memory.bank_B[buffer[2]] = available ? 1 : 0;
            break;
        case SA:
            memory.bank_A[memory.bank_A[buffer[2]]] = available ? 1 : 0;
            break;
        case SB:
            memory.bank_B[memory.bank_B[buffer[2]]] = available ? 1 : 0;
            break;
    }
}

void SYS_file_size(uint8_t buffer[4], Memory &memory, std::fstream &file) {
    if (!file.is_open()) {
        return;  // Salir si el archivo no está abierto
    }

    // Guardar la posición actual
    std::streampos current_pos = file.tellg();

    // Ir al final para obtener el tamaño
    file.seekg(0, std::ios::end);
    std::streampos size = file.tellg();

    // Volver a la posición original
    file.seekg(current_pos, std::ios::beg);

    // Asignar tamaño al banco B
    switch (buffer[3]) {
        case B:
            memory.bank_B[buffer[2]] = static_cast<int32_t>(size);
            break;
        case SB:
            memory.bank_B[memory.bank_B[buffer[2]]] = static_cast<int32_t>(size);
            break;
    }
}
