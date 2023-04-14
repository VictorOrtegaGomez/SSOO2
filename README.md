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
