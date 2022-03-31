#ifndef __LISTE_H__
#define __LISTE_H__
#include <stdlib.h>
#include <stdio.h>

typedef struct client {
	int numero;
	int coutTot;
	int nbAppel;
	struct client * filsG;
	struct client * filsD;
} Client;

// Nombre total de lignes dans le fichier
#ifndef NBLOGLINE
#define NBLOGLINE    1000000
#endif
// Nombre de clients
#ifndef NBCLIENT
#define NBCLIENT    20000
#endif

Client * createSampleTree();

Client * creerClient(int numeroTel, int nbAppel,int cout);

Client * chercher(Client * abr,int numeroTel);

Client *inserer(Client * abr, int numeroTel, int prixAppel);

Client *supprimerClient(Client * abr, int numeroTel);

void parcourirInfixe(Client * abr);

void min(Client * min, Client * pere_min, Client ** buffer);

//le pere doit avoir un fils droit
Client * detache_successeur(Client* pere);


#endif
