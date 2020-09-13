#include "Lock.h"
#include "Condition.h"

#define FILOMAX 5

class Mesa {

   public:
      Mesa();
      ~Mesa();
      int pickup( int );
      int putdown( int );


   private:
     Lock * lock;
     int test( int );
     enum { THINKING, HUNGRY, EATING } state[ 5 ];
     Condition  * self[ 5 ];	// inanición (starvation)

};