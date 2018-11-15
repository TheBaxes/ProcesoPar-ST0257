#include <iostream>
#include <cerrno>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <ProcesoPar.h>
#include <semaphore.h>

using namespace std;

//sem_t mutex;

void* hilo(void *args) {
  //  cout << "start" << endl;
  ProcesoPar_t *procesoPar = (ProcesoPar_t *) args;
  
  //char l[5];
  //int kk = read(procesoPar->tuberia_out[0], l, sizeof(l));
  //cerr << errno << " we " << l << endl;

  //sem_post(&mutex);
  
  char msg[10];
  int result;
  
  while(1){
    result = read(procesoPar->tuberia_out[0], msg, sizeof(msg));
    //msg[result] = '\0';
    //cerr << msg << endl;

    //sleep(1);
    
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
  pthread_exit(ret);
  return ret;
}

ProcesoPar_t *lanzarProcesoPar(const char *nombreArchivoEjecutable,
			       char *const *listaLineaComando,
			       char *const *env){
  ProcesoPar_t *procesoPar = new ProcesoPar_t();

  sem_init(&procesoPar->mutex, 0, 1);
  
  procesoPar->tuberia_in = new int[2];
  procesoPar->tuberia_out = new int[2];
  
  //procesoPar->f = f;
  if (pipe(procesoPar->tuberia_in) < 0) return nullptr;
  if (pipe(procesoPar->tuberia_out) < 0) return nullptr;
  //const char * p = "alo";
  procesoPar->hijo = fork();
  
  //cout << "xd: " << procesoPar->tuberia_in << endl;
  
  if (procesoPar->hijo == -1) {
    cout << "Error: " << errno << endl;

    if (errno == EAGAIN) {
      cerr<< "Not enough resources to create thread" << endl;
    }

    exit(-1);
  }

  if (procesoPar->hijo == 0) {
    
    //write(procesoPar->tuberia_out[1], p, 5);
    if (dup2(procesoPar->tuberia_in[0], 0) != 0 ||
    	close(procesoPar->tuberia_in[0]) != 0 ||
    	close(procesoPar->tuberia_in[1]) != 0)
      {
    	cerr << "Child: failed to set up standard input\n";
    	exit(1);
      }
    if (dup2(procesoPar->tuberia_out[1], 1) != 1 ||
    	close(procesoPar->tuberia_out[1]) != 0 ||
    	close(procesoPar->tuberia_out[0]) != 0)
      {
    	cerr << "Child: failed to set up standard output\n";
    	exit(1);
      }
    execve(nombreArchivoEjecutable, listaLineaComando, env);
    //cerr << "WE FAILED" << endl;
    exit(EXIT_FAILURE);
  } else {
    
    pthread_create(&procesoPar->thread, NULL, hilo, procesoPar);
    //sem_wait(&mutex);
    close(procesoPar->tuberia_in[0]);
    close(procesoPar->tuberia_out[1]);

  }
  
  return procesoPar;
}
