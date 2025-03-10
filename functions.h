#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

typedef struct {

    char** arquivos; //Ponteiro para o endereço de um array das strings (o usuario coloca os arquivos).
    int qtd_arq; //Quantidade de arquivos a serem lidos.

    int* inteiros_dos_arquivos; // Vetor que armazenará os inteiros de cada thread.
    int qtd_inteiros_thread; //Variável para saber a quantidade exata de cada inteiro que será armazenado na thread.

} criar_threads;

typedef struct {
    int qtd_int_org; //Quantidade de inteiros a serem organizados.
    int* int_thread; //Local aonde os inteiros serão armazenados.

    int inicio_int;
    int fim_int;

    double tempo_de_exec; //Variável que irá armazenar o tempo de execução da thread_org.

} criar_threads_org; //Criar threads organizadoras.

int Verificar_threads_leitura(int num_threads, int num_arq );

char** Vetor_arq (char* argv[], int num_arq);

void Criar_thread (criar_threads* thread, pthread_t* ids_thread, int num_threads);

void Passa_valores(char** vetor_arq, int num_threads,int inicio_arq_thread, int arq_lei_thread, int sobra_arq, criar_threads* thread);

void* Ler_arq (void* arg);

void Transferencia_int (criar_threads* thread, int* inteiros_lidos, int num_threads);

void Separa_qtd_int_thread(int num_threads, int qtd_int_total, criar_threads_org* thread_org);

void Aloca_inteiros_thread_org (criar_threads_org* thread_org, int num_threads);

void Passa_int_thread_org (int num_threads, criar_threads_org* thread_org, int* inteiros_lidos);

void Criar_threads_organizadoras(criar_threads_org* thread_org, pthread_t* ids_threads, int num_threads);

int* Aloca_int_final(int qtd_int_total);

void Escolhe_menor(criar_threads_org *thread_org, int num_threads, int* inteiros_organizados, int qtd_int_total);

void Imprime_vetor_final(int* inteiros_organizados, int qtd_int_total, int argc, char *argv[]);

void Imprime_tempo_exec (criar_threads_org* thread_org, int num_threads);

#endif