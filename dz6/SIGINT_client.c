#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>

#define SHM_SIZE 1024

int shmid;
int *shm_ptr;

void sig_handler(int signo)
{
  if (signo == SIGINT) {
    // Удаление сегмента разделяемой памяти
    if (shmdt(shm_ptr) == -1) {
      perror("shmdt");
      exit(1);
    }
    exit(0);
  }
}

int main()
{
  key_t key = 1234; // Ключ для создания сегмента разделяемой памяти

  // Создание сегмента разделяемой памяти
  if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) == -1) {
    perror("shmget");
    exit(1);
  }

  // Получение указателя на сегмент разделяемой памяти
  if ((shm_ptr = shmat(shmid, NULL, 0)) == (int *) -1) {
    perror("shmat");
    exit(1);
  }

  // Регистрация обработчика сигнала SIGINT
  if (signal(SIGINT, sig_handler) == SIG_ERR) {
    perror("signal");
    exit(1);
  }

  // Генерация случайных чисел и запись их в сегмент разделяемой памяти
  srand(time(NULL));
  for (int i = 0; i < 10; i++) {
    int num = rand() % 100;
    printf("Generated number: %d\n", num);
    shm_ptr[i] = num;
    sleep(1);
  }

  return 0;
}
