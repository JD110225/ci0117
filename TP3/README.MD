Paso de parametros:

El programa recibe 3 parametros por el metodo main, los dos primeros de ellos son necesarios
y corresponden a la cantidad de muestras y la cantidad de grupos o clusters respectivamente.
El tecer parametro es opcional y corresponde al nombre del archivo donde el usuario desee que
se genere la imagen en formato eps. Si el tercer parametro no se especifica, la imagen se generara en un archivo llamado resultado.eps.

Estrategia de asignacion de puntos a clases

    La primera estrategia es totalmente aleatoria (el profesor la programo) y la segunda 
corresponde en asignar de manera equitativa las muestras a cada clusters. Si por ejemplo se requiere trabajar con 15 muestras y 5 clusters, las primeras 3 muestras irian al primer cluster, las segundas 3 al segundo cluster etc... Si la cantidad de muestras no es un multiplo de la cantidad de clusters (e.g: 10 y 3), se le asigna a cada cluster un total de (muestras/clusters + 1) elementos y al ultimo cluster las muestras restantes.

Uso de OMP

    Se decidio paralelizar el programa en tres partes distintas. La primera de ellas a la hora 
de asignar las muestras a las diferentes clases(clusters), luego para calcular el centro mas
cercano de todos los puntos y por ultimo se generaron 3 hilos que corren el algoritmo principal
de manera concurrente, cada hilo generara un resultado distinto(con eleccion aleatoria de clusters se obtendran diversos resultados) y la imagen final se generara
con la solucion del hilo que tuvo la menor disimilaridad.   
