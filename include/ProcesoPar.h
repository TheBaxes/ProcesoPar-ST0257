#pragma once
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

enum Errors {};

typedef struct ProcesoPar {
  pthread_t thread;
  pid_t hijo;
  int *tuberia_in;
  int *tuberia_out;
  int (*f)(const void*, unsigned int);
  sem_t mutex;
} ProcesoPar_t;

ProcesoPar_t *lanzarProcesoPar(const char *nombreArchivoEjecutable,
			       char *const *listaLineaComando,
			       char *const *env);

int destruirProcesoPar(ProcesoPar_t *procesoPar);

int enviarMensajeProcesoPar(ProcesoPar_t *procesoPar,
			    const void *mensaje,
			    unsigned int longitud);

int establecerFuncionDeEscucha(ProcesoPar_t *procesoPar,
			       int (*f)(const void *, unsigned int) );

int funcion(const void* msg, unsigned int longitud);
