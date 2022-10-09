#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_CLIENTES 5 // ** Pode ser trocado

// Funções
void *cliente(void *num);
void *barbeiro(void *barb);
void taleatorio(int s); 

/* Declarando semáforos */

// Número de clientes esperando
sem_t espera;
// Cadeira
sem_t cadeira;
// Barbeiro pode dormir
sem_t amimir;
// Cliente não pode se levantar enquanto corta o cabelo
sem_t cortando;

int terminado = 0;

int main(int argc, char *argv[]) {
	pthread_t barb;
	pthread_t cli[NUM_CLIENTES];
	int i, clientes, cadeiras;
	int Numero[NUM_CLIENTES];

	printf("Insira o número de clientes: \n"); scanf("%d",&clientes) ;
	printf("Insira o número de cadeiras: \n"); scanf("%d",&cadeiras);

	if (clientes > NUM_CLIENTES) {
		printf("O número máximo de clientes é %d\n", NUM_CLIENTES);
		exit(-1);
	}

	for (i=0; i<NUM_CLIENTES; i++) {
		Numero[i] = i;
	}

	// Inicializar os semáforos
	sem_init(&espera, 0, cadeiras);
	sem_init(&cadeira, 0, 1);
	sem_init(&amimir, 0, 0);
	sem_init(&cortando, 0, 0);

	// Criação da thread do barbeiro
	pthread_create(&barb, NULL, barbeiro, NULL);

	// Criação da thread dos clientes
	for (i=0; i<clientes; i++) {
		pthread_create(&cli[i], NULL, cliente, (void *)&Numero[i]);
	sleep(1);
	}

	for (i=0; i<clientes; i++) {
		pthread_join(cli[i], NULL);
		sleep(1);
	}

	terminado = 1;
	sem_post(&amimir);
	pthread_join(barb, NULL);
}

// Função cliente
void *cliente(void *numero) {
	int num = *(int *)numero;
	printf("Cliente %d indo para barbearia\n", num);
	taleatorio(2);
	printf("Cliente %d chegou à barbearia\n", num);
	sem_wait(&espera);
	printf("Cliente %d entrou na sala de espera\n", num);
	sem_wait(&cadeira);
	sem_post(&espera);
	printf("Cliente %d acordando o barbeiro\n", num);
	sem_post(&amimir);
	sem_wait(&cortando);
	sem_post(&cadeira);
	printf("Cliente %d saindo da barbearia\n", num);
    return 0;
}

// Função barbeiro
void *barbeiro(void *barb) {
	while (!terminado) {
		printf("O barbeiro está dormindo\n");
		sem_wait(&amimir);
		if (!terminado) {
			printf("O barbeiro está cortando cabelo\n");
			taleatorio(2);
			printf("O barbeiro terminou de cortar cabelo\n");
			sem_post(&cortando);
		}
		else {
			printf("O barbeiro irá para casa\n");
		}
	}
    return 0;
}

// Função de tempo aleatório
void taleatorio(int s) {
	int len;
	len = (int) ((1 * s) + 1);
	sleep(len);
}
