#include "lru.h"
#include "fifo.h"

int p_miss=0;

void FPVazia(TipoPilha *Pilha)
{ Pilha->Topo = (TipoApontadorPilha) malloc(sizeof(TipoCelulaPilha));
  Pilha->Fundo = Pilha->Topo;
  Pilha->Topo->Prox = NULL;
  Pilha->Tamanho = 0;
}

int PVazia(TipoPilha Pilha)
{ return (Pilha.Topo == Pilha.Fundo); }

void Empilha(TipoItemPilha x, TipoPilha *Pilha)
{ TipoApontadorPilha Aux;
  Aux = (TipoApontadorPilha) malloc(sizeof(TipoCelulaPilha));
  Pilha->Topo->Item = x;
  Aux->Prox = Pilha->Topo;
  Pilha->Topo = Aux;
  Pilha->Tamanho++;
}

void Desempilha(TipoPilha *Pilha, TipoItemPilha *Item)
{ TipoApontadorPilha q;
  if (PVazia(*Pilha)) { printf("Erro: lista vazia\n"); return; }
  q = Pilha->Topo;
  Pilha->Topo = q->Prox;
  *Item = q->Prox->Item;
  free(q);  Pilha->Tamanho--;
}

int Tamanho(TipoPilha Pilha)
{ return (Pilha.Tamanho); }

// Função principal da lru. Verifica se uma página já está presente na pilha. Caso esteja, sobe esta página para o topo da pilha. Caso, ainda não esteja presente, empilha.
TipoPilha jaEmpilhou(TipoItemPilha Item, TipoPilha Pilha, int tam, int pag) {
    TipoItemPilha Copia, aux;
    TipoPilha pilhaAux;

    int i, presente=0;

    FPVazia(&pilhaAux);

    /* Laço que pesquisa se a página a ser acessada já está presente na pilha. Vai desempilando páginas da pilha ao mesmo tempo em que compara se é a página que se quer acessar.
    Ao desempilhar, vai empilhando em uma pilha auxiliar. Quando a pilha estiver vazia, desempilha toda a pilha auxiliar e reempilha na pilha original novamente. Se a página a
    ser acessada foi encontrada na pilha, ela é empilhada novamente, no topo. Caso contrário, ocorreu um miss. */
    for(i=0; i<tam; i++){
        Desempilha(&Pilha, &aux);
        if(aux.Chave == pag){
            presente = 1;
            Copia = Item;
        }
        else {
            Empilha(aux,&pilhaAux);
        }
    }

    while (!PVazia(pilhaAux)){
        Desempilha(&pilhaAux, &aux);
        Empilha(aux,&Pilha);
    }

    if (presente == 1){
        //Se página já estava na pilha, sobe para o topo da pilha.
        Empilha(Copia,&Pilha);
        presente=0;
    }
    else{
        Empilha(Item,&Pilha);
        p_miss++;
    }

    return Pilha;
}

void lru(Pagina *p, FILE *saida){
    int qtd_pags, bits_pag=0, aux, i, tam, preenche_zeros, tam_l, pos_pag;
    int n, r;
    tlista *lst = NULL;
    TipoPilha pilha;
    TipoItemPilha item;

    FPVazia(&pilha);

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
        item.Chave = pos_pag;

        // Se pilha está vazia, empilha a primeira página.
        if(i==0){
            Empilha(item, &pilha);
            p_miss++;
        }
        else{
            /* Se a pilha não está vazia, chama a função de pesquisa. Se a página a ser acessada já está presente na pilha, sobe ela para o topo. Insere, caso contrário,
            retirando a página que reside no fundo da pilha na falta de espaço. */
            if(pilha.Tamanho >= qtd_pags)
                pilha.Tamanho = qtd_pags;
                pilha = jaEmpilhou(item, pilha, pilha.Tamanho, pos_pag);
        }

        lst = destruir(lst);
    }

    fprintf(saida, "%d ", p_miss);
    p_miss = 0;

}
