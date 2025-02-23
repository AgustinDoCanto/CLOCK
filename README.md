# CLOCK - Lenguaje de programación

# (Foto del proyecto)

# Indice

1. [Descripción del proyecto](#descripción-del-proyecto)
2. [Características del proyecto](#características-del-proyecto)
3. [Ventajas de utilizar CLOCK](#ventajas-de-utilizar-clock)
   - [Abstracción del microcontrolador al más bajo nivel](#abstracción-del-microcontrolador-al-más-bajo-nivel)
   - [Ampliación de los recursos del microcontrolador](#ampliación-de-los-recursos-del-microcontrolador)
   - [WORA - Write Once Run Anywhere](#wora---write-once-run-anywhere)
   - [Aprovechamiento de los recursos al máximo](#aprovechamiento-de-los-recursos-al-máximo)
   - [Bajo coste](#bajo-coste)
4. [Limitaciones de CLOCK](#limitaciones-de-clock)
   - [Uso restringido de la memoria RAM](#uso-restringido-de-la-memoria-ram)
   - [Set de instrucciones reducido](#set-de-instrucciones-reducido)
   - [Curva de aprendizaje](#curva-de-aprendizaje)
   - [Velocidad](#velocidad)
5. [Guía de uso](#guía-de-uso)
   - [Especificación de CLOCK](#especificación-de-clock)
   - [Bancos de memoria](#bancos-de-memoria)
     - [Banco A](#banco-a)
     - [Banco B](#banco-b)
     - [Banco C y N](#banco-c-y-n)
   - [Set de instrucciones](#set-de-instrucciones)
   - [Ejemplos de uso](#ejemplos-de-uso)
     - [Instrucción MEM](#instrucción-mem)
     - [Instrucción ADD](#instrucción-add)
     - [Instrucción SUB](#instrucción-sub)
     - [Instrucción MUL](#instrucción-mul)

## Descripción del proyecto

CLOCK es un lenguaje de programación similar a assembler orientado a microcontroladores.

Este surge a través de un proyecto personal con el objetivo de crear una computadora/terminal que pueda correr sobre recursos extremadamente limitados, como lo es arduino (ATMEGA328p) y extender las capacidades del mismo de manera sencilla y con bajos costes, aprovechando asi al maximo el poder de estos pequeños gigantes.

# Características del pryecto

En un futuro se incluirá el esquema de conexión de los componentes. En mi caso cree dos versiones de una "computadora" portatil, una con teclado integrado que envia caracteres por el puerto Serial y otra haciendo uso de la libreria PS2Keyboard permitiendo así la conexión de un teclado PS2.

## Ventajas de utilizar CLOCK

El código en CLOCK se "compila" y se ejecuta directamente sobre una tarjeta SD ampliando la memoria del Arduino a potencialmente el tamaño de la tarjeta SD a través de la interfaz SPI de un modulo SD. 

Con esto se logra pasar de 32KB (Flash) del Arduino a 2, 8, 16, o 32 GB (dependiendo de la tarjeta SD). Logrando asi separar el código de la plataforma y generando algo similar al bytecode de java, pudiendo correr el mismo código sobre cualquier otro microcontrolador siempre y cuando cuente con el interprete correspondiente (Algo similar a lo que ocurre con la JVM de java).

### Abstracción del microcontrolador al más "bajo nivel"

CLOCK es muy similar a assembler lo que permite operar el hardware con un set de instrucciones básico.

### Ampliación de los recursos del micontrolador 32KB (Kilobytes) => 32GB (Gigabytes)

Como el código en CLOCK es "compilado" y ejecutado sobre una tarjeta SD, el tamaño de almacenamiento aumenta significativamente lo que permite no solo almacenar más código, sino también ejecutar distintos programas sobre el mismo microcontrolador, sin tener que compilar sobre el IDE cada vez.

### WORA - Write Once Run Anywhere - Código multiplataforma

El código en CLOCK es "compilado" (traducido) mediante python a instrucciones de 4 bytes lo que permite mayor eficiencia de almacenamiento y mejor aprovechamiento de los recursos del microcontrolador. Esto supone la ventaja adicional que contando con el interprete correspondiente cargado sobre el microcontrolador el mismo programa podrá ser ejecutado sobre diversos microcontroladores sin necesidad de recompilar nuevamente para cada uno, logrando así código de bajo nivel multiplataforma.

### Aprovechamiento de los recursos al maximo

CLOCK utiliza dos 'bancos' de memoria acotados que se definen sobre el hardware existente por lo que los programas en CLOCK deberán ser planificados para explotar estos recursos al máximo.

### Bajo coste

Como el lenguaje está destinado a correr sobre microcontroladores el consume energético de los mismos es extremadamente bajo, lo que permite al usuario contar con una plataforma programable portatil que se ejecute con baterias u otras fuentes de energia como paneles solares, logrando asi total independecia. Además el coste de los módulos lectores de SD (si no vienen ya integrados en el microcontrolador) rondan los 3 USD por lo que esta ampliación no supone un impacto económico significativo.

## Limitaciones de CLOCK

Si bien CLOCK es una buena promesa cuenta con algunas limitaciones que podrian llevar al usuario/a sobre un camino sinuoso tales como:

### Uso restringido de la memoria RAM: 

CLOCK amplia la memoria de almacenamiento del arduino, lo que se traduce en almacenar mayores cantidades de código dentro del mismo. Pero el lenguaje no amplia la RAM disponible e incluso se maneja dentro de un rango disponible, lo que limita las capacidades de los programas. Para solucionar este problema se deberá ser ingenioso con el uso de los 'Bancos' de memoria para lograr el resultado esperado,

### Set de instrucciones reducido

El set de instrucciones de CLOCK podria resultar reducido para usuarios/as experimentados en el uso de microcontroladores, ya que el lenguaje aún no cuenta con manejo de pines, ni instrucciones de 'alto nivel' por lo que escribir código en CLOCK podria resultar una tarea desafiante para usuarios no experimentados. Sin embargo esto se compensa en no estar limitado por la cantidad de código que se puede escribir ya que como se mencionó anteriormente se amplia el almacenamiento del microcontrolador.

### Curva de aprendizaje

Si bien CLOCK fue diseñado con la intención de ser simple, el mismo también se basa en el lenguaje ensamblador por lo que no estar familiarizado con la utilización de un lenguaje de mnemonicos (como assembler) podria presentar dificultades.

### Velocidad

Al ser código binario la interpretación del código en CLOCK es suficientemente rápida para usuarios corrientes, pero no se recomienda el uso de este lenguaje en sistemas de tiempo real críticos, ya que la misma consume algo de tiempo y puede afectar los sistemas donde el tiempo es un factor clave.

# Guía de uso

CLOCK se diseño sobre el IDE de arduino por lo que para utilizarlo basta con subir el interprete correspondiente al microcontrolador.

También puede optar por modificar el código existente y crear su propio interprete siguiendo los estándares.

## Especificación de CLOCK

Por el momento CLOCK permite únicamente la impresión de carácteres ASCII

### Bancos de memoria

CLOCK cuenta con dos bancos de memoria principales A y B, que al inciar cualquier programa empiezan con el valor 0:

### Banco A

El banco opera sobre el tipo 'uint8_t' o enteros sin signo de 8 bits, lo que quiere decir que puede almacenar números desde el 0 al 255 y tiene un largo de 64. Para acceder a una posición de este banco se utiliza el prefijo AX seguido de dos números decimales. 

**Ejemplo:** 

AX00 => Indica la posición 0 del banco A (primera posición del banco)
AX63 => Indica la posición 63 del banco A (última posición del banco)

### Banco B

El banco opera sobre el tipo 'int' o enteros, lo que quiere decir que puede almacenar números en el rango de -32.768 al 32.767 y tiene un largo de 64. Para acceder a una posición de este banco se utiliza el prefijo BX seguido de dos números decimales. 

**Ejemplo:** 

BX00 => Indica la posición 0 del banco B (primera posición del banco)
BX31 => Indica la posición 31 del banco B (última posición del banco)

### 'Banco' C y N

Los 'bancos' de memoria C y N refieren a constantes positivas y negativas, y no a posiciones de memoria en si, a través de estos se pueden indicar cantidades constantes. Se utilizan mediante el prefijo CX y NX respectivamente seguido de dos decimales.

**Ejemplo:**

CX04 => Indica la cantidad positiva 4
NX04 => Indica la cantidad -4 (negativo)
NX99 => Indica la cantidad -99 (negativa)

**NOTA:** El uso de números negativos solo es admitido sobre el banco B ya que el banco A solo opera con positivos.

### Set de instrucciones

CLOCK cuenta con un set de instrucciones reducido, siendo el mismo:
 
| Instrucción | Funcionamiento |    Uso     |
|:-----------:|:--------------:|:----------:|
|  MEM        | Permite la asignación de valores a bancos de memoria | MEM < banco >, < banco o valor >; |
|  ADD        | Permite la adición (suma) entre dos bancos de memoria | ADD < banco >, < banco o valor >; |
|  SUB        | Permite la substracción (resta) entre dos bancos de memoria | SUB < banco >, < banco o valor >; |
|  MUL        | Permite la multiplicación entre dos bancos de memoria | MUL < banco >, < banco o valor >; |
|  DIV        | Permite la división entre dos bancos de memoria | DIV < banco >, < banco o valor >; |
|  SYS        | Es el encargado de todas las operaciones de entrada y salida del sistema | SYS < operacion >, < banco o valor >;
|  JNZ        | Salta hacia una etiqueta si el banco apuntado no es cero | JNZ < banco >, < etiqueta (label) >;
|  LABELS     | Define el nombre de una etiqueta marcando a dónde saltará la instrucción JNZ | < nombre de la etiqueta >: (nombre de la etiqueta seguido de dos puntos) |

### Ejemplos de uso:

#### Instrucción MEM

La instrucción MEM recibe dos bancos de memoria y asigna el valor del segundo al primero:

**Ejemplo 1:**
MEM AX00, CX10; =>  A la posición 0 del banco A le asigna el valor 10
MEM BX20, NX30; =>  A la posición 20 del banco B le asigna el valor -30

#### Instrucción ADD

La instrucción ADD recibe dos bancos de memoria y suma el valor del segundo banco al primero, pudiendo ser este un banco constante o un banco en si:

**Ejemplo 1:**
MEM AX10, CX10; => Asigna el valor 10 a AX10
MEM BX20, CX10; => Asigna el valor 10 a BX20
ADD AX10, BX20; => Suma el valor de BX20 (10) al valor de AX10 (también 10) y deja el resultado en AX10. Ahora AX10 tiene el valor de 10.

#### Instrucción SUB

La instrucción SUB recibe dos bancos de memoria y resta el valor del segundo banco al primero, pudiendo ser este un banco constante o un banco en si:

**Ejemplo 1:**
MEM AX10, CX10; => Asigna el valor 10 a AX10
MEM BX20, CX10; => Asigna el valor 10 a BX20
SUB AX10, BX20; => Resta el valor de BX20 (10) al valor de AX10 (también 10) y deja el resultado en AX10. Ahora AX10 tiene el valor de 0.

#### Instrucción MUL

La instrucción MUL recibe dos bancos de memoria y multiplica el valor del segundo banco al primero, pudiendo ser este un banco constante o un banco en si:

**Ejemplo 1:**
MEM AX10, CX10; => Asigna el valor 10 a AX10
MEM BX20, CX10; => Asigna el valor 10 a BX20
MUL AX10, BX20; => Multiplica el valor de BX20 (10) al valor de AX10 (también 10) y deja el resultado en AX10. Ahora AX10 tiene el valor de 100.

#### Instrucción DIV

La instrucción MUL recibe dos bancos de memoria y multiplica el valor del segundo banco al primero, pudiendo ser este un banco constante o un banco en si:

**Ejemplo 1:**
MEM AX10, CX10; => Asigna el valor 10 a AX10
MEM BX20, CX10; => Asigna el valor 10 a BX20
DIV AX10, BX20; => Multiplica el valor de BX20 (10) al valor de AX10 (también 10) y deja el resultado en AX10. Ahora AX10 tiene el valor de 1.

#### Declaración de LABELS (etiquetas)

Las etiquetas funcionan para marcar un punto a donde otra parte del programa saltará, para esto cualquier secuencia de caracteres ASCII seguida de ':' es considerada una etiqueta:

**Ejemplo 1:** 'Nombre_de_la_etiqueta:'

**Ejemplo 2:** 'Etiqueta:'

**Ejemplo 3:** 'LOOP:'

**Ejemplo 4:** 'E1:'

Estas tienen utilidad en conjunto con la instrucción JNZ. 

#### Instrucción JNZ

La instrucción JNZ recibe un banco de memoria y el nombre de una etiqueta y si el banco de memoria recibido no tiene el valor de 0 salta a la etiqueta:

**Ejemplo 1:**

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

Este programa imprime la letra y resta 1 al valor de AX10, como resultado imprimirá 10 veces la letra A y luego imprimirá 10 veces la letra B.

#### Instrucción SYS

La instrucción SYS es la encargada de manejar las operaciones del sistema, como entrada, salida, impresión de caracteres o entrada de caracteres. (En futuras versiones se incluirá el manejo de pines y archivos a través de SYS).

**Ejemplo 1:**
SYS PRT, CX65; => Imprime la constante 65 que en ASCII corresponde a la 'A' mayúscula, por lo tanto imprime 'A'.

**Ejemplo 2:**
SYS PRT, AX20; => Imprimirá el caracter correspondiente al valor de AX20 en ASCII. Por ejemplo si AX20 tuviese el valor 95 imprimiría una barra baja '_'.

**Ejemplo 3:**
SYS INP, BX10; => Espera a la entrada por teclado y codifica a su valor ASCII la tecla ingresada, por ejemplo si se apretara la 'V' BX10 almacenaria el valor 86.



