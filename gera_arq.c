#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função que cria arquivos .dat com números aleatórios
void gera_arquivos(int num_arquivos, int *quantidades) {
    for (int i = 0; i < num_arquivos; i++) {
       
        //Nome do arquivo
        char nome_arquivo[20];
        sprintf(nome_arquivo, "arq%d.dat", i + 1);

        //Abre o arquivo para escrita
        FILE *arquivo = fopen(nome_arquivo, "w");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
            exit(EXIT_FAILURE);
        }

        //Escrita de números aleatórios
        for (int j = 0; j < quantidades[i]; j++) {
            int numero_aleatorio = rand() % 1000; // Gera um número aleatório entre 0 e 999
            fprintf(arquivo, "%d\n", numero_aleatorio);
        }

        printf("Arquivo %s criado com %d números aleatórios.\n", nome_arquivo, quantidades[i]);
        fclose(arquivo);
    }
}

int main() {
    int num_arquivos;

    srand(time(NULL));

    //Pede para que o num de arquivos a serem criados seja inserido
    printf("Digite o número de arquivos a serem criados: ");
    scanf("%d", &num_arquivos);

    //Alocação dinâmica da quantidade de nums em cada arquivo
    int *qtd = malloc(num_arquivos * sizeof(int));
    if (qtd == NULL) {
        printf("Erro na alocacao de memória.\n");
        return EXIT_FAILURE;
    }

    //Solicita a quantidade de nums em cada arquivo
    for (int i = 0; i < num_arquivos; i++) {
        printf("Digite a quantidade de números para o arquivo %d: ", i + 1);
        scanf("%d", &qtd[i]);
    }

    //Criação dos inteiros de forma aleatória
    gera_arquivos(num_arquivos, qtd);

    free(qtd);

    return 0;
}
