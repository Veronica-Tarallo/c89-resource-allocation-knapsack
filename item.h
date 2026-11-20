#ifndef __item_h
#define __item_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_LENGTH 256

#define boolean int
#define TRUE  1
#define FALSE 0

/* Definizione delle strutture */
typedef struct item item;
struct item
{
  int identificativo;     /* numero intero */
  char descrizione[51];   /* stringa alfanumerica composta da una singola parola */
  int budget;             /* è espresso in migliaia di euro */
  int categoria;          /* numero intero fra 1 a 5 */
  int ISEE;               /* numero intero fra 1 e 100*/
  int data_inizio;        /* numero intero compreso fra 1 e 365 */
  int data_fine;          /* numero intero compreso fra 1 e 365 */
  int entefinanziatore;   /* Se il progetto è finanziato questo campo contiene il codice identificativo dell'ente finanziatore */
};

static const struct item NullItem;

/* Stampa un item */
void stampaitem(item i);

/* Calcola l'indice di preferenza di due item */
int IndicePreferenza(item Ente, item Progetto);

#endif