
# Parametros de Ambiente

CC = gcc
 
CFLAGS = -Wall


# Parametros de Projeto

# TODO: altere a lista de forma que contenha todos os arquivos .c 
#       do seu projeto, que deve estar no diretório src/emulador

SRC_MONTADOR = main.c montador.c lista.c

OBJS_MONTADOR = $(SRC_MONTADOR:%.c=build/montador/%.o)


SRC_LIGADOR = main.c ligador.c

OBJS_LIGADOR = $(SRC_LIGADOR:%.c=build/ligador/%.o)

# Executaveis

all: bin/ligador bin/montador

bin/ligador: $(OBJS_LIGADOR)
	@echo "+ Compilando programa \"$@\""
	@$(CC) $(CFLAGS) $(OBJS_LIGADOR) -o bin/ligador

bin/montador: $(OBJS_MONTADOR)
	@echo "+ Compilando programa \"$@\""
	@$(CC) $(CFLAGS) $(OBJS_MONTADOR) -o bin/montador

# Objetos

build/%.o: src/%.c src/%.h
	@echo "- Compilando objeto \"$@\""
	@$(CC) -c $(CFLAGS) $< -o $@

build/%.o: src/%.c
	@echo "- Compilando objeto \"$@\""
	@$(CC) -c $(CFLAGS) $< -o $@


# Auxiliares

clean:
	rm -f bin/* $(OBJS_LIGADOR)

