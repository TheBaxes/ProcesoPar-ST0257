#include <sys/types.h>
#include <unistd.h>
#include <ProcesoPar.h>

using namespace std;

int enviarMensajeProcesoPar(ProcesoPar_t *procesoPar,
			    const void *mensaje,
			    unsigned int longitud) {
  sem_wait(&procesoPar->mutex);
  int result = write(procesoPar->tuberia_in[1], mensaje, longitud) != longitud;
  sem_post(&procesoPar->mutex);
  return result;
}
