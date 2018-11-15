#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <ProcesoPar.h>
#include <iostream>

using namespace std;

int destruirProcesoPar(ProcesoPar_t *procesoPar) {
  int joinVal;
  int *retval[1];
  //sleep(1);
  //cout << "in: " <<
  close(procesoPar->tuberia_in[1]);
  //<< " " << errno << endl;
  //cout << "out" << close(procesoPar->tuberia_out[0]) << errno << endl;
  //pthread_kill(procesoPar->thread, SIGINT);
  joinVal = pthread_join(procesoPar->thread, (void**) &retval[0]);
  //cout << joinVal << " " << **retval << endl;
  return joinVal;
}
