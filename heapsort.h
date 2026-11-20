#ifndef __heapsort_h
#define __heapsort_h

#include <stdio.h>
#include <stdlib.h>

#define boolean int
#define TRUE  1
#define FALSE 0
#include "alloc.h"

void Scambia (int *pa, int *pb);

/* Ordina il vettore V di interi di lunghezza n con l'algoritmo HeapSort */
void HeapSort (vint V, int n);

/* Crea un heap a partire dal vettore V[0,n] */
void creaheap(vint V,int n);

/* Ripara l'heap contenuto nel vettore V[0,n] che ha un errore in posizione 0 */
void aggiornaheap(vint V,int n, int i);

#endif