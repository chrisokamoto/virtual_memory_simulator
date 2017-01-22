#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pagina.h"
#include "fifo.h"
#include "lru.h"
#include "lfu.h"

int main(int argc, char *argv[]){
    FILE *entrada, *saida;
    int instancias, i;
    Pagina p;

    entrada = fopen(argv[1], "r");
    if (entrada == NULL){
        printf("Erro ao abrir o arquivo de entrada. \n");
        exit(1);
    }

    saida = fopen(argv[2], "wr");
    if (saida == NULL){
        printf("Erro ao abrir o arquivo de saída. \n");
        exit(1);
    }

    fscanf(entrada, "%d", &instancias);
    while(instancias > 0){

        // Leitura dos dados do arquivo de entrada
        fscanf(entrada, "%d", &p.tam_mem);
        fscanf(entrada, "%d", &p.tam_pag);
        fscanf(entrada, "%d", &p.acessos);

        p.pos_mem = (int*) malloc(p.acessos * sizeof(int));
        for(i=0; i<p.acessos; i++){
            fscanf(entrada, "%d", &p.pos_mem[i]);
        }

        // Chamada para os algoritmos das políticas de substituição de páginas.
        fifo(&p, saida);
        lru(&p, saida);
        lfu(&p, saida);

        instancias--;
    }

    fclose(entrada);
    fclose(saida);

    return 0;
}
