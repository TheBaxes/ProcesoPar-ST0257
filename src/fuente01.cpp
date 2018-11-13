#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <ProcesoPar.h>

using namespace std;

void* hilo(void *args) {
  ProcesoPar_t *procesoPar = (ProcesoPar_t *) args;
  
  char msg[80];
  int result;
  
  while((result = read(procesoPar->tuberia_out[0], msg, 80)) > 0) {
    if (procesoPar->f){
      procesoPar->f(&msg, 80);
    }
  }

  if (result < 0) {
    cerr << "Error at reading pipe" << endl;
    pthread_exit((void*) &result);
  }
  pthread_exit(0);
}

ProcesoPar_t *lanzarProcesoPar(const char *nombreArchivoEjecutable,
			       const char **listaLineaComando,
			       const char **env){
  ProcesoPar_t *procesoPar = new ProcesoPar_t();

  procesoPar->f = nullptr;
  if (pipe(procesoPar->tuberia_in) < 0) return nullptr;
  if (pipe(procesoPar->tuberia_out) < 0) return nullptr;
  procesoPar->hijo = fork();
  
  if (procesoPar->hijo == -1) {
    cout << "Error: " << errno << endl;

    if (errno == EAGAIN) {
      cout << "Not enough resources to create thread" << endl;
    }

    exit(-1);
  }

  if (procesoPar->hijo == 0) {
    dup2(procesoPar->tuberia_in[0], 0);
    dup2(procesoPar->tuberia_out[1], 1);
    close(procesoPar->tuberia_in[0]);
    close(procesoPar->tuberia_in[1]);
    close(procesoPar->tuberia_out[0]);
    close(procesoPar->tuberia_out[1]);
    execve(nombreArchivoEjecutable, (char* const *)listaLineaComando, (char* const *)env);
    exit(EXIT_FAILURE);
  }
  
  close(procesoPar->tuberia_in[0]);
  close(procesoPar->tuberia_out[1]);
  pthread_create(&procesoPar->thread, NULL, hilo, &procesoPar);
  
  return procesoPar;
}
