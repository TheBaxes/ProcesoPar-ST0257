#include <cerrno>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ProcesoPar.h>

using namespace std;

int abrirHijo(ProcesoPar_t *procesoPar, const char *nombreArchivoEjecutable,
	      char *const *listaLineaComando,
	      char *const *env,
	      int *errorP);
void* hilo(void *args);
int procesarPadre(ProcesoPar_t *procesoPar, int *errorP);

ProcesoPar_t *lanzarProcesoPar(const char *nombreArchivoEjecutable,
			       char *const *listaLineaComando,
			       char *const *env){
  ProcesoPar_t *procesoPar = new ProcesoPar_t();
  sem_init(&procesoPar->mutex, 0, 1);
  procesoPar->tuberia_in = new int[2]; procesoPar->tuberia_out = new int[2];
  int errorP[2];
  if (pipe(procesoPar->tuberia_in) != 0 || pipe(procesoPar->tuberia_out) != 0 ||
      pipe(errorP) != 0) return nullptr;
  if (fcntl(errorP[1], F_SETFD, fcntl(errorP[1], F_GETFD) | FD_CLOEXEC))
    return nullptr;
  if ((procesoPar->hijo = fork()) == -1) return nullptr;
  if (procesoPar->hijo == 0) {
    if (abrirHijo(procesoPar, nombreArchivoEjecutable,
		  listaLineaComando, env, errorP) < 0) exit(EXIT_FAILURE);
  } else {
    int result = procesarPadre(procesoPar, errorP);
    if (result > 0) return nullptr;
  }
  return procesoPar;
}

int procesarPadre(ProcesoPar_t *procesoPar, int *errorP) {
  int error = pthread_create(&procesoPar->thread, NULL, hilo, procesoPar);
  if (error != 0) {
    errno = error;
    return -1;
  }
  if (close(procesoPar->tuberia_in[0]) != 0 ||
      close(procesoPar->tuberia_out[1]) != 0 ||
      close(errorP[1])) return -1;
  int err;
  int count = read(errorP[0], &err, sizeof(err));
  if (count > 0) {
    errno = err;
    return err;
  }
  return 0;
}
  
int abrirHijo(ProcesoPar_t *procesoPar, const char *nombreArchivoEjecutable,
	      char *const *listaLineaComando,
	      char *const *env,
	      int *errorP) {
  if (dup2(procesoPar->tuberia_in[0], 0) != 0 ||
      close(procesoPar->tuberia_in[0]) != 0 ||
      close(procesoPar->tuberia_in[1]) != 0) {
    return -1;
    }
  if (dup2(procesoPar->tuberia_out[1], 1) != 1 ||
      close(procesoPar->tuberia_out[1]) != 0 ||
      close(procesoPar->tuberia_out[0]) != 0) {
    return -1;
    }
  if (close(errorP[0]) != 0) return -1;
  execve(nombreArchivoEjecutable, listaLineaComando, env);
  write(errorP[1], &errno, sizeof(int));
  return -1;
}

void* hilo(void *args) {
  ProcesoPar_t *procesoPar = (ProcesoPar_t *) args;
  char msg[80];
  int result;
  int *ret = new int;
  *ret = 0;
  while(true){
    result = read(procesoPar->tuberia_out[0], msg, sizeof(msg)-1);
    msg[result] = '\0';
    if (result < 0) {
      *ret = errno;
      pthread_exit(ret);
    } else if (result == 0) {
      pthread_exit(ret);
    }
    if (procesoPar->f){
      sem_wait(&procesoPar->mutex);
      procesoPar->f(msg, sizeof(msg));
      sem_post(&procesoPar->mutex);
    }    
  }
  return ret;
}
