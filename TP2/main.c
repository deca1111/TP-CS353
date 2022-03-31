#include "liste.h"

int main(int argc, char const *argv[]) {

	Client* root, *temp;
	Client * buffer[2];
	root = createSampleTree();
	inserer(root, 15, 14);
	inserer(root, 8, 9);
	inserer(root, 54, 3);

	parcourirInfixe(root);

	temp = chercher(root, atoi(argv[1]));

	printf("On a trouvé le num: [%d]\n",((temp == NULL)?-1:(temp->numero)));
	min(root, NULL, buffer);

	printf("Min de l'arbre est %d et son pere est %d\n", buffer[0]->numero, buffer[1]->numero);


	root = supprimerClient(root, atoi(argv[1]));

	parcourirInfixe(root);

	
	/*
	Client *liste=NULL;

	int i;
	int numeroTel;
	int prixAppel;

	// Aide au calcul du pourcentage d'avancement
	int pas = NBLOGLINE/100;
	for(i=0;i<NBLOGLINE;i++)
	{

	    // Génération d'un numéro de telephone portable
			numeroTel = 600000000+(rand() % NBCLIENT);

	    // Donne un prix d'appel compris entre 0.01 et 4 euros
	    prixAppel = (rand() % 400)+1;

	    // Ajout de cette ligne de log dans la liste des clients
		if (inserer(&liste ,numeroTel,prixAppel)==NULL) break;

	    // Affichage du pourcentage d'avancement
	    if ((i % pas)==0)
	    {
	         printf("Done  = %d %%...\n",i/pas);
	    }
	}

	printf("****** Facturation appels telephoniques ******\n");

	parcourirInfixe(liste);

	printf("****** Suppression de la facturation appels telephoniques ******\n");

	for (i=0;i<NBCLIENT;i++) {
	Client *tmp=NULL;
	if ((tmp=supprimerClient(&liste,600000000+i)) != NULL) { free(tmp); }
	}
	printf("****** Fin Facturation appels telephoniques ******\n");

	*/
	return 0;
}
