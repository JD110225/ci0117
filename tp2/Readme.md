Input: 

   Los argumentos  correspondientes a los archivos que se deseen procesar,la cantidad de trabajadores por archivo y las estrategias de lectura deberan ser pasados por linea de comandos. El usuario debera especificar tres flags: -t -e -f, despues de la bandera -t debera especificar la cantidad de trabajadores por archivo. La bandera -e se utiliza para especificar las estrategias de lectura que se deseen emplear y por utlimo -f indicara el nombre de los archivos. 

Ejemplos validos de input:
1)./m.exe -t 3 -e 4 -f ecci.html
   Mediante este comando, se indica que se quiere procesar el archivo "ecci.html" mediante la cuarta estrategia de lectura y utilizando 3 hilos(trabajadores).
2) ./m.exe -t 10 -e 1,2 -f ecci.html,ecci.html
   En este caso se indica que se quiere contabilizar la cantidad de etiquetas html del archivo "ecci.html" dos veces, la primera vez utilizando la primera estrategia y la segunda vez utilizando la segunda estrategia, ademas se utilizaran 10 trabajadores por cada archivo.

Manejo de Errores:
   En cuanto a los posibles errores generados por la entrada del usuario, se contemplaron los casos en que se especifiquen mas estrategias que archivos o viceversa, cuando se pide leer un archivo inexistente, cuando se selecciona una estrategia invalida y cuando se ingresa un numero negativo de trabajadores.

Estrategia original:

   Es una variacion del mapeo estatico equitativo por bloque,con la diferencia de que a cada hilo en vez de tocarle una cantidad de lineas igual a X, donde X es el cociente de la cantidad de lineas y la cantidad de hilos, a los hilos les corresponde una cantidad de lineas aleatoria entre 1 y X, esto implica que al ultimo hilo en llegar a la funcion se le asignen todas las lineas restantes (e.g: Si en mapeo estatico a cada hilo le corresponden 5 lineas, entonces en esta estrategiaa cada hilo le corresponderian de 1 a 5 lineas y al ultimo todas las restantes)

