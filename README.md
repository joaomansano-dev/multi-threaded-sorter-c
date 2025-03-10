# Ordenador Multi Threaded em C
Projeto desenvolvido para ordenação de números inteiros aleatórios em ordem crescente, utilizando **threads** e um algoritmo de **quicksort**. Cada thread ordenará seu subconjunto de inteiros. Posteriormente, ocorrerá uma junção dos inteiros ordenados em um arquivo de saída.

Para utilizar este código, recomendo a utilização do **WSL** ou sistema operacional **Linux**. Além disso, ao rodar esse código, a entrada no terminal deve seguir a seguinte ordem:

./nome_arq_executável num_threads arq1.dat arq2.dat ...arqN.dat -o nome_arq_saida.dat

Ou seja, é possível utilizar **N threads** para ordenar **X números inteiros**, além de ver o tempo de execução de cada thread e o tempo total do programa principal.

Ademais, adicionei também um código _(geraarq.c)_ que é capaz de gerar arquivos com números aleatório a serem utilizados durante o código principal. Caso o usuário não possua os arquivos numéricos, recomendo rodar este código antes da execução do programa principal. 

Para rodar o código principal, basta utilizar o comando "make", pois ele foi dividido em:
**Arquivo principal:** mergesort.c --- **Arquivo de funções:** functions.c --- **Cabeçalho:** functions.h --- **Makefile**

# Multi Threaded Sorter - C
This project was developed to sort random integers in ascending order using threads and a quicksort algorithm. Each thread will sort its subset of integers. After that, the sorted integers will be merged into an output file.

To use this code, I recommend using WSL or a Linux-based operating system. Furthermore, when running this program, the command in the terminal must follow this format:

./nome_arq_executável num_threads arq1.dat arq2.dat ...arqN.dat -o nome_arq_saida.dat

In other words, you can use N threads to sort X integers, in addition to viewing the execution time of each thread and the total execution time of the main program.

Additionally, I have also included a script  _(geraarq.c)_ capable of generating files containing random numbers to be used in the main program. If the user does not have numeric data files, I recommend running this script before executing the main program.

To compile and run the main program, simply use the _make_ command, as the project is divided into the following files
**Main archive:** mergesort.c --- **Function archive:** functions.c --- **Header:** functions.h --- **Makefile**
