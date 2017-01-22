#include "lfu.h"
#include "fifo.h"

//Retorna a página que foi menos acessada.
TipoItemL ItemMenosAcessado(TipoListaL *Lista){
    int menor = INF, menor_ind = INF;
    TipoApontadorL q;
    TipoItemL aux;

    q = Lista->Primeiro;
    q=q->Prox;

    while(q != NULL){
        // Pega a página menos acessada.
        if(menor > q->Item.cont){
            menor = q->Item.cont;
            aux = q->Item;
        }
        q = q->Prox;

    }
    q = Lista->Primeiro;
    q=q->Prox;

    // Em caso de empate, pega a página de menor índice.
    while(q != NULL){
        if(menor == q->Item.cont){
            if(menor_ind >= q->Item.Chave){
                menor_ind = q->Item.Chave;
                aux = q->Item;
            }
        }
        q = q->Prox;

    }

    return aux;
}

// Pesquisa se página a ser acessada já se encontra presente na lista.
int PesquisaL(TipoListaL *Lista, int pag){
    TipoApontadorL q;
    int hit=0; //se hit = 0, não encontrou página na lista.

    q = Lista->Primeiro;
    q=q->Prox;

    while(q != NULL){
        if(q->Item.Chave == pag){
            q->Item.cont++;
            hit = 1;
            break;
        }
        q = q->Prox;

    }

    return hit;
}

void FLVazia(TipoListaL *Lista)
{ Lista -> Primeiro = (TipoApontadorL) malloc(sizeof(TipoCelulaL));
  Lista -> Ultimo = Lista -> Primeiro;
  Lista -> Primeiro -> Prox = NULL;
}

int LVazia(TipoListaL Lista)
{ return (Lista.Primeiro == Lista.Ultimo);
}

void Insere(TipoItemL x, TipoListaL *Lista)
{ Lista -> Ultimo -> Prox = (TipoApontadorL) malloc(sizeof(TipoCelulaL));
  Lista -> Ultimo = Lista -> Ultimo -> Prox;
  x.cont = 0;
  Lista -> Ultimo -> Item = x;
  Lista -> Ultimo -> Prox = NULL;
}

void Retira(TipoItemL p, TipoListaL *Lista)
{ /*  ---   Obs.: o item a ser retirado e  o seguinte ao apontado por  p --- */
    TipoApontadorL q, aux, ant;
    int cont=0;

    q = Lista->Primeiro;
    q=q->Prox;

    while(q != NULL){

        if(cont == 0){
            ant = Lista -> Primeiro;
        }

        if(q->Item.Chave == p.Chave){
            if (q -> Prox == NULL) {
                Lista -> Ultimo = ant;
                break;
            }
            aux = q->Prox;
            ant->Prox = aux;
            free(q);
            break;
        }
        cont++;
        ant = q;
        q = q->Prox;

    }

}

void ImprimeLista(TipoListaL Lista)
{ TipoApontadorL Aux;
  Aux = Lista.Primeiro -> Prox;
  while (Aux != NULL)
    { printf("Pagina: %d Cont: %d\n", Aux -> Item.Chave, Aux->Item.cont);
      Aux = Aux -> Prox;
    }
}

void lfu(Pagina *p, FILE *saida){
    int qtd_pags, p_miss=0, bits_pag=0, aux, i, tam, preenche_zeros, tam_l, pos_pag, cont=0;
    int n, r;
    tlista *lst = NULL;
    TipoListaL lista;
    TipoItemL item;
    TipoItemL pointer;

    FLVazia(&lista);

    qtd_pags = p->tam_mem / p->tam_pag;  //Quantidade de páginas que cabem na memória.

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

        // Se lista está vazia, insere a primeira página.
        if(i==0){
            item.Chave = pos_pag;
            //item.cont = 0;
            Insere(item, &lista);
            p_miss++;
            cont++;
        }
        else{
            // Se a lista ainda não está cheia e ocorreu um miss, apenas insere a página.
            if(cont < qtd_pags){
                if(PesquisaL(&lista, pos_pag) == 0){ //miss
                    p_miss++;
                    item.Chave = pos_pag;
                    Insere(item, &lista);
                    cont++;
                }

            }
            //Se a lista está cheia e ocorreu um miss, retira a página menos acessada e insere a página atual.
            else{
                if(PesquisaL(&lista, pos_pag) == 0){ //miss
                    p_miss++;
                    pointer = ItemMenosAcessado(&lista);
                    Retira(pointer, &lista);
                    item.Chave = pos_pag;
                    Insere(item, &lista);
                }
            }
        }
        lst = destruir(lst);
    }

    fprintf(saida, "%d\n", p_miss);


}

