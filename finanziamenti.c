/* finanziamenti.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "item.h"
#include "listaelementi.h"
#include "alloc.h"
#include "knapsack.h"
#include "heapsort.h"

/* PROTOTIPI DELLE PROCEDURE SECONDARIE */

void InterpretaLineaComando (int argc, char *argv[], char *file_eventi);

/* Rstituisce il numero delle righe del file file_eventi */
int NumRigheFile(char *file_eventi);

/* Legge gli eventi scritti nel file file_eventi e esegue le procedure richieste. Restituisce il numero di progetti proposti - 1 */
int EsegueEventi(char *file_eventi, listaelementi *pEntiRegistrati,listaelementi *pProgettiFinanziati,
                    listaelementi *pProgettiDisponibili, vint OrdineProposte);

/* Cancella i progetti finanziati da questo ente dalla lista dei progetti finanziati e li riinserisce nella lista dei progetti disponibili */
void CancellaFinanziamentiEnte(item Ente, listaelementi *pProgettiFinanziati, listaelementi *pProgettiDisponibili);

/* Seleziona il sottoinsieme di progetti disponibili in modo che abbia indice totale di preferenza massimo e sia compatibile
  con il budget dell'ente. Cancella i progetti selezionati dalla lista dei progetti disponibili e li inserisce nella lista dei 
  progetti finanziati (dopo aver modificato opportunamente il campo entefinanziatore) e modifica opportunamente
  il budget disponibile dell'ente */
void SelezionaFinanziamenti(item Ente, listaelementi *pEntiRegistrati, listaelementi *pProgettiDisponibili, 
                            listaelementi *pProgettiFinanziati);

/* Risolve il Knapsack Problem con la programmazione dinamica bottom-up (iterativa) */
void PDiterativaKP (datiKP *pI, soluzioneKP *pS);

/* Cancella i progetti selezionati dall'ente dalla lista dei progetti disponibili e li inserisce nella lista dei 
  progetti finanziati (dopo aver modificato opportunamente il campo entefinanziatore) e modifica opportunamente
  il budget disponibile dell'ente */
void NuoviFinanziamenti(item Ente, listaelementi *pEntiRegistrati, listaelementi *pProgettiDisponibili, listaelementi *pProgettiFinanziati,
                        int n, soluzioneKP *pS);

/* Esegue il ripescaggio di fine anno */
void Ripescaggio(listaelementi EntiRegistrati, listaelementi *pProgettiDisponibili, listaelementi *pProgettiFinanziati);

/* Raccoglie il budget residuo di tutti gli enti ancora registrati nel sistema */
int RaccoltaFondi(listaelementi EntiRegistrati);

/* Restituisce la lista dei progetti selezionati in modo da avere diversità massima */
listaelementi TrovaInsiemeMaxDiversita(int budget, item Progetto, listaelementi ProgettiDisponibili);

/* Stampa la lista degli enti registrati, la lista dei progetti finanziati e la lista dei progetti disponibili */
void Report(listaelementi EntiRegistrati, listaelementi ProgettiFinanziati, listaelementi ProgettiDisponibili, 
            vint OrdineProposte, int NumProposte);

/* Stampa la lista degli enti registrati nell'ordine in cui si sono registrati */
void StampaEnti(listaelementi EntiRegistrati, listaelementi ProgettiFinanziati);

/* Stampa (in ordine crescente) i numeri identificativi dei progetti finanziati dall'ente */
void StampaFinanziamentiEnte(item Ente ,listaelementi ProgettiFinanziati);

/* Stampa la lista dei progetti finanziati nell'ordine in cui i progetti sono stati proposti */
void StampaProgettiFinanziati(listaelementi ProgettiFinanziati, listaelementi EntiRegistrati, vint OrdineProposte, int NumProposte);

/* Stampa il numero identificativo e la descrizione dell'ente finanziatore di un progetto */
void StampaEnteFinanziatore(item ProgettoFinanziato, listaelementi EntiRegistrati);

/* Stampa la lista dei progetti disponibili nell'ordine in cui i progetti sono stati proposti */
void StampaProgettiDisponibili(listaelementi ProgettiDisponibili, vint OrdineProposte, int NumProposte);



/* PROGRAMMA PRINCIPALE */
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char file_eventi[ROW_LENGTH];
  listaelementi EntiRegistrati;
  listaelementi ProgettiDisponibili, ProgettiFinanziati;
  int NumRighe, NumProposte;
  vint OrdineProposte;

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,file_eventi);

  /* Restituisce il numero di righe del file letto dalla linea di comando */
  NumRighe = NumRigheFile(file_eventi);

  /* Alloca il vettore in cui saranno memorizzati gli identificativi dei progetti proposti nell'ordine di proposta */
  OrdineProposte = intalloc(NumRighe);

  /* Crea le liste degli enti regsistrati, progetti disponibili e progetti finanziati */
  EntiRegistrati = crealista();
  ProgettiDisponibili = crealista();
  ProgettiFinanziati = crealista();

  /* Esegue la sequenza di eventi riportati nel file degli eventi e restituisce il numero di progetti proposti-1 */
  NumProposte = EsegueEventi(file_eventi, &EntiRegistrati, &ProgettiFinanziati, &ProgettiDisponibili, OrdineProposte);

  /* Esegue il ripescaggio di fine anno */
  Ripescaggio(EntiRegistrati, &ProgettiDisponibili, &ProgettiFinanziati);
  
  /* Stampa la situazione a fine anno */
  Report(EntiRegistrati, ProgettiFinanziati, ProgettiDisponibili, OrdineProposte, NumProposte);
  
  /* Dealloca le strutture dinamiche */
  intfree(&OrdineProposte);
  distruggelista(&EntiRegistrati);
  distruggelista(&ProgettiDisponibili);
  distruggelista(&ProgettiFinanziati);

  return EXIT_SUCCESS;
}


/* DEFINIZIONE DELLE PROCEDURE SECONDARIE */

void InterpretaLineaComando (int argc, char *argv[], char *file_eventi)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(file_eventi,argv[1]);
}

/* Rstituisce il numero delle righe del file file_eventi */
int NumRigheFile(char *file_eventi)
{
  FILE *fp;
  char Riga[ROW_LENGTH];
  int NumRigheFile;

  NumRigheFile = 0;
  /* Apre il file degli eventi */
  fp = fopen(file_eventi,"r");
  if (fp == NULL)
  {
    fprintf(stderr,"Errore nell'apertura del file %s!\n",file_eventi);
    exit(EXIT_FAILURE);
  }

  while (fscanf(fp,"%[^\n]\n",Riga) == 1)
    NumRigheFile++;
  
  /* Chiude il file dei movimenti */
  fclose(fp);

  return NumRigheFile;
}

/* Legge gli eventi scritti nel file file_eventi e esegue le procedure richieste. Restituisce il numero di progetti proposti - 1 */
int EsegueEventi(char *file_eventi, listaelementi *pEntiRegistrati, listaelementi *pProgettiFinanziati,
                 listaelementi *pProgettiDisponibili, vint OrdineProposte)
{
  FILE *fp;
  char Comando[ROW_LENGTH];
  item i, Ente, ProgettoFinanziato;
  posizione p;
  int NumProgetto, NumEnte;
  int NumProposte;

  NumProposte = -1;

  /* Apre il file degli eventi */
  fp = fopen(file_eventi,"r");
  if (fp == NULL)
  {
    fprintf(stderr,"Errore nell'apertura del file %s!\n",file_eventi);
    exit(EXIT_FAILURE);
  }

  /* Legge il comando */
  while (fscanf(fp,"%s",Comando) == 1)
  { 
    if (strcmp(Comando,"proposta") == 0)
    {
      fscanf(fp,"%d %s %d %d %d %d %d", &i.identificativo, i.descrizione, &i.budget, &i.categoria, &i.ISEE, &i.data_inizio, &i.data_fine);
      /* Inizializzo il campo entefinanziatore a zero (per indicare che non c'è ente finanziatore) */
      i.entefinanziatore = 0;
      /* Aggiunge il progetto alla lista dei progetti disponibili */
      p = succlista(*pProgettiDisponibili,ultimolista(*pProgettiDisponibili));
      *pProgettiDisponibili = inslista(*pProgettiDisponibili,p,i);
      /* Memorizza in OrdineProposte l'identificativo */
      NumProposte++;
      OrdineProposte[NumProposte] = i.identificativo;
    }
    else if (strcmp(Comando,"ritiro") == 0)
    {
      fscanf(fp,"%d", &NumProgetto);
      /* Cerca il progetto tra quelli disponibili e, se lo trova, lo elimina */
      p = TrovaItem(NumProgetto, *pProgettiDisponibili);
      if (p != NO_ELEMENT) *pProgettiDisponibili = canclista(*pProgettiDisponibili,&p);
      /* Cerca il progetto tra quelli finanziati e, se lo trova, lo elimina */
      p = TrovaItem(NumProgetto, *pProgettiFinanziati);
      if (p != NO_ELEMENT)
      {
        ProgettoFinanziato = leggelista(*pProgettiFinanziati, p);
        /* Cancella il progetto dalla lista dei progetti finanziati */
        *pProgettiFinanziati = canclista(*pProgettiFinanziati,&p);
        /* Restituisce il budget che era stato speso all'ente finanziatore */
        p = TrovaItem(ProgettoFinanziato.entefinanziatore, *pEntiRegistrati);
        (p->item).budget = (p->item).budget + ProgettoFinanziato.budget;
      }
    }
    else if (strcmp(Comando,"registrazione") == 0)
    {
      fscanf(fp,"%d %s %d %d %d %d %d", &i.identificativo, i.descrizione, &i.budget, &i.categoria, &i.ISEE, &i.data_inizio, &i.data_fine);
      /* Inizializzo il campo entefinanziatore a zero (in questo caso, trattandosi di un ente, questo campo non verrà utilizzato) */
      i.entefinanziatore = 0;
      /* Aggiunge l'ente alla lista degli enti registrati */
      p = succlista(*pEntiRegistrati,ultimolista(*pEntiRegistrati));
      *pEntiRegistrati = inslista(*pEntiRegistrati,p,i);
    }
    else if (strcmp(Comando,"cancellazione") == 0)
    {
      fscanf(fp,"%d", &NumEnte);
      /* Cerca l'ente tra quelli registrati e, se lo trova, lo elimina */
      p = TrovaItem(NumEnte, *pEntiRegistrati);
      if (p != NO_ELEMENT)
      {
        Ente = leggelista(*pEntiRegistrati, p);
        /* Cancella l'ente dalla lista degli enti registrati */
        *pEntiRegistrati = canclista(*pEntiRegistrati,&p);
        /* Cancella i progetti finanziati da questo ente dalla lista dei progetti finanziati
            e li riinserisce nella lista dei progetti disponibili  */
        CancellaFinanziamentiEnte(Ente, pProgettiFinanziati, pProgettiDisponibili);
      }
    }
    else if (strcmp(Comando,"stampa") == 0)
    {
      Report(*pEntiRegistrati, *pProgettiFinanziati, *pProgettiDisponibili, OrdineProposte, NumProposte);
    }
    else if (strcmp(Comando,"selezione") == 0)
    {
      fscanf(fp,"%d", &NumEnte);
      
      /* Cerca l'ente tra quelli registrati e, se lo trova, lo elimina */
      p = TrovaItem(NumEnte, *pEntiRegistrati);
      if (p != NO_ELEMENT)
      {
        Ente = leggelista(*pEntiRegistrati, p);
        /* Seleziona il sottoinsieme di progetti disponibili che abbia indice totale di preferenza massimo 
            e sia compatibile con il budget dell'ente (restituisce il budget speso e gli identificativi dei
            progetti selezionati) */
        SelezionaFinanziamenti(Ente,pEntiRegistrati, pProgettiDisponibili,pProgettiFinanziati);
      }
    }
  }
  /* Chiude il file dei libri */
  fclose(fp);

  return NumProposte;
}

/* Cancella i progetti finanziati da questo ente dalla lista dei progetti finanziati e li riinserisce nella lista dei progetti disponibili  */
void CancellaFinanziamentiEnte(item Ente, listaelementi *pProgettiFinanziati, listaelementi *pProgettiDisponibili)
{
  posizione p;
  item Progetto;
  vint finanziamenti;
  int i,j;

  /* Alloca un vettore di interi in cui memorizzare gli identificativi dei progetti finanziati dall'ente */
  finanziamenti = intalloc(lunghezzalista(*pProgettiFinanziati));

  j = -1;

  /* Memorizzo i numeri identificativi dei progetti finanziati dall'ente */
  for (p = primolista(*pProgettiFinanziati); !finelista(*pProgettiFinanziati,p); p = succlista(*pProgettiFinanziati,p))
  {
    Progetto = leggelista(*pProgettiFinanziati, p);
    if(Progetto.entefinanziatore == Ente.identificativo)  
    {
      j++;
      finanziamenti[j] = Progetto.identificativo;
    }
  }

  /* Modifico le liste progetti finanziati e progetti disponibili */
  for(i=0; i<=j; i++)
  {
    p = TrovaItem(finanziamenti[i], *pProgettiFinanziati);
    if(p!=NO_ELEMENT)
    {
      Progetto = leggelista(*pProgettiFinanziati, p);
      /* Cancella il progetto dalla lista dei progetti finanziati */
      *pProgettiFinanziati = canclista(*pProgettiFinanziati,&p);
      /* Modifica il campo entefinanziatore del progetto */
      Progetto.entefinanziatore = 0;
      /* Inserisce il progetto nella lista dei progetti disponibili */
      p = succlista(*pProgettiDisponibili, ultimolista(*pProgettiDisponibili));
      *pProgettiDisponibili = inslista(*pProgettiDisponibili, p, Progetto);
    }
  }

  /* Dealloca il vettore di int */
  intfree(&finanziamenti);
}

/* Seleziona il sottoinsieme di progetti disponibili in modo che abbia indice totale di preferenza massimo e sia compatibile
  con il budget dell'ente. Cancella i progetti selezionati dalla lista dei progetti disponibili e li inserisce nella lista dei 
  progetti finanziati (dopo aver modificato opportunamente il campo entefinanziatore) e modifica opportunamente
  il budget disponibile dell'ente */
void SelezionaFinanziamenti(item Ente, listaelementi *pEntiRegistrati, listaelementi *pProgettiDisponibili, 
                            listaelementi *pProgettiFinanziati)
{
  datiKP I;
  soluzioneKP S;

  /* Carica i dati */
  CaricaDatiKP(Ente, pProgettiDisponibili,&I);
  /* Crea una soluzione vuota */
  CreaSoluzioneKP(I.n,&S);

  /* Risolve il problema di trovare il sottoinsieme con indice di preferenza totale massimo */
  PDiterativaKP(&I,&S);

  /* Cancella i progetti selezionati dalla lista dei progetti disponibili e li inserisce nella lista dei 
    progetti finanziati (dopo aver modificato opportunamente il campo entefinanziatore) e modifica opportunamente
    il budget disponibile dell'ente */
  NuoviFinanziamenti(Ente, pEntiRegistrati, pProgettiDisponibili, pProgettiFinanziati, I.n, &S);

  /* Dealloca le strutture dati dinamiche */
  DistruggeDatiKP(&I);
  DistruggeSoluzioneKP(&S);
}

/* Risolve il Knapsack Problem con la programmazione dinamica bottom-up (iterativa) */
void PDiterativaKP (datiKP *pI, soluzioneKP *pS)
{
  int **F;
  int i, v;
  int f0, f1;


  /* Crea la matrice ausiliaria che conserva i sottoproblemi */
  F = int2alloc(pI->n+1,pI->V+1);

  /* Programmazione dinamica iterativa per il problema dello zaino */
  for (i = 0; i <= pI->n; i++)
  {
    for (v = 0; v <= pI->V; v++)
    {
      /* Risolve il sottoproblema (i,v) */
      if (i == 0)
        F[i][v] = 0;                          /* return 0;                                */
      else if (pI->v[i] > v)
        F[i][v] = F[i-1][v];                  /* return PDricorsivaKP(n-1,phi,v,V);          */
      else /* (pI->v[i] <= v) */
      {
        f0 = F[i-1][v];                       /* return PDricorsivaKP(n-1,phi,v,V);          */
        f1 = pI->phi[i] + F[i-1][v-pI->v[i]]; /* return PDricorsivaKP(n-1,phi,v,V-pI->v[n]); */
        F[i][v] = ( (f0 >= f1) ? f0 : f1 );
      }
    }
  }

  /* Il problema originale corrisponde alla cella (n,V): il suo valore ottimo e' il contenuto della cella */
  pS->f = F[pI->n][pI->V];

  /* Ricostruisce la soluzione, determinando all'indietro quale sottoproblema ha determinato la soluzione di ogni problema */
  v = pI->V;
  for (i = pI->n; i >= 1; i--)
    if (F[i][v] == F[i-1][v])
      pS->x[i] = FALSE;
    else
    {
      pS->x[i] = TRUE;
      v -= pI->v[i];
    }

  /* Dealloca la matrice ausiliaria */
  int2free(&F,pI->n+1);
}

/* Cancella i progetti selezionati dall'ente dalla lista dei progetti disponibili e li inserisce nella lista dei 
  progetti finanziati (dopo aver modificato opportunamente il campo entefinanziatore) e modifica opportunamente
  il budget disponibile dell'ente */
void NuoviFinanziamenti(item Ente, listaelementi *pEntiRegistrati, listaelementi *pProgettiDisponibili, listaelementi *pProgettiFinanziati,
                        int n, soluzioneKP *pS)
{
  int i, identificativo, budget_speso;
  posizione p, q;
  item Progetto;
  vint identificativi_selezionati;
  int j;

  /* Alloca il vettore di interi in cui saranno memorizzati gli identificativi dei progetti selezionati */
  identificativi_selezionati = intalloc(n);

  budget_speso = 0;   /* Inizializza a 0 il budget speso */
  j = -1;

  for (i = 1; i <= n; i++)
  {
    if (pS->x[i] == TRUE)
    {
      identificativo = TrovaIdentificativo(*pProgettiDisponibili, i);
      j++;
      identificativi_selezionati[j] = identificativo;
      budget_speso = budget_speso + TrovaBudget(*pProgettiDisponibili, i);
    }
  }

  /* Modifica le liste progetti disponibili e progetti finanziati */
  for(i=0; i<=j; i++)
  {
    p = TrovaItem(identificativi_selezionati[i], *pProgettiDisponibili);
    if(p != NO_ELEMENT)
    {
      Progetto = leggelista(*pProgettiDisponibili, p);
      /* Lo cancella dalla lista dei progetti disponibili */
      *pProgettiDisponibili = canclista(*pProgettiDisponibili, &p);
      /* Modifica il campo entefinanziatore */
      Progetto.entefinanziatore = Ente.identificativo;
      /* Lo inserisce nella lista dei progetti finanziati */
      p = succlista(*pProgettiFinanziati, ultimolista(*pProgettiFinanziati));
      *pProgettiFinanziati = inslista(*pProgettiFinanziati, p, Progetto);
    }
  }

  /* Modifica il budget dell'ente */
  q = TrovaItem(Ente.identificativo, *pEntiRegistrati);
  if(q!=NO_ELEMENT) (q->item).budget = (q->item).budget - budget_speso;

  /* Riordina il vettore dei numeri identificativi dei progetti selezionati */
  HeapSort(identificativi_selezionati, j);
  /* Stampa del messaggio di avviso per i nuovi finanziamenti */
  printf("Ente %d finanzia", (q->item).identificativo);
  for(i=0; i<=j; i++)
    printf(" %d", identificativi_selezionati[i]);
  printf(" budget residuo %d\n", (q->item).budget);

  /* Dealloca il vettore di int */
  intfree(&identificativi_selezionati);
}

/* Esegue il ripescaggio di fine anno */
void Ripescaggio(listaelementi EntiRegistrati, listaelementi *pProgettiDisponibili, listaelementi *pProgettiFinanziati)
{
  int i,j;
  int budget_temp, budget_residuo;
  int temp, div;
  int id_prec;
  int controllo;
  posizione p, q;
  item Progetto;
  listaelementi ProgettiSelezionati, ListaTemp;
  vint identificativi;

  controllo = 0;
  id_prec = 0;
  div = 0;
  ListaTemp = crealista();
  ProgettiSelezionati = crealista();

  /* Raccoglie il budget residuo di tutti gli enti ancora registrati nel sistema */
  budget_residuo = RaccoltaFondi(EntiRegistrati);

  /* Determina il sottoinsieme di progetti più diversificato compatibile con il budget raccolto */
  for (p=primolista(*pProgettiDisponibili); !finelista(*pProgettiDisponibili,p); p=succlista(*pProgettiDisponibili,p))
  {
    Progetto = leggelista(*pProgettiDisponibili,p);
    if(Progetto.budget <= budget_residuo)
    {
      budget_temp = budget_residuo;
      budget_temp = budget_temp - Progetto.budget;
      ListaTemp = TrovaInsiemeMaxDiversita(budget_temp, Progetto, *pProgettiDisponibili);
      
      /* Calcola la diversità dell'insieme selezionato */
      temp = Diversita(ListaTemp);
      if(temp > div || ((temp == div) && (controllo == 0)) || ((temp == div) && (Progetto.identificativo < id_prec)))
      {
        controllo = 1;
        div = temp;
        /* Copia nella lista dei progetti selezionati la lista temp */
        copialista(ListaTemp, &ProgettiSelezionati);
        id_prec = Progetto.identificativo;    /* Utilizzato per gestire i casi di parità */
      }
    }
  }

  /* Se esiste un sottoinsime non vuoto di progetti finanziabili dal comitato */
  if(controllo == 1)
  {
    /* Alloca un vetttore di interi in cui memorizzare gli identificativi dei progetti selezionati */
    identificativi = intalloc(lunghezzalista(ProgettiSelezionati));

    j=-1;
    for (p=primolista(ProgettiSelezionati); !finelista(ProgettiSelezionati,p); p=succlista(ProgettiSelezionati,p))
    {
      Progetto = leggelista(ProgettiSelezionati, p);
      Progetto.entefinanziatore = -1;
      /* Diminuossce il budget residuo del comitato */
      budget_residuo = budget_residuo - Progetto.budget;
      j++;
      identificativi[j] = Progetto.identificativo;
      /* Cancella i progetti selezionati dalla lista dei progetti disponibili */
      q = TrovaItem(Progetto.identificativo, *pProgettiDisponibili);
      *pProgettiDisponibili = canclista(*pProgettiDisponibili, &q);
      /* Inserisce i progetti selezionati nella lista dei progetti finanziati */
      q = succlista(*pProgettiFinanziati, ultimolista(*pProgettiFinanziati));
      *pProgettiFinanziati = inslista(*pProgettiFinanziati, q, Progetto);
    }

    /* Riordina gli identificativi */
    HeapSort(identificativi, j);
    /* Stampa messaggio di avviso dei finanziamenti del comitato */
    printf("Comitato finanzia");
    for(i=0; i<=j; i++)
      printf(" %d", identificativi[i]);
    printf(" budget residuo %d\n", budget_residuo);
  }
  /* Se il comitato non riesce a finanziare nessun progetto*/
  else printf("Comitato finanzia budget residuo %d\n", budget_residuo);

  /* Dealloca le strutture dinamiche ausiliarie */
  intfree(&identificativi);
  distruggelista(&ProgettiSelezionati);
  distruggelista(&ListaTemp);
}

/* Raccoglie il budget residuo di tutti gli enti ancora registrati nel sistema */
int RaccoltaFondi(listaelementi EntiRegistrati)
{
  int budget_comitato;
  posizione p;

  budget_comitato = 0;    /* Inizializza a 0 il budget del comitato */

  for (p = primolista(EntiRegistrati); !finelista(EntiRegistrati,p); p = succlista(EntiRegistrati,p))
    budget_comitato = budget_comitato + (p->item).budget;

  return budget_comitato;
}

/* Restituisce la lista dei progetti selezionati in modo da avere diversità massima */
listaelementi TrovaInsiemeMaxDiversita(int budget, item Progetto, listaelementi ProgettiDisponibili)
{
  listaelementi ProgettiSelezionati;  /* Lista in cui memorizzo i progetti selezionati */
  listaelementi ListaTemp;  /* Lista in cui memorizzo i progetti non ancora selezionati */
  posizione p,q;
  item ProgettoMaxDiv, ProgettoDisponibile;
  int div, temp;
  int controllo;

  controllo = 1;  /* 1 = c'è almeno un progetto finanziabile da parte del comitato */

  ProgettiSelezionati = crealista();
  ListaTemp = crealista();

  /* Copia in ListaTemp la lista dei progetti disponibili */
  copialista(ProgettiDisponibili, &ListaTemp);

  /* Inserisce il progetto iniziale nella lista dei progetti selezionati */
  ProgettiSelezionati = inslista(ProgettiSelezionati, primolista(ProgettiSelezionati), Progetto);

  /* Elimina il progetto iniziale da ListaTemp */
  q = TrovaItem(Progetto.identificativo, ListaTemp);
  ListaTemp = canclista(ListaTemp, &q);

  /* Finchè c'è almeno un progetto con budget richiesto <= del budget residuo del comitato */
  while(controllo == 1)
  {
    /* Trovo il progetto con la massima diversità */
    controllo = 0;
    div = 0;
    for (p = primolista(ListaTemp); !finelista(ListaTemp,p); p = succlista(ListaTemp,p))
    {
      ProgettoDisponibile = leggelista(ListaTemp, p);
      /* Se il budget richiesto dal progetto è <= del budget disponibile */
      if(ProgettoDisponibile.budget <= budget)
      {
        controllo = 1;
        /* Calcola l'indice di diversità */
        temp = IndiceDiversita(ProgettiSelezionati, ProgettoDisponibile);
        /* Se è >= di quello memorizzato */
        if(temp > div || ((temp == div) && (ProgettoDisponibile.identificativo < ProgettoMaxDiv.identificativo)))
        {
          div = temp;
          ProgettoMaxDiv = ProgettoDisponibile;
        }
      }
    }

    /* Se il comitato finanzia almeno un progetto */
    if(controllo == 1)
    {
      /* Diminuisce il budget  residuo */
      budget = budget - ProgettoMaxDiv.budget;
      /* Cancella il progetto selezionato dalla lista dei progetti disponibili ListaTemp */
      q = TrovaItem(ProgettoMaxDiv.identificativo, ListaTemp);
      ListaTemp = canclista(ListaTemp, &q);
      /* Inserisce il progetto selezionato nella lista dei progetti selezionati */
      q = succlista(ProgettiSelezionati,ultimolista(ProgettiSelezionati));
      ProgettiSelezionati = inslista(ProgettiSelezionati, q, ProgettoMaxDiv);
    }
  }

  /* Dealloca le strutture dinamiche ausiliarie */
  distruggelista(&ListaTemp);

  /* Restituisce la lista dei progetti selezionati dal comitato */
  return ProgettiSelezionati;
}

/* Stampa la lista degli enti registrati, la lista dei progetti finanziati e la lista dei progetti disponibili */
void Report(listaelementi EntiRegistrati, listaelementi ProgettiFinanziati, listaelementi ProgettiDisponibili, 
            vint OrdineProposte, int NumProposte)
{
  /* Stampa la lista degli enti registrati */
  StampaEnti(EntiRegistrati, ProgettiFinanziati);
  /* Stampa la lista dei progetti finanziati */
  StampaProgettiFinanziati(ProgettiFinanziati, EntiRegistrati, OrdineProposte, NumProposte);
  /* Stampa la lista dei progetti disponibili */
  StampaProgettiDisponibili(ProgettiDisponibili, OrdineProposte, NumProposte);
}

/* Stampa la lista degli enti registrati nell'ordine in cui si sono registrati */
void StampaEnti(listaelementi EntiRegistrati, listaelementi ProgettiFinanziati)
{
  posizione p;
  item Ente;

  printf("Enti\n");
  
   for (p = primolista(EntiRegistrati); !finelista(EntiRegistrati,p); p = succlista(EntiRegistrati,p))
  {
    Ente = leggelista(EntiRegistrati,p);
    printf("%d %s", Ente.identificativo, Ente.descrizione);
    /* Stampa gli identificativi dei progetti finanziati dall'ente in ordine crescente */
    StampaFinanziamentiEnte(Ente, ProgettiFinanziati);
    printf(" %d\n", Ente.budget);
  }
}

/* Stampa (in ordine crescente) i numeri identificativi dei progetti finanziati dall'ente */
void StampaFinanziamentiEnte(item Ente ,listaelementi ProgettiFinanziati)
{
  posizione p;
  item Progetto;
  vint identificativi_progetti;
  int i,j;

  /* Alloca il vettore in cui memorizzare gli identificativi dei progetti finanziati */
  identificativi_progetti = intalloc(lunghezzalista(ProgettiFinanziati));

  j = -1;

  /* Scorre la lista dei progetti finanziati */
  for (p = primolista(ProgettiFinanziati); !finelista(ProgettiFinanziati,p); p = succlista(ProgettiFinanziati,p))
  {
    Progetto = leggelista(ProgettiFinanziati,p);
    /* Se il progetto è finanziato dall'ente stampa il num identificativo del progetto */
    if (Progetto.entefinanziatore == Ente.identificativo) 
    {
      j++;
      identificativi_progetti[j] = Progetto.identificativo;
    }
  }

  /* Riordina il vettore identificatvi_progetti */
  HeapSort(identificativi_progetti, j);
  /* Stampa i numeri identificativi riordinati dei progetti finanziati dall'ente */
  for(i=0; i<=j; i++)
    printf(" %d", identificativi_progetti[i]);

  /* Dealloca il vettore di int */
  intfree(&identificativi_progetti);
}

/* Stampa la lista dei progetti finanziati nell'ordine in cui i progetti sono stati proposti */
void StampaProgettiFinanziati(listaelementi ProgettiFinanziati, listaelementi EntiRegistrati, vint OrdineProposte, int NumProposte)
{
  int i;
  posizione p;
  item ProgettoFinanziato;

  printf("Progetti finanziati\n");
  /* Per ogni cella utilizzata del vettore OrdineProposte */
  for(i=0; i<=NumProposte; i++)
  {
    /* Scorre la lista dei progetti finanziati */
    for (p = primolista(ProgettiFinanziati); !finelista(ProgettiFinanziati,p); p = succlista(ProgettiFinanziati,p))
    {
      ProgettoFinanziato = leggelista(ProgettiFinanziati,p);
      /* Se l'identificativo del progetto corrisponde all'elemento memorizzato in OrdineProposte stampa il progetto */
      if(ProgettoFinanziato.identificativo == OrdineProposte[i])
      {
        printf("%d %s", ProgettoFinanziato.identificativo, ProgettoFinanziato.descrizione);
        /* Stampa l'identificativo e il nome dell'ente finanziatore */
        StampaEnteFinanziatore(ProgettoFinanziato, EntiRegistrati);
        printf(" %d %d %d %d %d\n", ProgettoFinanziato.budget, ProgettoFinanziato.categoria,
              ProgettoFinanziato.ISEE, ProgettoFinanziato.data_inizio, ProgettoFinanziato.data_fine);
        break;
      }
    }
  }
}

/* Stampa il numero identificativo e la descrizione dell'ente finanziatore di un progetto */
void StampaEnteFinanziatore(item ProgettoFinanziato, listaelementi EntiRegistrati)
{
  posizione p;
  item Ente;

  /* Se il progetto è finanziato dal comitato stampa le info del comitato */
  if(ProgettoFinanziato.entefinanziatore == -1) printf(" -1 Comitato");
  else
  {
    /* Scorre la lista degli enti registrati */
    for (p = primolista(EntiRegistrati); !finelista(EntiRegistrati,p); p = succlista(EntiRegistrati,p))
    {
      Ente = leggelista(EntiRegistrati,p);
      /* Se l'identificativo dell'ente corrisponde con quello che si trova nel campo entefinanziatore del 
        progetto, stampa le info dell'ente */
      if(Ente.identificativo == ProgettoFinanziato.entefinanziatore)
        printf(" %d %s", Ente.identificativo, Ente.descrizione);
    }
  }
}

/* Stampa la lista dei progetti disponibili nell'ordine in cui i progetti sono stati proposti */
void StampaProgettiDisponibili(listaelementi ProgettiDisponibili,  vint OrdineProposte, int NumProposte)
{
  int i;
  posizione p;
  item ProgettoDisponibile;

  printf("Progetti disponibili\n");
  /* Per ogni cella utilizzata del vettore OrdineProposte */
  for(i=0; i<=NumProposte; i++)
  {
    /* Scorre la lista dei progetti disponibili */
    for (p = primolista(ProgettiDisponibili); !finelista(ProgettiDisponibili,p); p = succlista(ProgettiDisponibili,p))
    {
      ProgettoDisponibile = leggelista(ProgettiDisponibili,p);
      /* Se l'identificativo del progetto corrisponde all'elemento memorizzato in OrdineProposte stampa il progetto */
      if(ProgettoDisponibile.identificativo == OrdineProposte[i])
        stampaitem(ProgettoDisponibile);
    }
  }
}
