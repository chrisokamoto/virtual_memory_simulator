#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fifo.h"

void FFVazia(TipoFila *Fila)
{ Fila->Frente = (TipoApontador) malloc(sizeof(TipoCelula));
  Fila->Tras = Fila->Frente;
  Fila->Frente->Prox = NULL;
}

int Vazia(TipoFila Fila)
{ return (Fila.Frente == Fila.Tras); }

void Enfileira(TipoItem x, TipoFila *Fila)
{ Fila->Tras->Prox = (TipoApontador) malloc(sizeof(TipoCelula));
  Fila->Tras = Fila->Tras->Prox;
  Fila->Tras->Item = x;
  Fila->Tras->Prox = NULL;
}

void Desenfileira(TipoFila *Fila, TipoItem *Item)
{ TipoApontador q;
  if (Vazia(*Fila)) { printf("Erro fila esta vazia\n"); return; }
  q = Fila->Frente;
  Fila->Frente = Fila->Frente->Prox;
  *Item = Fila->Frente->Item;
  free(q);
}

int Pesquisa(TipoFila *Fila, int pag){
    TipoApontador q;
    int miss=1; //se miss = 1, não encontrou página na fila.

    q = Fila->Frente;
    q=q->Prox;

    while(q != NULL){
        if(q->Item.Chave == pag)
            miss = 0;
        q = q->Prox;
    }

    return miss;
}

void Imprime(TipoFila Fila)
{ TipoApontador Aux;
  Aux = Fila.Frente->Prox;
  while (Aux != NULL)
    { printf("%d\n", Aux->Item.Chave);
      Aux = Aux->Prox;
    }
}

// Função que transforma um número inteiro para binário.
tlista *insereinicio(tlista *plst, int pn)
{
     tlista *aux;

     aux = (tlista*)malloc(sizeof(tlista));
     aux -> n = pn;
     aux -> prox = plst;

     return aux;
}

// Função que exibe na tela o número binário passado como parâmetro.
void exibelista(tlista *plst)
{
     tlista *aux = plst;
     if (aux == NULL)

        printf("Lista vazia\n");
     else
         while (aux != NULL)
        {
               printf("%d", aux -> n);
               aux = (tlista*)aux -> prox;
        }
}

// Libera espaço de memória utilizado para armazenar o número binário.
tlista* destruir(tlista *list){
    tlista *aux;
    aux = list;

    while (aux != NULL){
        if(list->prox == NULL)
            aux = NULL;

        aux = (tlista*)list->prox;
        free(list);
        list = aux;
    }

    return list;
}

// Retonar a quantidade de dígitos do número binário.
int tam_list(tlista *list){
    int tam = 0;

    tlista *aux = list;
     while (aux != NULL)
        {
               tam++;
               aux = (tlista*)aux -> prox;
        }
    return tam;
}

//Função que retorna a página onde o byte está.
int devolve_pag(tlista *list, int bits_pag, int tam_mem){
    int pulo, cont = 0, pos_pag = 0;
    tlista *aux = list;
    tlista *pag;

    pulo = tam_mem - bits_pag;

    pag = (tlista*)malloc(sizeof(tlista));
    while(cont < pulo){
        pag -> n = aux->n;
        aux = (tlista*)aux -> prox;
        pos_pag = pos_pag + pag->n * pow(2, pulo-cont-1); //Transforma o número binário correspondente ao identificador da página onde o byte reside para inteiro.
        pag->prox = (tlista*)malloc(sizeof(tlista));
        pag = pag->prox;
        cont++;
    }

    return pos_pag;
}

void fifo(Pagina *p, FILE *saida){
    int qtd_pags, p_miss=0, bits_pag=0, aux, i, tam, preenche_zeros,  tam_l, pos_pag, cont=0;
    int n, r;
    tlista *lst = NULL;
    TipoFila fila;
    TipoItem item;

    FFVazia(&fila);

    qtd_pags = p->tam_mem / p->tam_pag; //Quantidade de páginas que cabem na memória.

    aux = p->tam_pag;
    while(aux != 1){
        aux = aux / 2;
        bits_pag++;
    }


    for(i=0; i<p->acessos; i++){
        n = p->pos_mem[i]; //p->pos_mem[i] guarda cada acesso.

        if(n == 0){
            r = n%2;
            lst = insereinicio(lst, r);
            n = n / 2;
        }

        while(n > 0){
            r = n%2;
            lst = insereinicio(lst, r);
            n = n / 2;
        }

        tam = tam_list(lst);

        preenche_zeros = p->tam_mem - tam;
        // Completa bits à esquerda do número binário com zeros.
        while(preenche_zeros > 0){
            lst = insereinicio(lst, 0);
            preenche_zeros--;
        }

        tam_l = tam_list(lst);
        pos_pag = devolve_pag(lst, bits_pag, tam_l);

        //Se fila está vazia, enfileira a primeira página.
        if(i==0){
            item.Chave = pos_pag;
            Enfileira(item, &fila);
            cont++;
            p_miss++;
        }
        else{
            if(cont < qtd_pags){
                // Se a fila ainda não está cheia e ocorreu um miss, apenas insere a página.
                if(Pesquisa(&fila, pos_pag) == 1){ //miss
                    p_miss++;
                    item.Chave = pos_pag;
                    Enfileira(item, &fila);
                    cont++;
                }
            }
            else{
                //Se fila está cheia e ocorreu um miss, desenfileira a primeira página que entrou na fila e enfileira a página atual.
                if(Pesquisa(&fila, pos_pag) == 1){ //miss
                    p_miss++;
                    Desenfileira(&fila, &item);
                    item.Chave = pos_pag;
                    Enfileira(item, &fila);
                }
            }
        }

        lst = destruir(lst);

    }

    fprintf(saida, "%d ", p_miss);

}
