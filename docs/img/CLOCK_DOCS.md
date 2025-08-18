# DOCUMENTACIÓN - CLOCK

## Set de instrucciones

CLOCK cuenta con un set de instrucciones reducido, siendo el mismo:
 
| Instrucción | Funcionamiento |    Uso     |
|:-----------:|:--------------:|:----------:|
|  MEM        | Memory Operation - Permite la asignación de valores a bancos de memoria | MEM < banco >, < banco o valor >; |
|  ADD        | Addition - Permite la adición (suma) entre dos bancos de memoria | ADD < banco >, < banco o valor >; |
|  SUB        | Subtraction - Permite la substracción (resta) entre dos bancos de memoria | SUB < banco >, < banco o valor >; |
|  MUL        | Multiplication - Permite la multiplicación entre dos bancos de memoria | MUL < banco >, < banco o valor >; |
|  DIV        | Division -Permite la división entre dos bancos de memoria | DIV < banco >, < banco o valor >; |
|  SYS        | System Operation - Es el encargado de todas las operaciones de entrada y salida del sistema | SYS < operacion >, < banco o valor >;
|  JNZ        | Jump if Not Zero - Salta hacia una etiqueta si el banco apuntado no es cero | JNZ < banco >, < etiqueta (label) >;
|  JEZ        | Jump if Equal Zero - Salta hacia una etiqueta si el banco apuntado es cero | JNZ < banco >, < etiqueta (label) >;
|  LABELS     | Define el nombre de una etiqueta marcando a dónde saltará la instrucción JNZ o JEZ | < nombre de la etiqueta >: (nombre de la etiqueta seguido de dos puntos) |

### Operaciones del Sistema (SYS):

Debido a que la memoria del microcontrolador es limitada, solo se proveen algunas operaciones del sistema escenciales tales como apertura y manejo de archivos, impresión en pantalla y entrada de caracteres. De todas formas el set puede ampliarse o modificarse en caso de prescindir de alguna de alguna de las mismas o necesitar alguna operacion adicional como manejo de pines.


> **NOTA:** Si lo que busca es ampliar el set de instrucciones del sistema se debe tener en cuenta que deberá modificar el "compilador" y el "interprete" para que los códigos de compilación y ejecución coincidan. 
>
> Los dispositivos que no soporten ciertas funcionalidades como la limpieza de display, simplemente no ejecutaran esas funciones. Por lo que pueden omitirse en el "intérprete" y "compilador". 




| Suboperación (SYS) | Funcionamiento |    Uso     |
|:-----------:|:--------------:|:----------:|
|  PRT        | Imprime un valor en ASCII en el dispositivo de salida | SYS PRT, < banco o valor >; |
| INP  | Recibe un caracter desde el teclado/entrada y coloca su valor en ASCII en el banco indicado | SYS INP, < banco >;    |
| VAL  | Imprime el valor NUMERICO del banco indicado en el dispositivo de salida |  SYS PRT, < banco o valor >; |
| FPO  | File Pointer Open - Abre el archivo con el nombre iniciando donde indica el banco | SYS FPO, < banco o valor >; |
| WRT  | Write in a file (ASCII) - Escribe en el archivo abierto el correspondiente en ASCII del valor del banco indicado | SYS WRT, < banco o valor >; |
| WRB  | Write in a file (BIN) - Escribe en el archivo abierto el valor del banco indicado en Binario | SYS WRB, < banco o valor >; |
| SEK  | Mueve el puntero de lectura del archivo abierto (similar a file.seek() de Arduino) | SYS SEK, < banco o valor >; |
| RAD  | Read a byte of file (ASCII) - Lee un caracter (byte) del archivo abierto y guarda el valor correspondiente en ASCII en el banco indicado | SYS RAD, < banco >; |
| RAB  | Read a byte of file (BIN) - Lee un byte del archivo abierto y guarda el valor correspondiente en Binario en el banco indicado | SYS RAB, < banco >; |
| FPC  | File Pointer Close - Cierra el archivo actualmente abierto con FPO en caso del mismo | SYS FPC, < banco o valor >; |
| DIG  | *No Implementado* - Escribe el valor del banco (digital - 1 si tiene valor, 0 si no ) en el pin de salida con su valor correspondiente | SYS DIG, < banco o valor >; |
| ANG  | *No Implementado* - Escribe el valor del banco (Analógico - PWM) en el pin de salida con su valor correspondiente | SYS ANG, < banco o valor >; |
| IND  | *No implementado* - Lee el valor de la entrada (en digital) dentro de un registro desde el pin con la dirección del registro | SYS PRT, < banco o valor >; |
| ING  | *No implementado* - Lee el valor de la entrada (en analogico) dentro de un registro desde el pin con la dirección del registro | SYS PRT, < banco o valor >; |
| TIM  | *No implementado* - Time - Guarda el valor de millis() dentro del registro indicado | SYS TIM, < banco o valor >; |
| SLP  | *No implementado* - Sleep - Detiene la ejecución del programa por el valor en el banco indicado en milisegundos (similar a delay() de Arduino) | SYS SLP, < banco o valor >; |
| SFA  | Still File Available - Guarda 1 si todavía quedan bytes por leer en el archivo abierto con FPO y 0 si no ( Similar a file.available() de Arduino) | SYS SFA, < banco o valor >; |
| FZE  | File Size - Guarda el tamaño del archivo abierto con FPO en el banco indicado - **Solo soportado por el banco B** (Similara a file.size() de Arduino) | SYS PRT, < banco o valor >; |
| CLS  | Clear Screen - Limpia el display de salida si el dispositivo de salida lo permite | SYS PRT, < banco o valor >; |


## Bancos de memoria

Los bancos de memoria existentes son los siguientes:


| Banco/Registro | Nombre | Tipo del Valor | Uso |
|:--------------:|:------:|:--------------:|:------------:|
| A  |  Banco/Registro  A    | uint8_t | Tiene como finalidad la impresión de y guardado de carácteres |
| B  |  Banco/Registro  B    | int32_t | Tiene como finalidad la realización y guardado de operaciones aritméticas |
| $A  |  Banco/Registro indireccion A | uint8_t | Accede a la dirección del banco A guardada en el banco indicado |
| $B  |  Banco/Registro indireccion B | int32_t | Accede a la dirección del banco B guardada en el banco indicado |
| C  |  Banco/Registro Constante    | Tipo del banco destino | Se utiliza para ingresar u operar con valores constantes positivos |
| N  |  Banco/Registro Constantes Negativos | Tipo del banco destino | Se utiliza para ingresar u operar con valores constantes negativos  |

## Como escribir, "compilar" y ejecutar un archivo CLOCK

### Compilación

Para "compilar" y ejecutar un archivo en CLOCK se debe contar con el interprete grabado en la placa, el archivo .CLK escrito.

Luego parado sobre el directorio CLOCK_compiler corremos la siguiente linea.

```bash
python clock_compiler.py <ruta_al_archivo_clk>
```

>El mismo nos creará en el directorio donde estemos ejecutando el compilador, un archivo .RCK con el código compilado y listo para ser ejecutado por el intérprete.

## Ejecución

Para ejecutar el archivo compilado deberemos mover el archivo resultado de la compilación a la tarjeta SD que leerá el microcontrolador. 

> Este proceso se puede llevar a cabo con la cantidad de archivos que se desee permitiendo correr sobre una misma arquitectura de hardware, más de un programa y el mismo programa en cualquier arquitectura de hardware distinta pero con el intérprete correspondiente.

## Guía de uso

CLOCK se diseño sobre el IDE de arduino por lo que para utilizarlo basta con subir el código del interprete correspondiente al microcontrolador.

También puede optar por modificar el código existente y crear su propio intérprete siguiendo los estándares.Permitiendo así la ampliación del mismo y la inclusión de instrucciones no implementadas, contempladas o nuevas. 

### Bancos de memoria

CLOCK cuenta con dos bancos de memoria (registros) principales A y B, que al inciar cualquier programa comienzan inicializados con el valor 0:

### Banco A

El banco opera sobre el tipo 'uint8_t' o enteros sin signo de 8 bits, lo que quiere decir que puede almacenar números desde el 0 al 255 y tiene un largo máximo de 64. Para acceder a una posición de este banco se utiliza el prefijo AX seguido de dos números decimales. 

```
Ejemplo: 

AX00 => Indica la posición 0 del banco A (primera posición del banco)
AX63 => Indica la posición 63 del banco A (última posición del banco)
```

### Banco B

El banco opera sobre el tipo 'int' o enteros, lo que quiere decir que puede almacenar números en el rango de -32.768 al 32.767 y tiene un largo de 64. Para acceder a una posición de este banco se utiliza el prefijo BX seguido de dos números decimales. 

```
Ejemplo:

BX00 => Indica la posición 0 del banco B (primera posición del banco)
BX31 => Indica la posición 31 del banco B (última posición del banco)
```


### Banco C y N

Los 'bancos' de memoria C y N refieren a constantes positivas y negativas, y no a posiciones de memoria en si, a través de estos se pueden indicar cantidades constantes. Se utilizan mediante el prefijo CX y NX respectivamente seguido de dos decimales.

```
Ejemplo:

CX04 => Indica la cantidad positiva 4
NX04 => Indica la cantidad -4 (negativo)
NX99 => Indica la cantidad -99 (negativa)
```

> **NOTA:** El uso de números negativos solo es admitido sobre el banco B ya que el banco A solo opera con positivos.

### Ejemplos de uso:

#### Instrucción MEM

La instrucción MEM recibe dos bancos de memoria y asigna el valor del segundo al primero:

```
Ejemplo 1:

MEM AX00, CX10; =>  A la posición 0 del banco A le asigna el valor 10
MEM BX20, NX30; =>  A la posición 20 del banco B le asigna el valor -30
```

#### Instrucción ADD

La instrucción ADD recibe dos bancos de memoria y suma el valor del segundo banco al primero, pudiendo ser este un banco constante o un banco en si:

```
Ejemplo 1:

MEM AX10, CX10; => Asigna el valor 10 a AX10
MEM BX20, CX10; => Asigna el valor 10 a BX20
ADD AX10, BX20; => Suma el valor de BX20 (10) al valor de AX10 (también 10) y deja el resultado en AX10. Ahora AX10 tiene el valor de 10.
```


#### Instrucción SUB

La instrucción SUB recibe dos bancos de memoria y resta el valor del segundo banco al primero, pudiendo ser este un banco constante o un banco en si:


```
Ejemplo 1:

MEM AX10, CX10; => Asigna el valor 10 a AX10
MEM BX20, CX10; => Asigna el valor 10 a BX20
SUB AX10, BX20; => Resta el valor de BX20 (10) al valor de AX10 (también 10) y deja el resultado en AX10. Ahora AX10 tiene el valor de 0.
```


#### Instrucción MUL

La instrucción MUL recibe dos bancos de memoria y multiplica el valor del segundo banco al primero, pudiendo ser este un banco constante o un banco en si:


```
Ejemplo 1:

MEM AX10, CX10; => Asigna el valor 10 a AX10
MEM BX20, CX10; => Asigna el valor 10 a BX20
MUL AX10, BX20; => Multiplica el valor de BX20 (10) al valor de AX10 (también 10) y deja el resultado en AX10. Ahora AX10 tiene el valor de 100.
```

#### Instrucción DIV

La instrucción MUL recibe dos bancos de memoria y multiplica el valor del segundo banco al primero, pudiendo ser este un banco constante o un banco en si:

```
Ejemplo 1:

MEM AX10, CX10; => Asigna el valor 10 a AX10
MEM BX20, CX10; => Asigna el valor 10 a BX20
DIV AX10, BX20; => Multiplica el valor de BX20 (10) al valor de AX10 (también 10) y deja el resultado en AX10. Ahora AX10 tiene el valor de 1.
```


#### Declaración de LABELS (etiquetas)

Las etiquetas funcionan para marcar un punto a donde otra parte del programa saltará, para esto cualquier secuencia de caracteres ASCII seguida de ':' es considerada una etiqueta:

```
Ejemplo 1: 'Nombre_de_la_etiqueta:'
Ejemplo 2: 'Etiqueta:'
Ejemplo 3: 'LOOP:'
Ejemplo 4: 'E1:'
```

Estas tienen utilidad en conjunto con la instrucción JNZ. 

#### Instrucción JNZ

La instrucción JNZ recibe un banco de memoria y el nombre de una etiqueta y si el banco de memoria recibido no tiene el valor de 0 salta a la etiqueta:

```
Ejemplo 1:

MEM AX10, CX09; => Asigna el valor 10 a AX10
LOOP1:
SYS PRT, CX65; => Imprime la letra A
SUB AX10, CX01; => Resta 1 al valor actual en AX10 
JNZ AX10, LOOP1; => Salta a la etiqueta LOOP1 si el valor en AX10 no es 0

MEM AX10, CX09; => Asigna el valor 10 a AX10
LOOP2:
SYS PRT, CX66; => Imprime la letra B
SUB AX10, CX01; => Resta 1 al valor actual en AX10 
JNZ AX10, LOOP2; => Salta a la etiqueta LOOP2 si el valor en AX10 no es 0
```

Este programa imprime la letra y resta 1 al valor de AX10, como resultado imprimirá 10 veces la letra A y luego imprimirá 10 veces la letra B.

#### Instrucción SYS

La instrucción SYS es la encargada de manejar las operaciones del sistema, como entrada, salida, impresión de caracteres o entrada de caracteres. (En futuras versiones se incluirá el manejo de pines y archivos a través de SYS).


```
Ejemplo 1:

SYS PRT, CX65; => Imprime la constante 65 que en ASCII corresponde a la 'A' mayúscula, por lo tanto imprime 'A'.
```

```
Ejemplo 2:

SYS PRT, AX20; => Imprimirá el caracter correspondiente al valor de AX20 en ASCII. Por ejemplo si AX20 tuviese el valor 95 imprimiría una barra baja '_'.
```

```
Ejemplo 3:

SYS INP, BX10; => Espera a la entrada por teclado y codifica a su valor ASCII la tecla ingresada, por ejemplo si se apretara la 'V' BX10 almacenaria el valor 86.
```