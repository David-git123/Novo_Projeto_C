# Makefile para compilar apenas davi.c

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS =

SRC = davi.c
OUT = davi

# Adaptações para macOS (caso precise de bibliotecas específicas, ajuste aqui)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # Exemplo de flags extras para macOS (modifique se necessário)
    LDFLAGS +=
endif

.PHONY: all clean run

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
