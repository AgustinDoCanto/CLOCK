import sys
import os

opcode_map = {
    "MEM": 0x01, # Memory Handlig
    "ADD": 0x02, # Adition
    "SUB": 0x03, # Substraction
    "MUL": 0x04, # Multiplication
    "DIV": 0x05, # Division
    "SYS": 0x06, # System functions
    "JNZ": 0x08, # Jump if Not equal to Zero
    "JEZ": 0x09  # Jump if Equal to Zero
}

register_map = {
    "A": 0x01,
    "B": 0x02,
    "C": 0x03,
    "N": 0x04,
    "$A": 0x05,
    "$B": 0x06
}

register_pairs_map = {
    "AA": 0x01,
    "AB": 0x02,
    "BA": 0x03,
    "BB": 0x04,
    "AC": 0x05,
    "BC": 0x06,
    "AN": 0x07,
    "BN": 0x08,
    "$A$A": 0x09,
    "$A$B": 0x0A,
    "$B$A": 0x0B,
    "$B$B": 0x0C,
    "A$A": 0x0D,
    "A$B": 0x0E,
    "B$A": 0x0F,
    "B$B": 0x10,
    "$AA": 0x11,
    "$AB": 0x12,
    "$BA": 0x13,
    "$BB": 0x14,
    "$AC": 0x15,
    "$BC": 0x16,
    "$AN": 0x17, 
    "$BN": 0x18 
}

sys_opcode_map = { # System options
    "PRT": 0x01, # Prints a Character in ASCII
    "INP": 0x02, # Get a character ASCII value from input
    "VAL": 0x03, # Prints the numerical valuo of a bank
    "FPO": 0x04, # Open a file 
    "WRT": 0x05, # Write a character ASCII in a file
    "WRB": 0x06, # Write a bank in binary to a file in binary mode
    "SEK": 0x07, # Moves the file reading pointer to a value (ONLY WORKS WITH BANK B)
    "RAD": 0x08, # Read a byte from a file to a bank
    "RAB": 0x09, # Read a byte from a file to a bank in binary
    "FPC": 0x0A, # Closes the opened file
    "DIG": 0x0B,
    "ANG": 0x0C,
    "IND": 0x0D,
    "ING": 0x0E,
    "TIM": 0x0F,
    "SLP": 0x10,
    "SFA": 0x11, # 1 if Still File Available 0 if not
    "FZE": 0x12, # Saves the file size in a bank (ONLY WORKS WITH BANK B)
    "CLS": 0x13 # Clear the Screen
}

def split_line(line):
    line = line.strip()
    if not line:  # Retornar una lista vacía si la línea está vacía
        return []
    splited_line = line.upper().replace(",", "").replace(";", "").split()
    # for part in splited_line: print(part)
    return splited_line


def parse_file_for_labels(lines): # Recorre el archivo en busca de etiquetas y guarda su posicion en el binario
    labels = {}
    position_counter = 0
    for index, line in enumerate(lines):
        parts = split_line(line)
        
        if parts != []:
            if parts[0].endswith(":"):
                etiqueta = parts[0][:-1]  # Eliminar los ":"
                labels[etiqueta] = position_counter // 4  # Guardar la linea a la que salta
            else:
                position_counter += 4 # Cada instruccion ocupara 4 bytes
    return labels


def assembly_code(lines):
    binario = bytearray()
    labels = parse_file_for_labels(lines)  # Diccionario de etiquetas con sus posiciones

    for index, line in enumerate(lines):
        parts = split_line(line)

        if parts != []:
            # OPERATION handling
            if (parts[0] in opcode_map):
                operation_code = opcode_map[parts[0]]  # Get the opcode
                
                # Manejo de instrucciones MEM
                if parts[0] in ["MEM", "ADD", "SUB", "MUL", "DIV"]:
                    
                    if parts[1][:1] == '$':
                        bank1 = (parts[1][:2], int(parts[1][4:])) # Pair (BANK A or B, Value)
                    else:
                        bank1 = (parts[1][:1], int(parts[1][2:])) # Pair (BANK A or B, Value)
                    
                    if parts[2][:1] == '$':
                        bank2 = (parts[2][:2], int(parts[2][4:]))  # Pair (BANK A, B or C, Value)
                    else:
                        bank2 = (parts[2][:1], int(parts[2][2:]))  # Pair (BANK A, B or C, Value)

                    instruction_line = [operation_code, bank1[1], bank2[1], register_pairs_map[f"{bank1[0]}{bank2[0]}"]]
                    print(f"{len(binario) // 4}: {instruction_line}") 
                    binario.extend(instruction_line)  # Agregar 4 bytes

                if parts[0] == "SYS":
                    option = sys_opcode_map[parts[1]]
                    
                    if parts[2][:1] == '$':
                        bank = (parts[2][:2], int(parts[2][4:])) # Pair (BANK A or B, Value)
                    else:
                        bank = (parts[2][:1], int(parts[2][2:])) # Pair (BANK A or B, Value)
                                    
                    instruction_line = [operation_code, option, bank[1], register_map[f"{bank[0]}"]]
                    print(f"{len(binario) // 4}: {instruction_line}") 
                    binario.extend(instruction_line)

                elif parts[0] in ["JNZ", "JEZ"]:  # Manejo de saltos
                    position_to_jump = labels[parts[2]]
                    
                    if parts[1][:1] == '$':
                        bank = (parts[1][:2], int(parts[1][4:]))
                    else:
                        bank = (parts[1][:1], int(parts[1][2:]))

                    instruction_line = [operation_code, bank[1], position_to_jump, register_map[f"{bank[0]}"]] # JNZ/JEZ <value> <position> <bank>
                    print(f"{len(binario) // 4}: {instruction_line}")
                    binario.extend(instruction_line)  # Agregar 4 bytes
            else:
                if not parts[0].endswith(':'): print(f"Unknown instruction at position {index}: '{line}'")

    return binario


if __name__ == '__main__':
    FILE_PATH = sys.argv[1]
    FILE_NAME = os.path.splitext(os.path.basename(FILE_PATH))[0] # Obtiene el nombre base y le quita la extension

    # Leer código CLOCK y compilar a binario
    with open(f"{FILE_PATH}", "r") as f:
        lines = f.readlines()

    print(parse_file_for_labels(lines))
    codigo_binario = assembly_code(lines)
    
    print("-"*22+"\n")
    print(codigo_binario)
    print("\n"+"-"*22+"\n")
    print(f"Final size: {len(codigo_binario)} bytes")
    # Guardar en binario
    with open(f"{FILE_NAME}.rck", "wb") as f:
        f.write(codigo_binario)

