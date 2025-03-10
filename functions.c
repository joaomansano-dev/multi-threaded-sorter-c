#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

int Verificar_threads_leitura(int num_threads, int num_arq ){ //Função para verificar se é necessário utilizar todas as threads
    if(num_arq<=num_threads){                                 // ou não com base na quantidade de arquivos passados.
        num_threads = num_arq;
    }
    return num_threads; 
}

char** Vetor_arq (char* argv[], int num_arq){ //Copia os arquivos em vetores de 0 a N. 
    int inicio_arq = 2, i=0;
    char** vetor_arq = malloc(num_arq*sizeof(char*));
        if(vetor_arq == NULL){
            printf("Erro ao alocar a memória!\n");
            return NULL;
        }

    for(i=0;i<num_arq;i++){
        int tamanho_string_arq = 0;
        tamanho_string_arq += strlen(argv[inicio_arq]) + 1; //Aqui, achamos o tamanho da string de caracteres mais o /0.
        vetor_arq[i] = malloc(tamanho_string_arq * sizeof(char));
            if(vetor_arq[i] == NULL){
                printf("Erro ao alocar a memória!\n");
                return NULL;
            }

        strcpy(vetor_arq[i],argv[inicio_arq]); //Finalmente, é copiado o nome dos arquivos da linha de comando para nosso vetor final.
        inicio_arq += 1;
    }
    return vetor_arq;
}

void Passa_valores(char** vetor_arq,int num_threads_leitura,int inicio_arq_thread,int arq_lei_thread,int sobra_arq,criar_threads* thread){
    int i=0;                                //Neste procedimento, basicamente é passado a quantidade de arquivos que cada thread terá.

    for(i=0;i<num_threads_leitura;i++){
        int arq_per_thread = arq_lei_thread;

        if(i==num_threads_leitura-1){
            arq_per_thread += sobra_arq;    //Caso o número de arquivos seja ímpar, a última thread ficará responsável com a     
        }                                   // leitura do resto da divisão dos arquivos pelo num_threads_leitura.

        thread[i].qtd_arq = arq_per_thread; //Quantidade de arquivo que cada thread deve ler.

        thread[i].arquivos = malloc(arq_per_thread*sizeof(char*)); //aloca a quantidade de arquivos para a thread.
            if(thread[i].arquivos == NULL){
                printf("Erro ao alocar a memória!\n");
                exit(EXIT_FAILURE);
            }

        int j=0;

        for(j=0; j< arq_per_thread; j++){
            int tamanho_string_arq = 0;
            tamanho_string_arq = strlen(vetor_arq[inicio_arq_thread])+1; // Verificar tamanho da string para colocar em arquivos[j].
            thread[i].arquivos[j] = malloc(tamanho_string_arq*sizeof(char));
                if(thread[i].arquivos[j] == NULL){
                    printf("Erro ao alocar a memória!\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(thread[i].arquivos[j], vetor_arq[inicio_arq_thread]); //Atribuição do nome do arquivo para thread[i].arquivos[j].
                inicio_arq_thread += 1; //Aumentará conforme loop até que a qtd de arq. per thread seja feita.
        }
    }
   
}

void* Ler_arq (void* arg){
    int i=0; //Variável local para loop.
    int j=0; //Outra variável local para loop,de forma a não misturar.
    int qtd_total_inteiros = 0; //Variável inicial para realocação de memória após ciclos de leitura dos arquivos de cada thread.
    
    criar_threads* thread = (criar_threads*)arg; //Passagem do struct de cada thread.

    for(i=0;i<thread->qtd_arq;i++){
        FILE* arquivo;
        arquivo = fopen(thread->arquivos[i], "r"); //Acesso a cada arquivo armazenado em sua respectiva thread.
            if(arquivo == NULL){
                printf("Erro ao abrir o arquivo!\n");
                exit(EXIT_FAILURE);
            }

    int count=0;
    
    while (1){ 
        int t; // Variável temporária.
        if(fscanf(arquivo, "%d", &t) == 1){ //Irá ler o arquivo desde que todo num seja inteiro. 
            count += 1;                     //Ademais, se n for inteiro, ele retornará 0 e irá parar.
        }else{
            break;
        }
    }

    thread->qtd_inteiros_thread += count; //Atribuição de count do loop acima para a variável que guardara a quantidade de cada thread.
    
    if(i==0){ //Caso seja o 1° arq da thread, alocará uma memória nova.
        thread->inteiros_dos_arquivos = malloc(count * sizeof(int));
            if(thread->inteiros_dos_arquivos == NULL){
                printf("Erro ao alocar a memória!\n");
                exit(EXIT_FAILURE);
            }
    } else { // Caso seja posterior ao 1°, realocará as memórias.
        int* temp = realloc(thread->inteiros_dos_arquivos, (qtd_total_inteiros + count) * sizeof(int));
            if (temp == NULL) {
            printf("Erro ao realocar a memória!\n");
            exit(EXIT_FAILURE);
    }
    thread->inteiros_dos_arquivos = temp;
    }

    rewind(arquivo); //Volta para o inicio do arquivo.


    for(j=0;j<count; j++){
        fscanf(arquivo, "%d", &thread->inteiros_dos_arquivos[qtd_total_inteiros+j]); //Armazenamento dos arquivos lidos.
    }                                        //qtd_total_inteiros serve para evitar a reutilização de endereços já utilizados.
    qtd_total_inteiros += count;
    fclose(arquivo);
    }  
return NULL;                                                           
}    

void Criar_thread (criar_threads* thread, pthread_t* ids_thread, int num_threads_leitura){ //Criação das threads de leitura.
    int i=0, rc;

    for(i=0;i<num_threads_leitura;i++){
        rc =  pthread_create(&ids_thread[i],NULL,Ler_arq,&thread[i]);
    
        if(rc){
            fprintf(stderr, "ERROR: pthread_create() retorna %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }
}

void Transferencia_int (criar_threads* thread, int* inteiros_lidos, int num_threads_leitura){
    int i=0, j=0;                             //Transfere os int. de cada thread para o vetor final.
    int qtd_total_inteiros = 0;               //Variável temporária para não sobrescrever endereços já utilizados.
    for(i=0;i<num_threads_leitura;i++){
        for(j=0;j<thread[i].qtd_inteiros_thread;j++){
            inteiros_lidos[qtd_total_inteiros] = thread[i].inteiros_dos_arquivos[j];
            qtd_total_inteiros++;
        }
    }
}

// A partir de agora, o enfoque é na utilização do vetor e das threads organizadoras (ou thread_org).

void Separa_qtd_int_thread(int num_threads, int qtd_int_total, criar_threads_org* thread_org){ //Separará a qtd de inteiros por thread.
    int resto = 0, qtd_int_per_thread = 0;
    int i=0;
    qtd_int_per_thread = qtd_int_total/num_threads; //Cálculo inteiros por thread.
    resto = qtd_int_total % num_threads;           //Cálculo do resto, caso tenha.

    for(i=0;i<num_threads;i++){ 
        thread_org[i].qtd_int_org = qtd_int_per_thread;       
        if(i < resto){ // Atribuição do resto para cada thread, simulando um round robin e evitando sobrecarregar uma thread_org.
            thread_org[i].qtd_int_org += 1;
        } 
    }
}


void Aloca_inteiros_thread_org (criar_threads_org* thread_org, int num_threads){ //Alocação dinâmica do vetor de inteiros que cada
    int i=0;                                                                     // thread_org possui.

    for(i=0;i<num_threads;i++){
        thread_org[i].int_thread = malloc(thread_org[i].qtd_int_org * sizeof(int));
        if(thread_org[i].int_thread == NULL){
            exit(EXIT_FAILURE);
        }
    }
        
}

void Passa_int_thread_org (int num_threads, criar_threads_org* thread_org, int* inteiros_lidos){
    int i=0, j = 0;
    int endereco = 0; //variável temporária para evitar colocar endereço inválido.

    for(i=0;i<num_threads;i++){
            for(j=0;j<thread_org[i].qtd_int_org;j++){
                thread_org[i].int_thread[j] = inteiros_lidos[endereco + j];
            }
        endereco += thread_org[i].qtd_int_org; // endereco serve para percorremos o vetor "inteiros_lidos" de forma eficaz.
    }
}

//A partir de agora, será feito as funções e procedimentos para utilização do quicksort na ordenação dos inteiros que cada thread
// possui. Vale ressaltar que esse procedimento pertence ao repositório do Marcos Castro. Segue link:
//  https://gist.github.com/marcoscastro/1dd65900cc7b188e1ab9 

void Troca(int vet[], int i, int j) { //Faz a troca durante o quicksort.
    int aux = vet[i];
    vet[i] = vet[j];
    vet[j] = aux;
}

int Particiona(int vet[], int inicio, int fim) { //Particiona o vetor.
    int pivo = vet[fim]; //Pivo sempre será o fim do vetor.
    int pivo_indice = inicio;
    
    for (int i = inicio; i < fim; i++) {
        if (vet[i] <= pivo) {
            Troca(vet, i, pivo_indice);
            pivo_indice++;
        }
    }
    Troca(vet, pivo_indice, fim);
    return pivo_indice;
}

int Particiona_random(int vet[], int inicio, int fim) {       //Evita o pior caso do quicksort, que é a divisão desordenada dele.
    int pivo_indice = (rand() % (fim - inicio + 1)) + inicio;
    Troca(vet, pivo_indice, fim);
    return Particiona(vet, inicio, fim);
}

void Quick_sort(int vet[], int inicio, int fim) {
    if (inicio < fim) {
        int pivo_indice = Particiona_random(vet, inicio, fim);
        Quick_sort(vet, inicio, pivo_indice - 1);
        Quick_sort(vet, pivo_indice + 1, fim);
    }
}

void* Quick_sort_final(void *arg) {
    criar_threads_org *thread_org = (criar_threads_org*) arg; //Garantimos o acesso do procedimento para o struct thread_org.
    
    struct timespec inicio, fim;                              //Inicia a medição do tempo.
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // realiza o quicksort no intervalo atribuído a essa thread
    Quick_sort(thread_org->int_thread, 0, thread_org->qtd_int_org-1);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    thread_org->tempo_de_exec = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9; //Finaliza a medição do tempo.
    
    pthread_exit(NULL);
}

void Criar_threads_organizadoras(criar_threads_org* thread_org, pthread_t* ids_threads_org, int num_threads){ //Cria as thread_org.
    int i=0, rc;

    for(i=0;i<num_threads;i++){
        rc =  pthread_create(&ids_threads_org[i],NULL,Quick_sort_final,&thread_org[i]);
    
        if(rc){
            fprintf(stderr, "ERROR: pthread_create() retorna %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }
}

int* Aloca_int_final(int qtd_int_total){                            //Aloca o vetor final aonde os inteiros estarão em ordem                                                      //crescente.

    int* inteiros_finais = malloc(qtd_int_total*sizeof(int));
        if(inteiros_finais == NULL){
            printf("Erro ao alocar os inteiros organizados!\n");
            exit(EXIT_FAILURE);
        }
        return inteiros_finais;
}

void Escolhe_menor(criar_threads_org *thread_org, int num_threads, int* inteiros_organizados, int qtd_int_total) {
    
    int i;

    int* aux_temp = malloc(num_threads * sizeof(int)); // Variável que controla ints já visto na thread_org X.
    if (aux_temp == NULL) {
        printf("Erro ao alocar a memória dos índices temporários!\n");
        exit(EXIT_FAILURE);
    }

    for(i=0;i<num_threads;i++){
        aux_temp[i] = 0;
    }

    for (int i = 0; i < qtd_int_total; i++) {
        int m_indice = -1; //Inicializado com -1, o que significa que não achamos o endereço correto.
    

        for (int j = 0; j < num_threads; j++) {
            if (aux_temp[j] < thread_org[j].qtd_int_org) { // Verifica se existe números a serem pegos.
                if (m_indice == -1 || thread_org[j].int_thread[aux_temp[j]] < thread_org[m_indice].int_thread[aux_temp[m_indice]]){
                    m_indice = j; //O menor indice se tornará j, em que j representa em qual thread_org se encontra o menor
                }                 // inteiro entre todas as threads organizacionais no momento. 
            }
        }                         //aux_temp[j] indica o endereço do vetor que estamos olhando, sendo ele aquele não selecionado
                                  //anteriormente. aux_temp[m_indice] mostra   
       
        inteiros_organizados[i] = thread_org[m_indice].int_thread[aux_temp[m_indice]];
        aux_temp[m_indice]++; // Já usamos uma posição. Partimos para a próxima dentro das posições de inteiros da thread selecionada.
    }

    free(aux_temp);
}

void Imprime_vetor_final(int* inteiros_organizados, int qtd_int_total, int argc, char *argv[]){ 
    FILE* arq = fopen(argv[argc-1], "w"); //Abertura de um arquivo com o nome passado na linha de comando.
        if(arq == NULL){
            printf("Erro ao criar o arquivo!\n");
            exit(EXIT_FAILURE);
        }

int i;

    for(i=0;i<qtd_int_total;i++){
        fprintf(arq, "%d\n", inteiros_organizados[i]); //Imprimimos um arq. por linha.
    }
    fclose(arq);
}

void Imprime_tempo_exec (criar_threads_org* thread_org, int num_threads){ //Procedimento para imprimir o tempo de exec. de cada
    int i=0;                                                              // thread_organizadora.  

    for(i=0; i<num_threads; i++){
        printf("Tempo de execucao do Thread %d: %f segundos.\n", i, thread_org[i].tempo_de_exec);
    }
}