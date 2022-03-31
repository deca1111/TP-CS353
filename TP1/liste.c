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

void supprimerListe(Client *liste){
	if(liste->suivant != NULL){
		supprimerListe(liste->suivant);
	}

	free(liste);
}
