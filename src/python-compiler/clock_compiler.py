import sys
import os

opcode_map = {
    "MEM": 0x01,
    "ADD": 0x02,
    "SUB": 0x03,
    "MUL": 0x04,
    "DIV": 0x05,
    "SYS": 0x06,
    "JNZ": 0x08
}

register_map = {
    "A": 0x01,
    "B": 0x02,
    "C": 0x03,
    "N": 0x04
}

register_pairs_map = {
    "AA": 0x01,
    "AB": 0x02,
    "BA": 0x03,
    "BB": 0x04,
    "AC": 0x05,
    "BC": 0x06,
    "AN": 0x07,
    "BN": 0x08
}

sys_opcode_map = {
    "PRT": 0x01,
    "INP": 0x02
}

def split_line(line):
    line = line.strip()
    if not line:  # Retornar una lista vacía si la línea está vacía
        return []
    splited_line = line.upper().replace(",", "").replace(";", "").split()
    return splited_line

def ensamblar_codigo(lines):
    binario = bytearray()
    labels = {}  # Diccionario de etiquetas con sus posiciones

    for index, line in enumerate(lines):
        parts = split_line(line)

        if parts != []:
            # LABELS handling
            if parts[0].endswith(":"):
                etiqueta = parts[0][:-1]  # Eliminar los ":"
                labels[etiqueta] = len(binario) // 4  # Guardar la linea a la que salta
                continue

            # OPERATION handling
            if parts[0] in opcode_map:
                operation_code = opcode_map[parts[0]]  # Get the opcode
                
                # Manejo de instrucciones MEM
                if parts[0] in ["MEM", "ADD", "SUB", "MUL", "DIV"]:
                    bank1 = (parts[1][:1], int(parts[1][2:])) # Pair (BANK A or B, Value)
                    bank2 = (parts[2][:1], int(parts[2][2:]))  # Pair (BANK A, B or C, Value)
                    instruction_line = [operation_code, bank1[1], bank2[1], register_pairs_map[f"{bank1[0]}{bank2[0]}"]]
                    print(f"{len(binario) // 4}: {instruction_line}") 
                    binario.extend(instruction_line)  # Agregar 4 bytes

                if parts[0] == "SYS":
                    option = sys_opcode_map[parts[1]]
                    bank = (parts[2][:1], int(parts[2][2:])) # Pair (BANK A or B, Value)
                    instruction_line = [operation_code, option, bank[1], register_map[f"{bank[0]}"]]
                    print(f"{len(binario) // 4}: {instruction_line}") 
                    binario.extend(instruction_line)

                elif parts[0] in ["JNZ"]:  # Manejo de saltos
                    position_to_jump = labels[parts[2]]
                    bank = (parts[1][:1], int(parts[1][2:]))
                    instruction_line = [operation_code, bank[1], position_to_jump, register_map[f"{bank[0]}"]] 
                    print(f"{len(binario) // 4}: {instruction_line}")
                    binario.extend(instruction_line)  # Agregar 4 bytes

            else:
                print(f"Unknown instruction at position {index}")

    return binario


if __name__ == '__main__':
    FILE_PATH = sys.argv[1]
    FILE_NAME = os.path.splitext(os.path.basename(FILE_PATH))[0] # Obtiene el nombre base y le quita la extension
    try:
        # Leer código CLOCK y compilar a binario
        with open(f"{FILE_PATH}", "r") as f:
            lines = f.readlines()

        codigo_binario = ensamblar_codigo(lines)

        print("-"*22+"\n")
        print(codigo_binario)
        print("\n"+"-"*22+"\n")
        print(f"Final size: {len(codigo_binario)} bytes")

        # Guardar en binario
        with open(f"{FILE_NAME}.rck", "wb") as f:
            f.write(codigo_binario)

    except Exception as e:
        print(e) 
