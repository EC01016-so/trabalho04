#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

// Funções Filósofos e "comer"
void* filos(void *);
void* come(int);

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
	
	// Loop para distrubuir garfos para os 5 filósofos
	for(i = 0; i < 5 ; i++){
		sem_init(&garfo[i], 0, 1);
	}
	
	// Loop para criação de threads
	for(i = 0; i < 5; i++){
		a[i]=i;
		pthread_create(&tid[i], NULL, filos, (void *)&a[i]);
	}
	// Loop para fazer juntar as threads
	for(i = 0; i < 5; i++){
		pthread_join(tid[i], NULL);
	}
}
// A thread executa a função "Filos"
void* filos(void * num)
{
	int fil = *(int *)num;
	
	// Aguarda caso não possua garfo
	sem_wait(&filosofos);
	sem_wait(&garfo[fil]);
	sem_wait(&garfo[(fil+1)%5]);

	// O filósofo, quando tem garfo, come
	sleep(2);
	come(fil);

	// Após comer, o próximo filósofo irá comer
	printf("Filósofo %d terminou de comer\n", fil);
	sem_post(&garfo[(fil+1)%5]);
	sem_post(&garfo[fil]);
	sem_post(&filosofos);
    
    return 0;
}
// A função "Filos" executa a função "come"
void* come(int fil)
{
	// Indica qual filósofo está comendo
	sleep(1);
	printf("Filósofo %d está comendo\n",fil);
	return 0;
}
