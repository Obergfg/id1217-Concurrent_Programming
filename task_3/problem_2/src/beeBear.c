#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>

#define BEES 3
#define CAPACITY 5
#define SHARED 1
#define TRUE 1

sem_t fill, wake;
pthread_mutex_t lock;
int portions = 0;

void *buzy_bee(void *arg)
{

    long id = (long)arg;

    while (TRUE)
    {
        sem_wait(&fill);
        portions++;
        sleep(1);

        if (CAPACITY <= portions)
        {
            printf("Bee %ld has filled up the honey pot and awakens the bear!\n", id);
            sem_post(&wake);
        }
        else
        {
            printf("Bee %ld has added one portion of honey to the pot! The pot has %d portions of honey in it!\n", id, portions);
            sem_post(&fill);
        }
        sleep(1);
    }
}

void *hungry_bear(void *arg)
{

    while (TRUE)
    {
        sem_wait(&wake);
        portions = 0;
        sleep(1);
        printf("The bear has eaten all the honey and goes back to sleep!\n");
        sleep(1);
        sem_post(&fill);
    }
}

int main()
{

    pthread_t bear;
    pthread_t *bee = malloc(BEES * sizeof(pthread_t));
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&fill, SHARED, 1);
    sem_init(&wake, SHARED, 0);

    printf("The bees are busy!!!\n");

    pthread_create(&bear, &attr, hungry_bear, NULL);

    for (long i = 0; i < BEES; i++)
        pthread_create(&bee[i], &attr, buzy_bee, (void *)(i + 1));

    pthread_join(bear, NULL);

    for (size_t i = 0; i < BEES; i++)
        pthread_join(bee[i], NULL);

    free(bee);
}