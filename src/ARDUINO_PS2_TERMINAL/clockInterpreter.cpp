// Creado por Agustin Do Canto 2024
#include "clockInterpreter.h"
#define SD_CS 4

#if DEBUG == 1
Endl endl;
#endif

ClockInterpreter::ClockInterpreter(){}

void ClockInterpreter::run(File &file, DisplayManager &displayManager, PS2Keyboard &keyboard){
    SD.begin(SD_CS);
    uint8_t buffer[4];  // Buffer para almacenar 4 bytes
    File opened_file;
    #if DEBUG == 1
    debugln("---- Executing file ----");
    #endif
    
    file.seek(0);
    while (file.available()) {
        uint8_t bytesLeidos = file.read(buffer, 4);  // Leer hasta 4 bytes
        switch(buffer[0]){
            case MEM: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break;
            case ADD: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break;
            case SUB: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break;
            case MUL: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break; 
            case DIV: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break;
            case SYS:
                handle_SYS_function(buffer, this->memory, displayManager, keyboard, opened_file);
              break;
            case JNZ:
                handle_JNZ_function(buffer, this->memory, file);
              break;
          }
    }
    file.close();
    
    #if DEBUG == 1
    debugln("\nFinal memory: ");
    for(uint8_t i = 0; i < BANK_A_LENGTH; i++){
      if(memory.bank_A[i] != 0) debugln(i << " A: "<< memory.bank_A[i]);
    }
    
    for(uint8_t i = 0; i < BANK_B_LENGTH; i++){
      if(memory.bank_B[i] != 0) debugln(i << " B: "<< memory.bank_B[i]);
    }
    #endif
}

Memory::Memory(){
  for(uint8_t i=0; i < BANK_A_LENGTH; i++) this->bank_A[i] = 0;
  for(uint8_t i=0; i < BANK_B_LENGTH; i++) this->bank_B[i] = 0;
}

bool Memory::pos_is_zero(uint8_t bank, uint8_t pos) {
  switch(bank){
    case A: return this->bank_A[pos] == 0; break;
    case B: return this->bank_B[pos] == 0; break;
    case SA: return this->bank_A[this->bank_A[pos]] == 0; break;
    case SB: return this->bank_B[this->bank_B[pos]] == 0; break;
  }
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
    }
}

void handle_SYS_function(uint8_t buffer[4], Memory &memory, DisplayManager &console, PS2Keyboard &keyboard, File &file) {
  switch(buffer[1]){
    case PRT:
        SYS_print(buffer, memory, console); // Imprime el valor del banco como ASCII
      break;
    case INP:
        SYS_input(buffer, memory, keyboard); // Ingresa el valor en ASCII en el banco
     break;
    case VAL:
        SYS_print(buffer, memory, console); // Imprime el valor del banco
      break;
    case FPO:
        SYS_file_open(buffer, memory, file); 
      break;
    case WRT:
        SYS_file_write(buffer, memory, file, true);  // Escribe el banco en modo ASCII en el archivo 
      break;
    case WRB:
        SYS_file_write(buffer, memory, file, false); // Escribe el banco en modo BINARIO en el archivo 
      break;
    case SEK:
        SYS_file_seek(buffer, memory,file); // Cambia el puntero de donde lee el archivo
      break;
    case RAD:
        SYS_file_read(buffer, memory, file); // Lee un byte del archivo y lo guarda en un banco en formato ASCII
      break;
    case RAB:
        SYS_file_read(buffer, memory, file);  // Lee un byte del archivo y lo guarda en un banco en formato BINARIO
      break;
    case FPC:
        SYS_file_close(file); // Cierra el archivo
      break;
    case SFA:
        SYS_still_file_available(buffer, memory, file); // Guarda un 1 en el banco si todavia queda contenido y 0 si no
      break;
    case DIG:
      break;
    case ANG:
      break; 
    case IND:
      break;
    case ING:
      break;
    case TIM:
      break;
    case SLP:
    break; 
    }
  }

void handle_JNZ_function(uint8_t buffer[4], Memory &memory, File &file) {
   if (!memory.pos_is_zero(buffer[3], buffer[1])) { // JNZ <value> <file position to jump> <bank>
      file.seek(buffer[2] * 4);  // Usa buffer[2] directamente para el salto
   }
}

void SYS_print(uint8_t buffer[4], Memory &memory, DisplayManager &console) {
    uint8_t value = 0;
    
    // Obtener el valor según el banco
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
    
    if (buffer[1] != VAL && (char)value == PS2_DELETE) { // Si es un carácter de borrado, eliminar y actualizar
        console.delOneOnBuffer();
        console.updateText();
        return;
    }

    if (buffer[1] == PRT) { // Imprimir el valor según el modo
        console.print((char)value);
        console.updateText();
    } else if (buffer[1] == VAL) {
        console.printv(value);
        console.updateText();
    }
    console.updateText();
}

void SYS_input(uint8_t buffer[4], Memory &memory, PS2Keyboard &keyboard){
  while(!keyboard.available()){} // Espera a que el teclado este disponible
  char received_character = keyboard.read();
  switch(buffer[3]){ // SYS INP <pos> <bank>
    case A:
      memory.bank_A[buffer[2]] = (int)received_character; 
      break;
    case B:
      memory.bank_B[buffer[2]] = (int)received_character;
      break;
    }
}

void SYS_file_open(uint8_t buffer[4], Memory &memory, File &file) { // 0: SYS 1: FPO 2: <VALUE> 3: <BANK>
    SYS_file_close(file);
    String filename = "";
    uint8_t end = 0;  
    switch (buffer[3]) {
        case A:
            end = buffer[2];
            while (memory.bank_A[end] != 0 && end < BANK_A_LENGTH) { filename += (char)memory.bank_A[end]; end++; } // Encuentra el final del nombre
            break;
        case B:
            end = buffer[2];
            while (memory.bank_B[end] != 0 && end < BANK_B_LENGTH) { filename += (char)memory.bank_B[end]; end++;  }
            break;
        case SA:
            end = memory.bank_A[buffer[2]];
            while (memory.bank_A[memory.bank_A[end]] != 0 && end < BANK_A_LENGTH) { filename += (char)memory.bank_A[memory.bank_A[end]]; end++; }
            break;
        case SB:
            end = memory.bank_B[buffer[2]];
            while (memory.bank_B[memory.bank_B[end]] != 0 && end < BANK_B_LENGTH) { filename += (char)memory.bank_B[memory.bank_B[end]]; end++; }
            break;
    }
    if (file){ file.close(); }
    file = SD.open(filename.c_str(), O_RDWR | O_CREAT);
}


void SYS_file_write(uint8_t buffer[4], Memory &memory, File &file, bool isASCII) { 
    uint8_t value = 0;
    if(!file) { return; }
    // Determinar el valor a escribir según el banco
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

    // Escribir en el archivo según el modo
     if (isASCII) {
      if (value >= 32 && value <= 126) {
          char c = value;
          file.print(c);
      } else {
          // Manejo de error o impresión de un carácter de reemplazo
          file.print('?');  // Carácter de reemplazo si value no es válido
      }
    } else {
        file.write(value); // Modo binario
    }
}

void SYS_file_seek(uint8_t buffer[4], Memory &memory, File &file) { // 0: SYS 1: SEK 2: <VALUE> 3: <BANK>
    int value = 0;

    // Determinar el valor a escribir según el banco
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

    file.seek(value);
  }

void SYS_file_read(uint8_t buffer[4], Memory &memory, File &file) { // 0: SYS 1: RAD 2: <VALUE> 3: <BANK>
    uint8_t value = file.read(); // Leer un byte del archivo
    
    if(!file) { return; }
    // Guardar el valor en el banco correspondiente
    switch (buffer[3]) {
        case A:
            memory.bank_A[buffer[2]] = value;
            break;
        case B:
            memory.bank_B[buffer[2]] = (int)value;
            break;
        case SA:
            memory.bank_A[memory.bank_A[buffer[2]]] = value;
            break;
        case SB:
            memory.bank_B[memory.bank_B[buffer[2]]] = (int)value;
            break;
    }
}


void SYS_file_close(File &file) { // 0: SYS 1: FPC 2: <VALUE> 3: <BANK>
  if(file){ 
    file.close();
    }
}

void SYS_still_file_available(uint8_t buffer[4], Memory &memory, File &file){ // 0: SYS 1: SFA 2: <VALUE> 3: <BANK>
   if (!file) {  // Verificar si el archivo está abierto
    return;  // Salir si el archivo no está abierto
  }
  switch(buffer[3]){
    case A:
        memory.bank_A[buffer[2]] = file.available() ? 1 : 0;
      break;
    case B:
        memory.bank_B[buffer[2]] = file.available() ? 1 : 0;
      break;
    case SA:
        memory.bank_A[memory.bank_A[buffer[2]]] = file.available() ? 1 : 0;
      break;
    case SB:
        memory.bank_B[memory.bank_B[buffer[2]]] = file.available() ? 1 : 0;
      break;
    }
}
