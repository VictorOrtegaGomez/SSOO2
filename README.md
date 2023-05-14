# SSOO2

## P1
Por un lado, el programa consta de un proceso principal manager encargado de crear y coordinar al
resto de procesos. Dichos procesos deben acceder al fichero estudiantes.txt, el cual consta de un
conjunto de líneas donde se especifica la siguiente información en cada una de ellas:


1. DNI del estudiante.
2. Modelo de examen que debe realizar.
3. Nota que obtuvo en el examen anterior.


El proceso manager debe crear tres tipos de procesos **PA**, **PB** y **PC**, cada uno de ellos con una labor
diferente.


En primer lugar, el proceso manager debe crear un proceso **PA** y esperar a que finalice. El proceso
**PA**, debe realizar una lectura del fichero estudiantes.txt para crear un directorio por cada estudiante.
El nombre del directorio debe ser el mismo que el DNI del estudiante. Cuando este proceso finalice
la creación de directorios debe notificárselo al proceso manager, con el objetivo de que pueda
continuar con su trabajo.


Una vez creado un directorio para cada estudiante, el proceso manager debe crear un proceso **PB** y
otro **PC** que deben trabajar de forma simultánea; el proceso manager debe esperar a la finalización
de ambos. El proceso **PB** debe hacer una lectura del fichero estudiantes.txt para extraer el modelo
de examen que debe realizar cada estudiante. Note como en el material aportado con la práctica, hay
un directorio llamado examenes, donde hay tres modelos de examen A.pdf, B.pdf y C.pdf. El
proceso **PB** debe copiar en cada directorio de estudiante el modelo que le corresponda. El modelo
que le corresponde a cada estudiante se indica en el fichero estudiantes.txt, en concreto, en el
segundo campo o valor de cada línea.


Por otro lado, un estudiante supera la prueba si la media aritmética del resultado obtenido en el
examen anterior, junto con el que va a obtener con la realización de alguno de los modelos de
examen, llega a la nota de corte de 5 (ambas pruebas tienen el mismo peso). La nota obtenida en el
último examen para cada estudiante también está registrada en el tercer campo del fichero
estudiantes.txt. El proceso manager debe crear un proceso **PC**, que trabaje de forma paralela al
proceso **PB**. El proceso **PC** debe acceder al fichero estudiantes.txt y determinar, por cada
estudiante, la nota que debe obtener en la nueva prueba para que la media finalmente alcance la nota
de corte. El proceso **PC** debe crear un fichero de texto con extensión .txt que contenga la siguiente
cadena: *«La nota que debes obtener en este nuevo examen para superar la prueba es X.»*, siendo X
la nota calculada. El fichero de texto debe estar almacenado en el directorio del estudiante. En
resumen, cada directorio de estudiante debe contener un modelo de examen y un archivo de texto
que le informe de la nota que debe obtener en el segundo examen, para que la media junto con la
primera nota obtenida alcance la nota de corte.


Además, el proceso manager debe mantener un archivo log.txt donde se registre la finalización de
cada una de las tareas y la media de las notas obtenidas por los estudiantes en la primera prueba.
Dicha media debe ser calculada por **PC** y enviada al proceso manager a través de una tubería o
pipe. Un ejemplo del contenido que debe tener el archivo log.txt puede ser el siguiente.


> ******** Log del sistema ********
>
> Creación de directorios finalizada.
>
> Copia de modelos de examen, finalizada.
>
> Creación de archivos con nota necesaria para alcanzar la nota de corte, finalizada.
>
> La nota media de la clase es: 7,25
>
> FIN DE PROGRAMA


**Finalización del programa:**


El programa finaliza si se producen alguna de las dos siguientes situaciones:


1. Los procesos **PB** y **PC**, finalizan de forma correcta.
2. El usuario pulsa CTRL+C durante la ejecución de alguno de los procesos, para interrumpir
la ejecución de forma voluntaria. En tal caso, el proceso manager debe crear un cuarto
proceso **PD** que se encargue de limpiar todo el contenido creado, es decir, debe eliminar
todos los directorios y archivos asociados a los estudiantes. Antes de eliminar los
contenidos, el proceso manager debe forzar la finalización de los procesos creados para
evitar que continúen realizando cambios. Por último, se debe registrar en log.txt que se ha
producido una interrupción voluntaria ordenada por el propio usuario.


Por último, la práctica debe incluir un proceso que trabaje en **segundo plano** (demonio) y que se
active **periódicamente** cada minuto. El proceso debe realizar una copia de seguridad con todos los
archivos relativos a la práctica en un directorio aparte. La finalización de este proceso puede
realizarse de forma manual mediante el uso de órdenes en el intérprete de coma

## P2

En la segunda práctica de la asignatura SSOO II se solicita el diseño y codificación del buscador
SSOOIIGLE, capaz de buscar información en un fichero de forma paralela con múltiples hilos. En
líneas generales, el usuario debe especificar al buscador el fichero donde buscar, la palabra que
desea buscar y el número de hilos que se deben emplear en la búsqueda. 

> SSOOIIGLE <nombre_fichero> <palabra> <numero_hilos> 
  
De esta forma, el fichero se «dividirá1» en tantas partes como hilos se indiquen. Por ejemplo,
imagine el caso en el que un fichero consta de 100 líneas. Si el usuario invocara al buscador de la
siguiente forma:
  
> SSOOIIGLE historia.txt castillo 2
  
El proceso principal creará dos hilos, el primero de ellos buscará la palabra castillo entre las líneas 1
y 50 del fichero historia.txt, mientras que el segundo hilo realizará la misma búsqueda de forma
paralela entre las líneas 51 y 100. 
  
  
Por otro lado, el buscador debe mostrar por pantalla la aparición de la palabra en el archivo con la
siguiente información:
- Fragmento en el que se ha encontrado la palabra. A su vez un fragmento se define mediante
un número de hilo, línea inicial y línea final.
- Línea del fichero en la que ha sido encontrada.
- Referencia a las dos palabras entre las que se encuentra la buscada: palabra anterior y
palabra posterior.
  
  
Un requisito importante es que el resultado de búsqueda se debe mostrar en orden. Es decir, no se
pueden mostrar los resultados de un fragmento posterior, aunque el hilo que haya realizado la
búsqueda en ese fragmento haya finalizado antes. Un ejemplo de salida podría ser el siguiente:
  
  
> [Hilo 1 inicio:0 – final: 25] :: línea 12 :: … el castillo antiguo …
>
> [Hilo 1 inicio:0 – final: 25] :: línea 20 :: … un castillo abandonado …
>
> [Hilo 3 inicio:51 – final: 75] :: línea 63 :: … el castillo y …
>
> [Hilo 3 inicio:51 – final: 75] :: línea 72 :: … sin castillo y …
>
> [Hilo 4 inicio:76 – final: 100] :: línea 92 :: … el castillo derrumbado …
  
 
El estudiante debe diseñar una estructura de datos donde almacenar las ocurrencias de una palabra 
de acuerdo al formato anterior. Esta estructura debe ser compartida por todos los hilos y se debe 
garantizar exclusión mutua en el acceso. Es decir, mientras un hilo acceda a la estructura para 
escribir una nueva ocurrencia, el resto debe esperar.
  
## P3
  
Esta práctica pretende ser una continuación/ampliación de la práctica anterior. Si en la práctica dos
se realizaba una única búsqueda mediante el uso de diferentes hilos, en esta se pretende simular la
búsqueda simultánea de varios usuarios en un sistema operativo multiusuario.
  
  
En esta nueva versión del buscador SSOOIIGLE será necesario modelar usuarios (clientes) como
entidades activas. Al no existir personas físicas que interactúen con el sistema, será necesario
emularlos mediante la creación de hilos o procesos. En otras palabras, cada usuario será un hilo o
proceso (decisión que deberá tomar el estudiante) que solicite al buscador la realización de una
búsqueda.
  
  
El sistema deberá disponer de un diccionario de palabras 1 . En el momento de la creación de un
usuario, se obtendrá al azar una de las palabras de este diccionario, y será la empleada para la
búsqueda. Además de la palabra a buscar, el usuario debe tener asociado un identificador, que lo
diferencia del resto, y una categoría o perfil:
  
  
- Usuario con cuenta gratuita.
- Usuario premium con límite de saldo. En este caso, el usuario debe tener asociada una cantidad de saldo, medida en créditos.
- Usuario premium con saldo ilimitado.
  
El perfil de un usuario determinará sus privilegios. Con la creación de los primeros usuarios deben
comenzar las primeras búsquedas y, al mismo tiempo que éstas se están realizando, la creación de
nuevos usuarios debe continuar, hasta un número determinado. Por ejemplo, podría probarse el
sistema con la creación de 50 usuarios, aunque este valor debe ser configurable. Se recomienza dejar
un tiempo prudencial en la creación entre clientes para conseguir la existencia simultánea de varios
de ellos y proporcionar un nivel de concurrencia adecuado para evaluar la práctica.
  
  
El servidor de búsquedas debe dar preferencia a los clientes premium en la siguiente relación: 80%
usuarios premium – 20 % usuarios con cuenta gratuita. Esto significa que, de 10 búsquedas realizadas,
2 deben ser para clientes con cuenta gratuita. No se debe producir inanición en clientes con cuenta
gratuita. Es decir, se debe atender a este tipo de clientes (en menor medida) a pesar de que existan
clientes premium.
  
  
El servicio de búsqueda debe ser concurrente, pero debe está limitado a N replicas. Esto quiere decir
que si N = 4, tan solo se podrán atender a cuatro clientes de forma simultánea. El resto deberá esperar
a que llegue su turno. Si en la práctica anterior se realizaba búsquedas sobre una única fuente o libro
(archivo .txt), en esta ocasión cada búsqueda se debe realizar sobre el conjunto de libros que estén
incluidos como fuentes. Además, en la práctica anterior se utilizaban varios hilos para un libro. Para
la práctica 3 se pide que la búsqueda se haga de forma simultánea en las diversas fuentes, lo que
implica que al menos exista un hilo para cada libro. Es opcional y mejora la calificación que el
estudiante consiga emplear varios hilos para la búsqueda en cada fuente.
  
El tipo de cliente también condiciona la búsqueda de la siguiente manera:
  
- **Cliente con cuenta gratuita:** la búsqueda está limitada a un número fijo de palabras. Si la
búsqueda ha superado ese valor límite, se debe dar por finalizada la búsqueda haciendo llegar
al cliente los resultados obtenidos hasta ese momento. Se debe tener especial cuidado en el
control de palabras, en el caso de emplear varios hilos para la búsqueda  
 
- **Cliente con cuenta premium con límite de saldo:** cada palabra encontrada supone el coste
de 1 crédito. Cuando el saldo del cliente llega a cero, la búsqueda debe detenerse
momentáneamente para solicitar al cliente un nuevo pago y recarga de saldo. Una vez hecho
esto, la búsqueda debe continuar. Se debe tener especial cuidado en el control de saldo, en el
caso de que se empleen varios hilos para la búsqueda. Más adelante se hablará con más detalle
del sistema de pago.
  
- **Cliente con cuenta premium ilimitada:** no tiene restricciones en la búsqueda.
  
El sistema de pago es un servicio que debe ser modelado mediante un hilo o proceso. Este servicio es
único, lo que quiere decir que no puede ser empleado al mismo tiempo por más de un cliente. El
hilo/proceso debe permanecer a la espera/bloqueado, hasta que un cliente solicite sus servicios, que
será cuando se agote su saldo. Este servicio no debe ser anónimo, el sistema de pago debe conocer la
identidad del cliente, y el servicio debe notificar al cliente que su saldo ha sido modificado cuando se
haya efectuado el pago. Si una búsqueda se ha detenido por falta de saldo, se deberá reanudar una vez
restaurado.
  
  
Cuando finaliza una búsqueda se deben hacer llegar los resultados al cliente que hizo la solicitud (y
que permanece a la espera), para que éste sea quien muestre por pantalla los resultados o bien los
registre en un archivo. El modo en el que deben mostrarse los resultados es exactamente el mismo al
de la segunda práctica, pero, además, se deberá medir el tiempo total de búsqueda dedicado al cliente
e incluirlo en los resultados.
  
  
Por último, se debe mostrar por pantalla la finalización de cada uno de los clientes y se debe
comprobar que sean tantos como se crearon inicialmente.
ATENCIÓN: en la siguiente página se incluye la autoevaluación que deberá ser completada por los
estudiantes. Se recomienda su lectura antes de comenzar a realizar la práctica.
