#include "listaelementi.h"

listaelementi crealista ()
{
  listaelementi L;

  L = malloc(sizeof(elemento));
  if (L == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione di una lista!\n");
    exit(EXIT_FAILURE);
  }

  L->item = NullItem;
  L->pred = L;
  L->succ = L;

  return L;
}

void distruggelista (listaelementi *pL)
{
  posizione p;

  p = primolista(*pL);
  while (!listavuota(*pL))
    canclista(*pL,&p);

  free(*pL);
  *pL = NULL;
}

item leggelista (listaelementi L, posizione p)
{
  return (p->item);
}

listaelementi scrivelista (listaelementi L, posizione p, item i)
{
  p->item = i;
  return L;
}

boolean listavuota (listaelementi L)
{
  return (L->succ == L);
}

posizione primolista (listaelementi L)
{
  return L->succ;
}

posizione ultimolista (listaelementi L)
{
  return L->pred;
}

posizione succlista (listaelementi L, posizione p)
{
  return p->succ;
}

posizione predlista (listaelementi L, posizione p)
{
  return p->pred;
}

boolean finelista (listaelementi L, posizione p)
{
  return (p == L);
}

listaelementi inslista (listaelementi L, posizione p, item i)
{
  posizione q;

  q = malloc(sizeof(elemento));
  if (q == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione di un elemento!\n");
    exit(EXIT_FAILURE);
  }

  q->item = i;
  q->pred = p->pred;
  q->succ = p;

  p->pred->succ = q;
  p->pred = q;  /* NB: p->pred va aggiornata per ultima, o le istruzioni precedenti sono scorrette! */

  return L;
}

listaelementi canclista (listaelementi L, posizione *pp)
{
  posizione p;

  p = *pp;
  *pp = p->succ;

  p->pred->succ = p->succ;
  p->succ->pred = p->pred;

  free(p);

  return L;
}


/* Copia nella lista copia la lista L (eliminando cosa era precedentemente contenuto in copia) */
void copialista(listaelementi L, listaelementi *copia) 
{
  posizione p,q;

  distruggelista(copia);
  *copia = crealista();
  
  q = primolista(*copia);
  for (p = primolista(L); !finelista(L,p); p = succlista(L,p))
  {
    *copia = inslista(*copia, q, (p->item));
  }
}

/* Restituisce il numero di elementi presenti nella lista L */
int lunghezzalista(listaelementi L)
{
  posizione p;
  int cont;

  /* Inizializza a zero il contatore di elementi della lista */
  cont = 0;
  /* Scorre la lista e aumenta il contatore */
  for (p = primolista(L); !finelista(L,p); p = succlista(L,p))
    cont++;
  
  return cont;
}

/* Restitusce il budget dell'item in posizione n-esima nella lista L */
int TrovaBudget(listaelementi L, int n)
{
  posizione p;
  item i;
  int cont;

  cont = 0;
  for (p = primolista(L); !finelista(L,p); p = succlista(L,p))
  {
    cont++;
    if (cont == n)
    {
      i = leggelista(L,p);
      return i.budget;
    }
  }

  return 0;
}

/* Restitusce l'identificativo dell'item in posizione n-esima nella lista L */
int TrovaIdentificativo(listaelementi L, int n)
{
  posizione p;
  item i;
  int cont;

  cont = 0;
  for (p = primolista(L); !finelista(L,p); p = succlista(L,p))
  {
    cont++;
    if (cont == n)
    {
      i = leggelista(L,p);
      return i.identificativo;
    }
  }

  return 0;
}

/* Restituisce la posizione dell'item con numero identificativo n nella lista L se questo è presente,
  altrimenti restituisce NO_ELEMENT */
posizione TrovaItem(int n, listaelementi L)
{
  posizione p;
  item i;

  for (p = primolista(L); !finelista(L,p); p = succlista(L,p))
  {
    i = leggelista(L,p);
    if (n == i.identificativo) return p;
  }

  return NO_ELEMENT;
}

/* Restituisce la distanza di un progetto da una lista di progetti, ovvero la somma delle distanze
  di Progetto da ogni progetto nella lista ProgettiSelezionati */
int IndiceDiversita(listaelementi ProgettiSelezionati, item Progetto)
{
  posizione p;
  item PSel;
  int div;

  div = 0;
  for (p = primolista(ProgettiSelezionati); !finelista(ProgettiSelezionati,p); p = succlista(ProgettiSelezionati,p))
  {
    PSel = leggelista(ProgettiSelezionati, p);
    div = div + (abs(PSel.categoria-Progetto.categoria)*100) + (abs(PSel.ISEE-Progetto.ISEE)*4) +
         abs(PSel.data_inizio-Progetto.data_inizio) + abs(PSel.data_fine-Progetto.data_fine);
  }

  return div;
}

/* Restituisce la diversità di un insieme di progetti, ovvero la somma di tutte le distanze a coppie 
  tra i progetti dell'insieme */
int Diversita(listaelementi L)
{
  posizione p, q;
  item pp, qq;
  int div;

  div = 0;
  for (p = primolista(L); !finelista(L,p); p = succlista(L,p))
  {
    pp = leggelista(L, p);
    for (q = primolista(L); !finelista(L,q); q = succlista(L,q))
    {
      qq = leggelista(L, q);
      if(p < q)
      {
        div = div + (abs(qq.categoria-pp.categoria)*100) + (abs(qq.ISEE-pp.ISEE)*4) +
                      abs(qq.data_inizio-pp.data_inizio) + abs(qq.data_fine-pp.data_fine);
      }
    }
  }

  return div;
}