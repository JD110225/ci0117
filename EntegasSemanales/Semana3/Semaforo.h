class Semaforo {
public:
    Semaforo( int ValorInicial = 0 );
    ~Semaforo();
    int signal();	// Puede llamarse V
    int wait();	// Puede llamarse P
private:
    int creador;
    int id;		// Identificador del semaforo
};