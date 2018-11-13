#include <sys/types.h>
#include <unistd.h>
#include <ProcesoPar.h>

using namespace std;

int establecerFuncionDeEscucha(ProcesoPar_t *procesoPar,
			       int (*f)(const void *, unsigned int)) {
  procesoPar->f = f;
  return 0;
}
