# Makefile para o Mergesort.
#
# Author: João Victor Mansano e Marcelo Dos Santos Da Boa Morte.
#
# Nome do programa
MAIN=mergesort
# Compilador.
CC=gcc
#
# Adição de uma flag para emitir warnings.
CCFLAGS=-Wall 
#
# Bibliotecas adicionais.
LIBS=-pthread 
# 
# Arquivos com conteúdos.
SOURCES=functions.c mergesort.c
#
# Aqui, dizemos para transformar arquivos.c para ".o".
OBJECTS=$(SOURCES:.c=.o)
#
# Diretório aonde o programa será criado.
PROGRAMDIR=.
#
# Dependências do Compilador.
all: $(MAIN)

$(MAIN): $(OBJECTS)
	$(CC) $(LIBS) $(OBJECTS) -o $(PROGRAMDIR)/$(MAIN)

# For each .c file, compile according to the following rule
# and generate .o file.
# $<: Nome do pré-requisito da regra (a .c file).
# $@: O nome do arquivo em que a regra é direcionada. (a .o file).
.c.o: 
	$(CC) $(CCFLAGS) $(INCLUDES) $(LIBS) -c $<  -o $@

# Limpeza:
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(PROGRAMDIR)/$(MAIN)