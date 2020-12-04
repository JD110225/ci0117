Estudiantes:
    Jose David Delgado, B92570
    David Rojas B96873

Nota: Se mantuvo la propuesta inicial del algoritmo. Adicionalmente, todos los procesos envian las subsecuencias calculadas al proceso 0, y este se encarga de verificar cual de ellas pertenece a la otra hilera. Para esto cada proceso tiene su propio contador de cuantas subsecuencias calculo. Este contador se envia por mensaje al proceso 0, para asi poder establecer un numero especifico de veces que se tiene que llamar al metodo MPI_Recv(para recibir todas las subsecuencias que no fueron calculadas por el proceso 0)


