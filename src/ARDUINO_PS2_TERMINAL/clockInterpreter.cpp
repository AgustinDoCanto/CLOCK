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
    
    #if DEBUG == 1
    debugln("---- Executing file ----");
    #endif
    
    file.seek(0);
    while (file.available()) {
        int bytesLeidos = file.read(buffer, 4);  // Leer hasta 4 bytes
        switch(buffer[0]){
            case MEM: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break;
            case ADD: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break;
            case SUB: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break;
            case MUL: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break; 
            case DIV: this->memory.bank_op(buffer[1], buffer[2], buffer[3], buffer[0]); break;
            case SYS:
                handle_SYS_function(buffer, this->memory, displayManager, keyboard);
              break;
            case JNZ:
                handle_JNZ_function(buffer, this->memory, file);
              break;
          }
    }
    file.close();
    
    #if DEBUG == 1
    debugln("\nFinal memory: ");
    for(int i = 0; i < BANK_A_LENGTH; i++){
      if(memory.bank_A[i] != 0) debugln(i << " A: "<< memory.bank_A[i]);
    }
    
    for(int i = 0; i < BANK_B_LENGTH; i++){
      if(memory.bank_B[i] != 0) debugln(i << " B: "<< memory.bank_B[i]);
    }
    #endif
}

Memory::Memory(){
  for(int i=0; i < 32; i++) this->bank_A[i] = 0;
  for(int i=0; i < 64; i++) this->bank_B[i] = 0;
}

void Memory::clear_memory(){
  for(int i=0; i < 32; i++) this->bank_A[i] = 0;
  for(int i=0; i < 64; i++) this->bank_B[i] = 0;
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

void handle_SYS_function(uint8_t buffer[4], Memory &memory, DisplayManager &console, PS2Keyboard &keyboard) {
  switch(buffer[1]){
    case PRT:
        SYS_print(buffer, memory, console);
      break;
    case INP:
        SYS_input(buffer, memory, keyboard);
     break;
    }
  }

void handle_JNZ_function(uint8_t buffer[4], Memory &memory, File &file) {
   if (!memory.pos_is_zero(buffer[3], buffer[1])) { // JNZ <value> <file position to jump> <bank>
      file.seek(buffer[2] * 4);  // Usa buffer[2] directamente para el salto
   }
}

void SYS_print(uint8_t buffer[4], Memory &memory, DisplayManager &console) {
  switch(buffer[3]){ // SYS PRT <value> <bank>
    case A:
      if((char)memory.bank_A[buffer[2]] == PS2_DELETE){
        console.delOneOnBuffer();
        console.updateText();
        return;
      }
      console.print((char)memory.bank_A[buffer[2]]);
      break;
    case B:
      if((char)memory.bank_B[buffer[2]] == PS2_DELETE){
        console.delOneOnBuffer();
        console.updateText();
        return;
      }
      console.print((char)memory.bank_B[buffer[2]]);
      break;
    case C:
      if((char)buffer[2] == PS2_DELETE){
        console.delOneOnBuffer();
        console.updateText();
        return;
      }
      console.print((char)buffer[2]);
      break;
    case N:
      if((char)-buffer[2] == PS2_DELETE){
        console.delOneOnBuffer();
        console.updateText();
        return;
      }
      console.print((char)-buffer[2]);
      break;
    case SA:
      if((char)memory.bank_A[memory.bank_A[buffer[2]]] == PS2_DELETE){
        console.delOneOnBuffer();
        console.updateText();
        return;
      }
      console.print((char)memory.bank_A[memory.bank_A[buffer[2]]]);
      break;
    case SB:
      if((char)memory.bank_B[memory.bank_B[buffer[2]]] == PS2_DELETE){
        console.delOneOnBuffer();
        console.updateText();
        return;
      }
      console.print((char)memory.bank_B[memory.bank_B[buffer[2]]]);
      break;
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
