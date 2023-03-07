#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int shmid;
int *shm_ptr;

void cleanup()
{
  // Удаление сегмента разделяемой памяти
  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("shmctl");
    exit(1);
  }
}

int main()
{
  key_t key = 1234; // Ключ для создания сегмента разделяемой памяти

  // Получение идентификатора сегмента разделяемой памяти
  if ((shmid = shmget(key, SHM_SIZE, 0666)) == -1) {
    perror("shmget");
    exit(1);
  }

  // Получение указателя на сегмент разделяемой памяти
  if ((shm_ptr = shmat(shmid, NULL, 0)) == (int *) -1) {
    perror("shmat");
    exit(1);
  }

  // Вывод случайных чисел из сегмента разделяемой памяти
  for (int i = 0; i < 10; i++) {
    printf("Received number: %d\n", shm_ptr[i]);
    sleep(1);
  }

  // Регистрация функции для удаления сегмента разделяемой памяти
  if (atexit(cleanup) != 0) {
    fprintf(stderr, "Failed to register cleanup function\n");
    exit(1);
  }

  return 0;
}
