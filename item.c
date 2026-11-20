#include "item.h"

/* Stampa un item */
void stampaitem(item i)
{
    printf("%d %s %d %d %d %d %d\n", i.identificativo, i.descrizione, i.budget, i.categoria, i.ISEE,
            i.data_inizio, i.data_fine);
}

/* Calcola l'indice di preferenza di due item */
int IndicePreferenza(item e, item p)
{
    int ip;
    ip = ((4-abs(e.categoria-p.categoria))*100) + ((99-abs(e.ISEE-p.ISEE))*4) +
         (364-abs(e.data_inizio-p.data_inizio)) + (364-abs(e.data_fine-p.data_fine));

    return ip;
}