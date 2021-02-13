#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>

#define BABYBIRDS 3
#define FULLDISH 5;
#define SHARED 1
#define TRUE 1

sem_t eat, empty;
int worms = FULLDISH;


void *baby_bird(void *arg){

    long id = (long)arg;
  
    while (TRUE)
    {
        sem_wait(&eat);
        sleep(1);
        if(0 >= worms){
            printf("Baby bird %ld finds empty dish and waits for parent to get new worms!\n", id);
            sem_post(&empty);
            while (0 >= worms){
                printf("CHIRP!!\n");
                sleep(1);
            }
            sleep(2);
        }

        worms--;
        printf("Baby bird %ld eats and there are %d worms left!\n", id, worms);
        sem_post(&eat);
        sleep(1);
    }
}

void *parent_bird(void * arg){
    
    while (TRUE)
    {
       sem_wait(&empty);
       sleep(5);
       worms = FULLDISH;
       printf("The parent has gotten new worms and has saved the day!\n");
    }
}

int main(){

    pthread_t parent;
    pthread_t *babyBird = malloc(BABYBIRDS*sizeof(pthread_t));
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&eat, SHARED, 1);
    sem_init(&empty, SHARED, 0);

    printf("The birds are alive!!!\n");

    pthread_create(&parent, &attr, parent_bird, NULL);

    for(long i = 0;i < BABYBIRDS; i++)
        pthread_create(&babyBird[i], &attr, baby_bird, (void *)(i + 1));

    pthread_join(parent, NULL);

    for (size_t i = 0; i < BABYBIRDS; i++)
        pthread_join(babyBird[i], NULL);
    
    free(babyBird);
}