#include "liste.h"

Client *createClient(int numero, int nbAppel, int cout)
{
	Client* result = malloc(sizeof(Client));
	result->numero = numero;
	result->coutTot = cout;
	result->nbAppel = nbAppel;
	result->filsG = NULL;
	result->filsD = NULL;
	return result;
}

Client * createSampleTree(){
	Client* racine, *n12, *n8, *n14, *n20, *n16, *n21;
	racine = createClient(15,1,10);
	n12 = createClient(12,1,10);
	n8 = createClient(8,1,10);
	n14 = createClient(14,1,10);
	n20 = createClient(20,1,10);
	n16 = createClient(16,1,10);
	n21 = createClient(21,1,10);

	racine->filsG = n12;
	racine->filsD = n20;
	n12->filsG=n8;
	n12->filsD=n14;
	n20->filsG=n16;
	n20->filsD=n21;


	return racine;

}

Client * chercher(Client * abr,int numeroTel){
	if(abr == NULL){
		return NULL;
	}else if(abr->numero == numeroTel){
		return abr;
	}else{
		if (numeroTel > abr->numero) {
			return(chercher(abr->filsD, numeroTel));
		}else{
			return(chercher(abr->filsG, numeroTel));
		}
	}
}

Client *inserer(Client * abr, int numeroTel, int prixAppel){
	Client* prec, *temp;
	//cas arbre vide
	if(abr == NULL){
		abr = createClient(numeroTel, 1, prixAppel);
		printf("Salut\n");
		return abr;
	}
	prec = NULL;
	temp = abr;
	while(temp != NULL){
		prec = temp;
		//cas ou le noeud existe deja
		if(temp->numero == numeroTel){
			temp->coutTot += prixAppel;
			temp->nbAppel ++;
			return abr;
		}else if(numeroTel < temp->numero){
			temp = prec->filsG;
		}else{
			temp = prec->filsD;
		}
	}

	//cas ou il faut inserer le nouveau noeud
	if(numeroTel < prec->numero){
		prec->filsG = createClient(numeroTel, 1, prixAppel);
	}else{
		prec->filsD = createClient(numeroTel, 1, prixAppel);
	}
	return abr;
}

Client *supprimerClient(Client * abr, int numeroTel){
	Client* prec, *temp;
	//cas arbre vide
	if(abr == NULL){
		printf("Arbre vide, aucune suppression\n");
		return abr;
	}
	prec = NULL;
	temp = abr;

	//on parcours l'arbre jusqu'a trouver le noeud ou arriver a la fin d'une branche
	while(temp != NULL){

		//cas ou on a trouve le noeud
		if(temp->numero == numeroTel){

			//cas ou temp a 0 ou 1 fils
			if((temp->filsD == NULL) || (temp->filsG == NULL)){
				Client* fils;
				fils = (temp->filsD == NULL)?(temp->filsG):(temp->filsD);

				if(prec == NULL){
					abr = fils;
				}else{
					if(prec->filsD == temp){
						prec->filsD = fils;
					}else{
						prec->filsG = fils;
					}
				}
				//on libère le noeud
				free(temp);
			}else{//cas ou temp a 2 fils
				prec = detache_successeur(temp);
				//on copie les valeurs du succeseur dans le noeud a supprimer
				temp->numero = prec->numero;
				temp->nbAppel = prec->nbAppel;
				temp->coutTot = prec->coutTot;

				//on libère le noeud
				free(prec);
			}

			printf("Le noeud %d a ete supprime\n",numeroTel);
			return abr;

		//cas ou on continue a chercher
		}else if(numeroTel < temp->numero){
			prec = temp;
			temp = prec->filsG;
		}else{
			prec = temp;
			temp = prec->filsD;
		}
	}

	//on est sorti de la boucle, le noeud n'est pas dans l'arbre
	printf("Client %d non present dans l'arbre, aucune suppression\n", numeroTel);
	return abr;
}


void parcourirInfixe(Client * abr){
	if(abr != NULL){
		if(abr->filsG!=NULL){
			parcourirInfixe(abr->filsG);
		}

		printf("Client num : [%d], nbAppel : %d, coutTot: %d\n",abr->numero,abr->nbAppel,abr->coutTot);

		if(abr->filsD!=NULL){
			parcourirInfixe(abr->filsD);
		}
	}else{
		printf("Arbre vide, rien a parcourir\n");
	}
}

//fonction qui renvoi le min d'un sous arbre, on fournis le depart du sous arbre
// et son pere ainsi que le tableau ou l'on mettra le resultat
void min(Client * min, Client * pere_min, Client *buffer[2]){
	while(min->filsG != NULL){
		pere_min = min;
		min = min->filsG;
	}
	buffer[0] = min;
	buffer[1] = pere_min;
}

//le pere doit avoir un fils droit
Client * detache_successeur(Client* pere){
	Client * buffer[2];
	min(pere->filsD,pere,buffer);

	if(buffer[1]->filsD == buffer[0]){
		buffer[1]->filsD = buffer[0]->filsD;
	}else{
		buffer[1]->filsG = buffer[0]->filsD;
	}

	buffer[0]->filsD = NULL;
	return buffer[0];
}
