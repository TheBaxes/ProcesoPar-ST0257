#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
#include <ProcesoPar.h>

using namespace std;

int enviarMensajeProcesoPar(ProcesoPar_t *procesoPar,
			    const void *mensaje,
			    unsigned int longitud) {
  if (waitpid(procesoPar->hijo, NULL, WNOHANG) != 0) {
    errno = ESRCH;
    return -1;
  }
  return (write(procesoPar->tuberia_in[1], mensaje, longitud) != longitud)?
    -1 : 0;
}
