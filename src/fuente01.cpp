#include <pthread>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

ProcesoPar_t *lanzarProcesoPar(const char *nombreArchivoEjecutable,
			       const char **listaLineaComando,
			       const char **env){
  pid_t hijo = fork();
  
  if (hijo == -1) {
    cout << "Error: " << errno << endl;

    if (errno == EAAGAIN) {
      // Aqui pasa algo
    }

    exit(0);
  }

  if (hijo == 0) {
    execve(nombreArchivoEjecutable, listaLineaComando, env);
    exit(10);
  }

  ProcesoPar_t procesoPar;

  procesoPar.hijo = hijo;

  return &procesoPar;
}
