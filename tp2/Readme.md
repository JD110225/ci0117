//Estrategia original:
   // Es una variacion del mapeo estatico equitativo por bloque,con la diferencia
   // de que a cada hilo en vez de tocarle una cantidad de lineas igual a X, donde
   // X es el cociente de la cantidad de trabajadores y la cantidad de hilos, a los hilos
   // les corresponde una cantidad de lineas aleatoria entre 1 y X, esto implica que al
   // ultimo hilo en llegar a la funcion se le asignen todas las lineas restantes 
   //(e.g: Si en mapeo estatico a cada hilo le corresponden 5 lineas, entonces en esta estrategia
   // a cada hilo le corresponderian de 1 a 5 lineas y al ultimo todas las restantes)