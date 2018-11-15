#include <iostream>
#include <cerrno>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <ProcesoPar.h>
#include <semaphore.h>

using namespace std;

void* hilo(void *args) {
  ProcesoPar_t *procesoPar = (ProcesoPar_t *) args;

  char msg[80];
  int result;
    
  while(true){
    result = read(procesoPar->tuberia_out[0], msg, sizeof(msg)-1);
    msg[result] = '\0';
    
    if (result < 0) {
      cerr << "Error: " << errno << " at reading pipe" << endl;
      int *ret = new int;
      *ret = errno;
      pthread_exit(ret);
    } else if (result == 0) {
      int *ret = new int;
      *ret = 0;
      pthread_exit(ret);
    }
    
    if (procesoPar->f){
      sem_wait(&procesoPar->mutex);
      procesoPar->f(msg, sizeof(msg));
      sem_post(&procesoPar->mutex);
    }    
  }
  int *ret = new int;
  *ret = 0;
  return ret;
}

ProcesoPar_t *lanzarProcesoPar(const char *nombreArchivoEjecutable,
			       char *const *listaLineaComando,
			       char *const *env){
  ProcesoPar_t *procesoPar = new ProcesoPar_t();

  sem_init(&procesoPar->mutex, 0, 1);
  
  procesoPar->tuberia_in = new int[2];
  procesoPar->tuberia_out = new int[2];
  
  if (pipe(procesoPar->tuberia_in) != 0) return nullptr;
  if (pipe(procesoPar->tuberia_out) != 0) return nullptr;
  procesoPar->hijo = fork();
  
  if (procesoPar->hijo == -1) {
    return nullptr;
  }

  if (procesoPar->hijo == 0) {
    
    if (dup2(procesoPar->tuberia_in[0], 0) != 0 ||
    	close(procesoPar->tuberia_in[0]) != 0 ||
    	close(procesoPar->tuberia_in[1]) != 0) {
      exit(-1);
    }
    if (dup2(procesoPar->tuberia_out[1], 1) != 1 ||
    	close(procesoPar->tuberia_out[1]) != 0 ||
    	close(procesoPar->tuberia_out[0]) != 0) {
      exit(-1);
    }
    execve(nombreArchivoEjecutable, listaLineaComando, env);
    exit(-1);
  } else {
    
    int error = pthread_create(&procesoPar->thread, NULL, hilo, procesoPar);
    if (error != 0) {
      errno = error;
      return nullptr;
    }
    if (close(procesoPar->tuberia_in[0]) != 0 ||
	close(procesoPar->tuberia_out[1]) != 0) {
      return nullptr;
    }
  }
  return procesoPar;
}
