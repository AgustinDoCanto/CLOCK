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
            case MEM:
                this->memory.mem(buffer[1], buffer[2], buffer[3]); 
              break;
            case ADD:
                this->memory.aritmethic_add(buffer[1], buffer[2], buffer[3]);
              break;
            case SUB:
                this->memory.aritmethic_sub(buffer[1], buffer[2], buffer[3]);
              break;
            case MUL:
                this->memory.aritmethic_mul(buffer[1], buffer[2], buffer[3]);
              break; 
            case DIV:
                this->memory.aritmethic_div(buffer[1], buffer[2], buffer[3]); 
              break;
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

bool Memory::pos_is_zero(uint8_t bank, uint8_t pos) {
  switch(bank){
    case A: return this->bank_A[pos] == 0; break;
    case B: return this->bank_B[pos] == 0; break;
    case SA: return this->bank_A[this->bank_A[pos]] == 0; break;
    case SB: return this->bank_B[this->bank_B[pos]] == 0; break;
  }
}

void Memory::mem(uint8_t fst_pos, uint8_t snd_pos, uint8_t register_pair) {
  switch(register_pair){
        case AA: this->bank_A[fst_pos] =  this->bank_A[snd_pos]; break;
        case AB: this->bank_A[fst_pos] =  (uint8_t)this->bank_B[snd_pos]; break;
        case BA: this->bank_B[fst_pos] =  (int)this->bank_A[snd_pos]; break;
        case BB: this->bank_B[fst_pos] =  this->bank_B[snd_pos];  break;
        case AC: this->bank_A[fst_pos] =  (uint8_t)snd_pos; break;
        case BC: this->bank_B[fst_pos] =  (int)snd_pos; break;
        case AN: this->bank_A[fst_pos] =  (uint8_t)(-snd_pos); break;
        case BN: this->bank_B[fst_pos] =  (int)(-snd_pos); break;
        case SASA: this->bank_A[this->bank_A[fst_pos]] =  this->bank_A[this->bank_A[snd_pos]]; break;
        case SASB: this->bank_A[this->bank_A[fst_pos]] =  (uint8_t)this->bank_B[this->bank_B[snd_pos]]; break;
        case SBSA: this->bank_B[this->bank_B[fst_pos]] =  (int)this->bank_A[this->bank_A[snd_pos]]; break;
        case SBSB: this->bank_B[this->bank_B[fst_pos]] =  this->bank_B[this->bank_B[snd_pos]]; break;
        case ASA: this->bank_A[fst_pos] =  this->bank_A[this->bank_A[snd_pos]]; break;
        case ASB: this->bank_A[fst_pos] =  (uint8_t)this->bank_B[this->bank_B[snd_pos]]; break;
        case BSA: this->bank_B[fst_pos] =  (int)this->bank_A[this->bank_A[snd_pos]]; break;
        case BSB: this->bank_B[fst_pos] =  this->bank_B[this->bank_B[snd_pos]];  break;
        case SAA: this->bank_A[this->bank_A[fst_pos]] =  this->bank_A[snd_pos]; break;
        case SAB: this->bank_A[this->bank_A[fst_pos]] =  (uint8_t)this->bank_B[snd_pos];  break;
        case SBA: this->bank_B[this->bank_B[fst_pos]] =  (int)this->bank_A[snd_pos]; break;
        case SBB: this->bank_B[this->bank_B[fst_pos]] =  this->bank_B[snd_pos]; break;
  }
}
 
void Memory::aritmethic_sub(int fst_pos, int snd_pos, uint8_t register_pair) {
    switch(register_pair){
        case AA: this->bank_A[fst_pos] = this->bank_A[fst_pos] - this->bank_A[snd_pos]; break;
        case AB: this->bank_A[fst_pos] = this->bank_A[fst_pos] - (uint8_t)this->bank_B[snd_pos]; break;
        case BA: this->bank_B[fst_pos] =  this->bank_B[fst_pos] - (int)this->bank_A[snd_pos]; break;
        case BB: this->bank_B[fst_pos] =  this->bank_B[fst_pos] - this->bank_B[snd_pos];  break;
        case AC: this->bank_A[fst_pos] =  this->bank_A[fst_pos] - (uint8_t)snd_pos; break;
        case BC: this->bank_B[fst_pos] =  this->bank_B[fst_pos] - (int)snd_pos; break;
        case AN: this->bank_A[fst_pos] =  this->bank_A[fst_pos] - (uint8_t)(-snd_pos); break;
        case BN: this->bank_B[fst_pos] =  this->bank_B[fst_pos] - (int)(-snd_pos); break;
    }
  }
  
void Memory::aritmethic_add(int fst_pos, int snd_pos, uint8_t register_pair) {
      switch(register_pair){
        case AA: this->bank_A[fst_pos] += this->bank_A[snd_pos]; break;
        case AB: this->bank_A[fst_pos] += (uint8_t)this->bank_B[snd_pos]; break;
        case BA: this->bank_B[fst_pos] += (int)this->bank_A[snd_pos]; break;
        case BB: this->bank_B[fst_pos] += this->bank_B[snd_pos];  break;
        case AC: this->bank_A[fst_pos] += (uint8_t)snd_pos; break;
        case BC: this->bank_B[fst_pos] += (int)snd_pos; break;
        case AN: this->bank_A[fst_pos] += (uint8_t)(-snd_pos); break;
        case BN: this->bank_B[fst_pos] += (int)(-snd_pos); break;
        case SASA: this->bank_A[this->bank_A[fst_pos]] +=  this->bank_A[this->bank_A[snd_pos]]; break;
        case SASB: this->bank_A[this->bank_A[fst_pos]] +=  (uint8_t)this->bank_B[this->bank_B[snd_pos]]; break;
        case SBSA: this->bank_B[this->bank_B[fst_pos]] +=  (int)this->bank_A[this->bank_A[snd_pos]]; break;
        case SBSB: this->bank_B[this->bank_B[fst_pos]] +=  this->bank_B[this->bank_B[snd_pos]]; break;
        case ASA: this->bank_A[fst_pos] +=  this->bank_A[this->bank_A[snd_pos]]; break;
        case ASB: this->bank_A[fst_pos] +=  (uint8_t)this->bank_B[this->bank_B[snd_pos]]; break;
        case BSA: this->bank_B[fst_pos] +=  (int)this->bank_A[this->bank_A[snd_pos]]; break;
        case BSB: this->bank_B[fst_pos] +=  this->bank_B[this->bank_B[snd_pos]];  break;
        case SAA: this->bank_A[this->bank_A[fst_pos]] +=  this->bank_A[snd_pos]; break;
        case SAB: this->bank_A[this->bank_A[fst_pos]] +=  (uint8_t)this->bank_B[snd_pos];  break;
        case SBA: this->bank_B[this->bank_B[fst_pos]] +=  (int)this->bank_A[snd_pos]; break;
        case SBB: this->bank_B[this->bank_B[fst_pos]] +=  this->bank_B[snd_pos]; break;
    }
  }

void Memory::aritmethic_mul(int fst_pos, int snd_pos, uint8_t register_pair) {
      switch(register_pair){
        case AA: this->bank_A[fst_pos] = this->bank_A[fst_pos] * this->bank_A[snd_pos]; break;
        case AB: this->bank_A[fst_pos] = this->bank_A[fst_pos] * (uint8_t)this->bank_B[snd_pos]; break;
        case BA: this->bank_B[fst_pos] =  this->bank_B[fst_pos] * (int)this->bank_A[snd_pos]; break;
        case BB: this->bank_B[fst_pos] =  this->bank_B[fst_pos] * this->bank_B[snd_pos];  break;
        case AC: this->bank_A[fst_pos] =  this->bank_A[fst_pos] * (uint8_t)snd_pos; break;
        case BC: this->bank_B[fst_pos] =  this->bank_B[fst_pos] * (int)snd_pos; break;
        case AN: this->bank_A[fst_pos] =  this->bank_A[fst_pos] * (uint8_t)(-snd_pos); break;
        case BN: this->bank_B[fst_pos] =  this->bank_B[fst_pos] * (int)(-snd_pos); break;
    }
  }
void Memory::aritmethic_div(int fst_pos, int snd_pos, uint8_t register_pair) {
      switch(register_pair){
        case AA: this->bank_A[fst_pos] = this->bank_A[fst_pos]  / this->bank_A[snd_pos]; break;
        case AB: this->bank_A[fst_pos] = this->bank_A[fst_pos] / (uint8_t)this->bank_B[snd_pos]; break;
        case BA: this->bank_B[fst_pos] =  this->bank_B[fst_pos] / (int)this->bank_A[snd_pos]; break;
        case BB: this->bank_B[fst_pos] =  this->bank_B[fst_pos] / this->bank_B[snd_pos];  break;
        case AC: this->bank_A[fst_pos] =  this->bank_A[fst_pos] / (uint8_t)snd_pos; break;
        case BC: this->bank_B[fst_pos] =  this->bank_B[fst_pos] / (int)snd_pos; break;
        case AN: this->bank_A[fst_pos] =  this->bank_A[fst_pos] / (uint8_t)(-snd_pos); break;
        case BN: this->bank_B[fst_pos] =  this->bank_B[fst_pos] / (int)(-snd_pos); break;
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
