Explicacion Estrategia matrices:
    A cada hilo se le asigna calcular una fila de la matriz resultado, lo que esto implica
es que si se quiere realizar la multiplicacion: A X B donde A y B son dos matrices cuadradas, entonces mediante la funcion Scatter de MPI, cada hilo recibe una fila de la matriz A. Para realizar esta asignacion de filas, primero se convierte la matriz a un arreglo unidimensional y luego se reparte este arreglo con Scatter equitativamente entre threads.Esto signfica que la cantidad de hilos debe ser igual a la cantidad de filas de la matriz A. Luego mediante el metodo "multMatrizVector" , cada hilo toma la fila de la matriz A que recibio de Scatter y la multiplica por la matriz B y por ultimo se emplea la funcion Gather para recopilar los resultados de la matriz resultado.

NOTA: VERIFICAR EN MATRICES QUE LA COSNTANTE SIZEDEFAULT SEA IGUAL AL VALOR ENVIADO MEDIANTE -N
A LA HORA DE CORRER EL PROGRAMA CON MPI.