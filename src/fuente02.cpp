#include <pthread>
#include <sys/types.h>
#include <signal.h>

using namespace std;

int destruirProcesoPar(ProcesoPar_t *procesoPar) {
  return kill(procesoPar->hijo, 9);
}
