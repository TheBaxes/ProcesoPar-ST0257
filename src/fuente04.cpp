#include <sys/types.h>
#include <unistd.h>
#include <ProcesoPar.h>

using namespace std;

int establecerFuncionDeEscucha(ProcesoPar_t *procesoPar,
			       int (*f)(const void *, unsigned int)) {
  sem_wait(&procesoPar->mutex);
  procesoPar->f = f;
  sem_post(&procesoPar->mutex);
  return 0;
}
