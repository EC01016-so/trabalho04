#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

// Funções
void* filos(void *);
void come(int);

/* Declarando semáforos */

// Filosófos à mesa
sem_t filosofos;
// Garfos
sem_t garfo[5];

int main()
{
	int i, a[5];
	pthread_t tid[5];
	
	// Inicializar semáforo
	sem_init(&filosofos, 0, 4);
	
	for(i=0;i<5;i++)
		sem_init(&garfo[i], 0, 1);
		
	for(i=0;i<5;i++){
		a[i]=i;
		pthread_create(&tid[i], NULL, filos, (void *)&a[i]);
	}
	for(i=0;i<5;i++)
		pthread_join(tid[i], NULL);
}

void* filos(void * num)
{
	int fil = *(int *)num;

	sem_wait(&filosofos);
	sem_wait(&garfo[fil]);
	sem_wait(&garfo[(fil+1)%5]);

	come(fil);
	sleep(2);
	printf("Filósofo %d terminou de comer\n", fil);

	sem_post(&garfo[(fil+1)%5]);
	sem_post(&garfo[fil]);
	sem_post(&filosofos);
    
    return 0;
}

void come(int fil)
{
	printf("Filósofo %d está comendo\n",fil);
}
