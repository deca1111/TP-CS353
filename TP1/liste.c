#include "liste.h"

Client *createClient(int numero, int nbAppel, int prixAppel)
{
	Client* result = malloc(sizeof(Client));
	result->numero = numero;
	result->prixAppel = prixAppel;
	result->nbAppel = nbAppel;
	result->suivant = NULL;
	return result;
}


Client* addLogLine(Client *liste, int numero, int prixAppel){
	Client* element = liste;
	Client* prec = NULL;
	while ( element!=NULL && element->numero < numero ) {
		prec = element;
		element = element->suivant;
	}
	if (element == NULL) {
		element = createClient(numero, 1, prixAppel);
		if (prec == NULL) {
			liste = element;
		}else{
			prec->suivant = element;
		}
	}else if (element->numero == numero) {
		element->nbAppel ++;
		element->prixAppel += prixAppel;
	}else{
		Client* newClient = createClient(numero, 1, prixAppel);
		newClient->suivant = element;
		if (prec == NULL) {
			liste = newClient;
		}else{
			prec->suivant = newClient;
		}
	}

	return liste;
}

void dumpListe(Client *liste){
	if(liste != NULL){
		printf("Le client [%d] a fait %d appel(s) pour un cout total de %d \n",liste->numero,liste->nbAppel, liste->prixAppel);
		dumpListe(liste->suivant);
	}
}

/*
int main()
{
    client *liste=NULL;

    int i;
    int numeroTel;
    int prixAppel;

    // Aide au calcul du pourcentage d'avancement
    int pas = NBLOGLINE/100;

    printf("****** Facturation appels telephoniques ******\n");


    for(i=0;i<NBLOGLINE;i++)
    {

        // Génération d'un numéro de telephone portable
        numeroTel = 600000000+(rand() % NBCLIENT);

        // Donne un prix d'appel compris entre 0.01 et 4 euros
        prixAppel = (rand() % 400)+1;

        // Ajout de cette ligne de log dans la liste des clients
   	if (!addLogLine(&list ,numeroTel,prixAppel)) break;
        //
        // printf("numero=%d prix = %d\n",numeroTel,prixAppel);


        // Affichage du pourcentage d'avancement
        if ((i % pas)==0)
        {
             printf("Done  = %d %%...\n",i/pas);
        }
    }

   dumpListe(liste);
   printf("======= Facturation appels telephoniques ======\n");

   return 0;
}
*/
