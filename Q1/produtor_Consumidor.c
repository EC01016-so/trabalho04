#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

/* Define o máximo de itens que um produtor pode produzir
ou um consumidor pode consumir */
#define Maximo 5
// Define o tamanho do buffer
#define TBuffer 5

// Funções do produtor e do consumidor
void *produtor(void *pno);
void *consumidor(void *cno);
// Declaração dos semáforos
sem_t vazio;
sem_t cheio;

/* Variavel de controle, para verificar quem está dentro e 
quem está fora*/
int dentro = 0;
int fora = 0;
int buffer[TBuffer];

pthread_mutex_t mutex;

int main()
{   
    // Criação de variavel do tipo thread
    pthread_t pro[5],con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&vazio,0,TBuffer);
    sem_init(&cheio,0,0);

    // Numerar produtor e consumidor
    int a[5] = {1,2,3,4,5};
    
    // Loop para criação de threads
    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)produtor, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumidor, (void *)&a[i]);
    }
    
    // Loop para fazer juntar as threads
    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);
    }
    
    // Encerra as threads
    pthread_mutex_destroy(&mutex);
    sem_destroy(&vazio);
    sem_destroy(&cheio);

    return 0;
}

// Função Produtor
void *produtor(void *pno)
{   
    int item;
    for(int i = 0; i < Maximo; i++) {
        item = rand() % 10; // Produz um item aleatório
        sem_wait(&vazio);
        pthread_mutex_lock(&mutex);
        buffer[dentro] = item;
        printf("Produtor %d: Insere item %d em %d\n", *((int *)pno),buffer[dentro],dentro);
        dentro = (dentro+1)%TBuffer;
        pthread_mutex_unlock(&mutex);
        sem_post(&cheio);
    }
    return 0;
}

// Função Consumidor
void *consumidor(void *cno)
{   
    for(int i = 0; i < Maximo; i++) {
        sem_wait(&cheio);
        pthread_mutex_lock(&mutex);
        int item = buffer[fora];
        printf("Consumidor %d: Remove item %d de %d\n",*((int *)cno),item, fora);
        fora = (fora+1)%TBuffer;
        pthread_mutex_unlock(&mutex);
        sem_post(&vazio);
    }
    return 0;
}
