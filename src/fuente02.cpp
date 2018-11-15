#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctime>
#include <cerrno>
#include <ProcesoPar.h>

using namespace std;

int destruirProcesoPar(ProcesoPar_t *procesoPar) {
  int *retval[1];
  time_t start, now;
  pid_t status;
  
  if (close(procesoPar->tuberia_in[1]) != 0) return -1;

  time(&start);
  time(&now);
  while ((status = waitpid(procesoPar->hijo, NULL, WNOHANG)) == 0 &&
	 difftime(start, now) < 30) {
    usleep(10000);
    time(&now);
  }
  
  if (status == 0) {
    kill(procesoPar->hijo, SIGKILL);
    pthread_kill(procesoPar->thread, SIGKILL);
    if (close(procesoPar->tuberia_out[0]) != 0) return -1;
    errno = ETIMEDOUT;
    return -1;
  }
  
  if (pthread_join(procesoPar->thread, (void**) &retval[0]) != 0) return -1;
  if (close(procesoPar->tuberia_out[0]) != 0) return -1;
  if (*retval[0] != 0) {
    errno = *retval[0];
    return -1;
  }
  return 0;
}
