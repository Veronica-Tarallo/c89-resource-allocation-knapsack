#include "knapsack.h"

/* Carica dalla lista dei progetti disponibili l'istanza I del problema dello zaino */
void CaricaDatiKP (item Ente, listaelementi *pProgettiDisponibili, datiKP *pI)
{
  int i;
  posizione p;
  item Progetto;
  
  pI->V = Ente.budget;
  pI->n = lunghezzalista(*pProgettiDisponibili);

  pI->phi = (int *) calloc(pI->n+1,sizeof(int));
  if (pI->phi == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore dei premi!\n");
    exit(EXIT_FAILURE);
  }

  pI->v = (int *) calloc(pI->n+1,sizeof(int));
  if (pI->v == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore dei volumi!\n");
    exit(EXIT_FAILURE);
  }

  i = 1;  /* Inizializza i a 1 */
  /* Scorre la lista dei progetti disponibili */
  for (p = primolista(*pProgettiDisponibili); !finelista(*pProgettiDisponibili,p); p = succlista(*pProgettiDisponibili,p))
  {
    Progetto = leggelista(*pProgettiDisponibili, p);
    pI->v[i] = Progetto.budget;
    pI->phi[i] = IndicePreferenza(Ente, Progetto);
    i++;
  }
}


/* Dealloca le strutture dati dinamiche */
void DistruggeDatiKP (datiKP *pI)
{
  free(pI->phi);
  free(pI->v);
  pI->n = 0;
  pI->phi = NULL;
  pI->v = NULL;
  pI->V = 0;
}


/* Crea una soluzione vuota */
void CreaSoluzioneKP (int n, soluzioneKP *pS)
{
  pS->f = 0L;
  pS->x = (vboolean) calloc(n+1,sizeof(boolean));
  if (pS->x == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione della soluzione!\n");
    exit(EXIT_FAILURE);
  }
}


/* Distrugge la soluzione S */
void DistruggeSoluzioneKP (soluzioneKP *pS)
{
  pS->f = 0L;
  free(pS->x);
  pS->x = NULL;
}