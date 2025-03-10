#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main (int argc, char *argv[]){

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int i=0;
    int j=0;

    int num_threads = atoi(argv[1]); //Número de threads passados.

    int num_arq = argc - 4; //Arquivos tirando o nome, threads e a saida e -o.

    if(argc < 5){
        printf("Dados passados na linha de comando não atingem o esperado!\n");
        exit(EXIT_FAILURE);
    }

    int num_threads_leitura = Verificar_threads_leitura(num_threads, num_arq); //Verificação referente à qtd_Arq x qtd_threads leitura.

    int inicio_arq_thread = 0; //Variavel para utilizar na funçao Passa_Valores, referente ao inicio do vetor de arq (função Vetor_Arq).

    char** vetor_arq = Vetor_arq(argv,num_arq); //Passar os arquivos para outro vetor dinamicamente.

    int arq_lei_thread = num_arq/num_threads_leitura; //Quantidade arquivo de leitura.
    int sobra_arq = num_arq % num_threads_leitura; // Resto da divisao (se tiver).

    pthread_t* ids_threads = malloc(num_threads_leitura * sizeof(pthread_t)); //Criar id das threads.
    criar_threads* thread = malloc(num_threads_leitura * sizeof(criar_threads)); //Criar a thread mesmo.

    if(ids_threads == NULL){
        printf("Falha na criacao dos id das threads!\n");
        return 1;
    }

    if(thread == NULL){
        printf("Falha na criacao dinamica das threads!\n");
        return 1;
    }

    Passa_valores(vetor_arq,num_threads_leitura,inicio_arq_thread, arq_lei_thread, sobra_arq, thread);

    Criar_thread(thread, ids_threads, num_threads_leitura);

    for (i = 0; i < num_threads_leitura; i++) {
    pthread_join(ids_threads[i], NULL); // Espera que a thread i termine.
    }

    //Abaixo, inicia-se a tratativa de liberação de memória alocada dos nomes dos arquivos a serem lidos.

    for (i = 0; i < num_threads_leitura; i++) { //Liberação de memória para quantidade arquivos e os nomes dos arquivos.
        for (j = 0; j < thread[i].qtd_arq; j++) {
            free(thread[i].arquivos[j]); // Libera cada arquivo armazenado.
        }
        free(thread[i].arquivos); // Libera o vetor criado com quantidade de arquivos.
    }

    //Tratativa encerrada!

    int qtd_int_total = 0; // Quantidade para saber o total de inteiros que existe em todos os arquivos.

    for(i=0;i<num_threads_leitura;i++){ //Alocação de memória para o vetor que receberá o conteúdo das threads.
        qtd_int_total += thread[i].qtd_inteiros_thread;
    }

    int* inteiros_lidos = malloc(qtd_int_total*sizeof(int));
        if(inteiros_lidos == NULL){
        printf("Falha na criacao dos inteiros lidos!\n");
        exit(EXIT_FAILURE);
    }

    Transferencia_int(thread, inteiros_lidos, num_threads_leitura); //Transfere os inteiros. do struct thread para o vetor de armazenamento.        

    free(vetor_arq); // Libera o vetor de arquivos.
    free(ids_threads); // Libera os IDs das threads.
    free(thread); // Libera a estrutura de threads.

    //Tratativa para caso o número de inteiros for menor que o número de threads passado.
    if(qtd_int_total <= num_threads){
        num_threads = qtd_int_total; //O número de threads será igual ao número de inteiros no total.
    }

    pthread_t* ids_threads_org = malloc(num_threads*sizeof(pthread_t)); //Aloca o id das threads organizacionais.
    if(ids_threads_org == NULL){
        printf("Erro ao alocar a memória dos ids_threads!\n");
        exit(EXIT_FAILURE);
    } 

    criar_threads_org* thread_org = malloc(num_threads * sizeof(criar_threads_org)); //Criação do struct que conterá dados das
    if(thread_org == NULL){                                                          //  threads oganizacionais (thread_org).
        printf("Erro ao alocar a memória das threads_org!\n");
        exit(EXIT_FAILURE);
    }  

    Separa_qtd_int_thread(num_threads, qtd_int_total, thread_org); //Separa quantos inteiros cada thread_org deverá organizar.

    Aloca_inteiros_thread_org(thread_org, num_threads); //Aloca dinamicamente o vetor de inteiros que cada thread_org possui.

    Passa_int_thread_org(num_threads, thread_org, inteiros_lidos); //Procedimento que transfere os ints_lidos para as threads_org.

    Criar_threads_organizadoras(thread_org, ids_threads_org, num_threads); //Criação das threads organizadoras.

    for (i = 0; i < num_threads; i++) { //Laço para aguardar a conclusão de todas as threads antes da main continuar.
    pthread_join(ids_threads_org[i], NULL); 
    }

    Imprime_tempo_exec(thread_org, num_threads); //Impressão do tempo de execução de cada thread_organizadora.

    int* inteiros_organizados = Aloca_int_final(qtd_int_total); //Alocação dinâmica do vetor de inteiros organizados.

    Escolhe_menor(thread_org, num_threads, inteiros_organizados, qtd_int_total); //Transferência dos inteiros organizados das threads_org.

    for(i=0;i<num_threads;i++){
        free(thread_org[i].int_thread);
    }

    free(ids_threads_org); // Liberação dos ids das threads.
    free(thread_org); //Liberação das estruturas das threads.
    free(inteiros_lidos); //Liberação do vetor de inteiros lidos.

    Imprime_vetor_final(inteiros_organizados, qtd_int_total, argc, argv); //Por fim, criação do arq. final e escrita nele.

    free(inteiros_organizados);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double total_time = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tempo total de execucao: %lf.\n", total_time); //Acima, é mostrado o tempo total. No printf ao lado exemplificamos ele.

    return 0;
}