#include "liste.h"

int main(int argc, char const *argv[]) {
	/*Client* listeClient = NULL;
	listeClient = addLogLine(listeClient, 12, 5);
	listeClient = addLogLine(listeClient, 1, 5);
	listeClient = addLogLine(listeClient, 45, 5);
	listeClient = addLogLine(listeClient, 100, 5);
	listeClient = addLogLine(listeClient, 45, 7);

	dumpListe(listeClient);*/

	//
	Client* list = NULL;
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
	list = addLogLine(list ,numeroTel,prixAppel);
	//
	// printf("numero=%d prix = %d\n",numeroTel,prixAppel);
	// Affichage du pourcentage d'avancement
	if ((i % pas)==0)
	{
	printf("Done = %d %%...\n",i/pas);
	}
	}
	//
	dumpListe(list);
	//
	printf("======= Facturation appels telephoniques ======\nSuppression de la liste\n");

	supprimerListe(list);

	return 0;
}
