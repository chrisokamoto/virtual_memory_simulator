#include <stdio.h>
#include <stdlib.h>

#ifndef PAGINA_H
	#include "pagina.h"
#endif

typedef int TipoChavePilha;

typedef struct {
  int Chave;
} TipoItemPilha;

typedef struct TipoCelulaPilha *TipoApontadorPilha;

typedef struct TipoCelulaPilha {
  TipoItemPilha Item;
  TipoApontadorPilha Prox;
} TipoCelulaPilha;

typedef struct {
  TipoApontadorPilha Fundo, Topo;
  int Tamanho;
} TipoPilha;


//Cabeçalhos das funções implementadas em lru.c

void FPVazia(TipoPilha *);
int PVazia(TipoPilha);
void Empilha(TipoItemPilha, TipoPilha *);
void Desempilha(TipoPilha *, TipoItemPilha *);
int Tamanho(TipoPilha);
// Função principal da lru. Verifica se uma página já está presente na pilha. Caso esteja, sobe esta página para o topo da pilha. Caso, ainda não esteja presente, empilha.
TipoPilha jaEmpilhou(TipoItemPilha, TipoPilha, int, int);
void lru(Pagina *, FILE *);
