#include <stdio.h>
#include <stdlib.h>

#define PAGINA_H

typedef struct Pagina{
    int tam_mem, tam_pag, acessos;
    int *pos_mem;
}Pagina;


