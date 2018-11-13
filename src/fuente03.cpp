#include <sys/types.h>
#include <unistd.h>
#include <ProcesoPar.h>

using namespace std;

int enviarMensajeProcesoPar(ProcesoPar_t *procesoPar,
			    const void *mensaje,
			    unsigned int longitud) {
  return write(procesoPar->tuberia_in[0], mensaje, longitud);
}
