#ifndef __LISTE_H__
#define __LISTE_H__
#include <stdlib.h>
#include <stdio.h>

typedef struct client {
	int numero;
	int prixAppel;
	int nbAppel;
	struct client * suivant;
} Client;

// Nombre total de lignes dans le fichier
#ifndef NBLOGLINE
#define NBLOGLINE    2000000
#endif

// Nombre de clients
#ifndef NBCLIENT
#define NBCLIENT    20000
#endif

Client *createClient(int numero, int nbAppel, int prixAppel);

Client* addLogLine(Client *liste, int numero, int prixAppel);

void dumpListe(Client *liste);

void supprimerListe(Client *liste);

#endif
