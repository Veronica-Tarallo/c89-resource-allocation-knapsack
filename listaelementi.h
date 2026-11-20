#ifndef __lista_h
#define __lista_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"


/* Definizione delle strutture */
typedef struct _elemento elemento;
typedef elemento *listaelementi;
typedef elemento *posizione;

struct _elemento {
  item item;
  posizione succ;
  posizione pred;
};

#define NO_ELEMENT NULL
#define EMPTY_LIST NULL


listaelementi crealista ();

void distruggelista (listaelementi *pL);

item leggelista (listaelementi L, posizione p);

listaelementi scrivelista (listaelementi L, posizione p, item i);

boolean listavuota (listaelementi L);

posizione primolista (listaelementi L);

posizione ultimolista (listaelementi L);

posizione succlista (listaelementi L, posizione p);

posizione predlista (listaelementi L, posizione p);

boolean finelista (listaelementi L, posizione p);

listaelementi inslista (listaelementi L, posizione p, item i);

listaelementi canclista (listaelementi L, posizione *pp);

/* Copia nella lista copia la lista L */
void copialista(listaelementi L, listaelementi *copia);

/* Restituisce il numero di elementi presenti nella lista L */
int lunghezzalista(listaelementi L);

/* Restitusce il budget dell'item in posizione n-esima nella lista L */
int TrovaBudget(listaelementi L, int n);

/* Restitusce l'identificativo dell'item in posizione n-esima nella lista L */
int TrovaIdentificativo(listaelementi L, int n);

/* Restituisce la posizione dell'item con numero identificativo n nella lista L se questo è presente,
  altrimenti restituisce NO_ELEMENT */
posizione TrovaItem(int n, listaelementi L);

/* Restituisce la distanza di un progetto da una lista di progetti, ovvero la somma delle distanze
  di Progetto da ogni progetto nella lista ProgettiSelezionati */
int IndiceDiversita(listaelementi ProgettiSelezionati, item Progetto);

/* Restituisce la diversità di un insieme di progetti, ovvero la somma di tutte le distanze a coppie 
  tra i progetti dell'insieme */
int Diversita(listaelementi L);


#endif