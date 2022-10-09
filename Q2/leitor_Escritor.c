#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

/*
10 leitores e 5 escritores ** Pode ser trocado
*/

// Funções
void *escritor(void *wno);
void *leitor(void *rno);

// Declarar semáforo para escritor
sem_t escr;

int cnt = 1;
int numleit = 0;

pthread_mutex_t mutex;

int main()
{

    pthread_t ler[10], escrever[5];
    pthread_mutex_init(&mutex, NULL);
    // Inicializar semáforo
    sem_init(&escr, 0, 1);

    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // Numerar leitor e escritor
    
    for(int i = 0; i < 10; i++) {
        pthread_create(&ler[i], NULL, (void *)leitor, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&escrever[i], NULL, (void *)escritor, (void *)&a[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(ler[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(escrever[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&escr);

    return 0;

}

// Função escritor
void *escritor(void *wno) {
    sem_wait(&escr);
    cnt = cnt*2;
    printf("Escritor %d: modificou contador para %d\n",(*((int *)wno)),cnt);
    sem_post(&escr);

}

// Função leitor
void *leitor(void *rno) {
    pthread_mutex_lock(&mutex);
    numleit++;
    if(numleit == 1) {
        sem_wait(&escr); // Se o primeiro id for leitor, então o escritor será adormecido.
    }
    pthread_mutex_unlock(&mutex);
    // Leitura
    printf("Leitor %d: Leu contador como %d\n",(*((int *)rno)),cnt);

    pthread_mutex_lock(&mutex);
    numleit--;
    if(numleit == 0) {
        sem_post(&escr); // Se esse for o último leitor, irá acordar o escritor.
    }
    pthread_mutex_unlock(&mutex);
}
