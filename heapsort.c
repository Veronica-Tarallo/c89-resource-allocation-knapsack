#include "heapsort.h"

void Scambia (int *pa, int *pb)
{
  int temp;

  temp = *pa;
  *pa = *pb;
  *pb = temp;
}


/* Aggiorna l'heap V di lunghezza (n+1) correggendo l'eventuale errore in posizione i */
void aggiornaheap (vint V, int n, int i)
{
  int s, d;
  int iMax;

  /* Identifica i nodi figli di quello potenzialmente scorretto */
  s = 2*i;
  d = 2*i+1;

  /* Determina il massimo fra il padre e i due nodi figli */
  iMax = i;
  if ( (s <= n) && (V[s] > V[iMax]) ) iMax = s;
  if ( (d <= n) && (V[d] > V[iMax]) ) iMax = d;

  /* Se l'heap e' scorretto, scambia padre e figlio e corregge il sottoalbero figlio */
  if (iMax != i)
  {
    Scambia(&V[iMax],&V[i]);
    aggiornaheap(V,n,iMax);
  }
}

/* Trasforma il vettore di interi V di lunghezza (n+1) in un max-heap */
void creaheap (vint V, int n)
{
  int i;

  for (i = n/2; i >= 0; i--)
    aggiornaheap(V,n,i);
}


/* Ordina il vettore V di interi di lunghezza (n+1) con l'algoritmo HeapSort */
void HeapSort (vint V, int n)
{
  int j;

  creaheap(V,n);
  for (j = n; j >= 1; j--)
  {
    Scambia(&V[0],&V[j]);
    aggiornaheap(V,j-1,0);
  }
}
