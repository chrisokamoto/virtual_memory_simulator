#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>

#ifndef PAGINA_H
	#include "pagina.h"
#endif

typedef struct TipoCelula *TipoApontador;

typedef int TipoChave;

typedef struct TipoItem {
  TipoChave Chave;
} TipoItem;

typedef struct TipoCelula {
  TipoItem Item;
  TipoApontador Prox;
} TipoCelula;

typedef struct TipoFila {
  TipoApontador Frente, Tras;
} TipoFila;

typedef struct tlista *TipoApontadorLista;

typedef struct tlista
{
          int n;
          TipoApontadorLista prox;
} tlista;


//Cabeçalhos das funções implementadas em fifo.c

void FFVazia(TipoFila*);
int Vazia(TipoFila);
void Enfileira(TipoItem, TipoFila*);
void Desenfileira(TipoFila*, TipoItem*);
int Pesquisa(TipoFila *, int);
void Imprime(TipoFila);
// Função que transforma um número inteiro para binário.
tlista *insereinicio(tlista *, int);
// Função que exibe na tela o número binário passado como parâmetro.
void exibelista(tlista *);
// Libera espaço de memória utilizado para armazenar o número binário.
tlista* destruir(tlista *);
// Retonar a quantidade de dígitos do número binário.
int tam_list(tlista *);
//Função que retorna a página onde o byte está.
int devolve_pag(tlista *, int, int);
void fifo(Pagina *, FILE *);


