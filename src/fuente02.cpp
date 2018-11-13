#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <ProcesoPar.h>

using namespace std;

int destruirProcesoPar(ProcesoPar_t *procesoPar) {
  int joinVal, retval;
  close(procesoPar->tuberia_in[0]);
  close(procesoPar->tuberia_in[1]);
  close(procesoPar->tuberia_out[0]);
  close(procesoPar->tuberia_out[1]);
  joinVal = pthread_join(procesoPar->thread, (void**) &retval);
  return joinVal;
}
