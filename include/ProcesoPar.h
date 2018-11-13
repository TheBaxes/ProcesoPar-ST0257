#pragma once
#include <pthread.h>
#include <unistd.h>

enum Errors {};

typedef struct ProcesoPar {
  pthread_t thread;
  pid_t hijo;
  int tuberia_in[2];
  int tuberia_out[2];
  int (*f)(const void*, unsigned int);
} ProcesoPar_t;

ProcesoPar_t *lanzarProcesoPar(const char *nombreArchivoEjecutable,
			       const char **listaLineaComando,
			       const char **env);

int destruirProcesoPar(ProcesoPar_t *procesoPar);

int enviarMensajeProcesoPar(ProcesoPar_t *procesoPar,
			    const void *mensaje,
			    unsigned int longitud);

int establecerFuncionDeEscucha(ProcesoPar_t *procesoPar,
			       int (*f)(const void *, unsigned int) );

int funcion(const void* msg, unsigned int longitud);
