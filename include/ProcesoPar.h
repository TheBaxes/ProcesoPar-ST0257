#pragma once
#include <unistd.h>

enum Errors {}

typedef struct ProcesoPar {
  pid_t hijo;
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
