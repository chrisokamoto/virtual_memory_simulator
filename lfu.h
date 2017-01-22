#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

#ifndef PAGINA_H
	#include "pagina.h"
#endif

typedef int TipoChaveL;

typedef struct {
  int Chave;
  int cont;
} TipoItemL;

typedef struct TipoCelulaL *TipoApontadorL;

typedef struct TipoCelulaL {
  TipoItemL Item;
  TipoApontadorL Prox;
} TipoCelulaL;

typedef struct {
  TipoApontadorL Primeiro, Ultimo;
} TipoListaL;


//Cabeçalhos das funções implementadas em lfu.c

//Retorna a página que foi menos acessada.
TipoItemL ItemMenosAcessado(TipoListaL *);
// Pesquisa se página a ser acessada já se encontra presente na lista.
int PesquisaL(TipoListaL *, int);
void FLVazia(TipoListaL *);
int LVazia(TipoListaL);
void Insere(TipoItemL, TipoListaL *);
void Retira(TipoItemL, TipoListaL *);
void ImprimeLista(TipoListaL);
void lfu(Pagina*, FILE*);
