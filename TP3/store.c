/******************************************************************************
 * Implementation du module gestion de stock d'un magasin
 * avec une table de hachage
 ******************************************************************************/

#include "store.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

static Item hash_table[TABLE_SIZE];
static Item* hash_table_index[TABLE_SIZE];
/*----------------------------------------------------------------------------
 * Cette fonction initialise le tableau hash_table
 * en positionnant tous les elements à NULL_ITEM
 *----------------------------------------------------------------------------*/
void init()
{
    int i;
    for(i=0;i<TABLE_SIZE;i++)
    {
        hash_table[i].status = NULL_ITEM;
        hash_table[i].price = 0.00f;
        hash_table[i].code = 0;
				hash_table_index[i] = NULL;
    }
}


/*----------------------------------------------------------------------------
 * Cette fonction calcule la valeur de hachage pour le produit itemCode
 *----------------------------------------------------------------------------*/
uint32_t hashkey(uint32_t itemCode,uint32_t nbTry)
{
    return (((itemCode%TABLE_SIZE)+nbTry*(1+itemCode%(TABLE_SIZE -1)))%TABLE_SIZE);
}

/*----------------------------------------------------------------------------
 * Cette fonction insère le produit indiqué dans la table de hachage.
 * Si le produit est inséré avec succès, alors la fonction retourne SUCCESS (0)
 * Si le produit existe déjà dans la table, alors la fonction retourne INSERT_ALREADY_EXIST (-1),
 * et la table de hachage n'est pas modifiée
 * Si la table est pleine, alors la fonction retourne TABLE_FULL (-2).
 *----------------------------------------------------------------------------*/
int insertItem(uint32_t itemCode, char* itemName, float itemPrice){
	Item *itemDeleted, *selectedItem;
	int deletedLibre = 0;
	for(uint32_t i = 0; i < TABLE_SIZE; i++){

		 selectedItem = &hash_table[hashkey(itemCode, i)];

		 if (selectedItem->status == NULL_ITEM) {
			 if (deletedLibre) {
				 selectedItem = itemDeleted;
			 }
			 selectedItem->code = itemCode;
			 selectedItem->status = USED_ITEM;
			 strcpy(selectedItem->name, itemName);
			 selectedItem->price = itemPrice;

			 insertItemIndex(selectedItem);
			 return SUCCESS;
		 }

		 if ((selectedItem->status == USED_ITEM) && ( selectedItem->code == itemCode)) {
			 return INSERT_ALREADY_EXIST;
		 }

		 if ((selectedItem->status == DELETED_ITEM) && (deletedLibre == 0)) {
			 itemDeleted = selectedItem;
			 deletedLibre = 1;
		 }
	}

	if (deletedLibre) {
		itemDeleted->code = itemCode;
		itemDeleted->status = USED_ITEM;
		strcpy(itemDeleted->name, itemName);
		itemDeleted->price = itemPrice;

		insertItemIndex(itemDeleted);
		return SUCCESS;
	}

	return TABLE_FULL;
}

/*----------------------------------------------------------------------------
 * fonction de suppression d'un produit du magasin
 * Si le produit est supprimé avec succès, alors la fonction retourne SUCCESS (0)
 * Si le produit n'existe pas, alors la fonction retourne DELETE_NO_ROW (-4)
 *----------------------------------------------------------------------------*/
int suppressItem(unsigned int itemCode){
	Item * item;
	for(uint32_t i = 0; i < TABLE_SIZE; i++){
		item = &hash_table[hashkey(itemCode, i)];
		if(item->status == NULL_ITEM){
			return DELETE_NO_ROW;
		}
		if((item->code == itemCode) && (item->status == USED_ITEM)){
			item->status = DELETED_ITEM;
			return SUCCESS;
		}
	}
	return DELETE_NO_ROW;
}

/*----------------------------------------------------------------------------
 * Pour chaque produit, cette fonction affiche sur une ligne
 * le code du produit
 * son libellé
 * son prix
 * son index dans la table de hashage
 * sa valeur de hash
 *----------------------------------------------------------------------------*/
void dumpItems(){
	printf("\n+------------+--------------------------------+----------+--------+\n|");
	couleur("7;1");
	printf("    CODE    |            LIBELLE             |   PRIX   | INDEX  ");
	couleur("0");
	printf("|\n+------------+--------------------------------+----------+--------+\n");
	for(uint32_t i = 0; i < TABLE_SIZE; i++){
		if(hash_table[i].status == USED_ITEM){
			char code[20];
			sprintf(code,"%d",hash_table[i].code);
			char prix[20];
			sprintf(prix,"%3.2f",hash_table[i].price);
			char index[20];
			sprintf(index,"%d",i);

			//on fait un joli tableau
			printf("|%*s%*s%*s|%*s%*s%*s|%*s%*s%*s|%*s%*s%*s|\n",
				(int)(12-strlen(code))/2,"",
				(int)(strlen(code)+(strlen(code)%2)),code,
				(int)(12-strlen(code))/2,"",
				(int)(32-strlen(hash_table[i].name))/2,"",
				(int)(strlen(hash_table[i].name)+(strlen(hash_table[i].name)%2)),hash_table[i].name,
				(int)(32-strlen(hash_table[i].name))/2,"",
				(int)(10-strlen(prix))/2,"",
				(int)(strlen(prix)+(strlen(prix)%2)),prix,
				(int)(10-strlen(prix))/2,"",
				(int)(8-strlen(index))/2,"",
				(int)(strlen(index)+(strlen(index)%2)),index,
				(int)(8-strlen(index))/2,""
			);
			printf("+------------+--------------------------------+----------+--------+\n");
		}
	}
}


/*----------------------------------------------------------------------------
 * Cette fonction trouve le produit dont le code est itemCode.
 * Cette fonction retourne NULL si le produit n'existe pas.
 * Cette fonction retourne un pointeur vers le produit si le produit existe.
 *----------------------------------------------------------------------------*/
Item *getItem(unsigned int itemCode){
	Item * item;
	for(uint32_t i = 0; i < TABLE_SIZE; i++){
		item = &hash_table[hashkey(itemCode, i)];
		if(item->status == NULL_ITEM){
			return NULL;
		}
		if((item->code == itemCode) && (item->status == USED_ITEM)){
			return item;
		}
	}
	return NULL;
}

/*----------------------------------------------------------------------------
 *  fonction de mise à jour d'un produit :
 * Si le produit est mis à jour avec succès, alors la fonction retourne SUCCESS (0)
 * Si le produit n'existe pas, alors la fonction retourne UPDATE_NO_ROW (-5)
 *----------------------------------------------------------------------------*/
int updateItem(unsigned int itemCode, char *itemName, float itemPrice) {
	Item * item = getItem(itemCode);
	if(item != NULL){
		strcpy(item->name, itemName);
		item->price = itemPrice;
		return SUCCESS;
	}
	return UPDATE_NO_ROW;
}



/*----------------------------------------------------------------------------
 * la fonction de réorganisation in situ:
 *----------------------------------------------------------------------------*/
void rebuildTable(){
	Item* selectedItem, *newLocation, *temp = malloc(sizeof(Item));
	int essai;
	//on commence par passer toues les clefs a "sale"
	for(int i=0; i<TABLE_SIZE; i++){
		if(hash_table[i].status == USED_ITEM){
			hash_table[i].dirty = 1;
		}
	}
	//on parcours la table a la recherche de cles sales
	for(int i=0; i<TABLE_SIZE; i++){
		selectedItem = &hash_table[i];
		if((selectedItem->status == USED_ITEM) && (selectedItem->dirty == 1)){
			essai = 0;
			newLocation = &hash_table[hashkey(selectedItem->code, essai)];
			if((newLocation->status == NULL_ITEM) || (newLocation->status == DELETED_ITEM)){
				//on insere la cle ancienement sale dans la case vide
				*newLocation = *selectedItem;
				newLocation->dirty = 0;
				selectedItem->status = NULL_ITEM;
			}else if((newLocation->status == USED_ITEM) && (newLocation->dirty == 1)){
				//on echange la cle à inserer avec la cle sale
				*temp = *newLocation;
				*newLocation = *selectedItem;
				newLocation->dirty = 0;
				selectedItem->status = NULL_ITEM;
				//on reinsert la cle sale
				if(insertItem(temp->code, temp->name, temp->price) != 0){
					printf("Problème de réinsertion\n");
					exit(0);
				}
				getItem(temp->code)->dirty = 0;
			}else{
				*temp = *selectedItem;
				selectedItem->status = NULL_ITEM;
				if(insertItem(temp->code, temp->name, temp->price) != 0){
					printf("Problème de réinsertion\n");
					exit(0);
				}
				getItem(temp->code)->dirty = 0;
			}
		}
	}
	free(temp);
}

/*----------------------------------------------------------------------------
 * fonction de recherche par noms
 *----------------------------------------------------------------------------*/
Result *findItem(char* itemName){
	Result* head = NULL, *new;
	//parcours toute la table de hashage et remplis au la liste chainee de resultat
	for(int i = 0; i < TABLE_SIZE; i++){
		if(strcmp(itemName, hash_table[i].name) == 0){
			if(head == NULL){//initialisation de la tete
				head = (Result*) malloc(sizeof(Result));
				head->next = NULL;
				head->item = &hash_table[i];
			}else{//insertion en tete
				new = (Result*) malloc(sizeof(Result));
				new->next = head->next;
				head->next = new;
				new->item = &hash_table[i];
			}
		}
	}
	return head;
}

//insert le pointeur vers item dans la deuxieme table de hashage
int insertItemIndex(Item* item){
	int index;
	for(int i=0; i<TABLE_SIZE; i++){
		index = hashkey(hashIndex(item->name, strlen(item->name)),i);
		if(hash_table_index[index] == NULL){
			hash_table_index[index] = item;
			return SUCCESS;
		}
	}
	return TABLE_FULL;
}

//permet de retrouver tout les item d'apres le nom, et les met dans une liste chainee
Result *findItemWithIndex(char* itemName){
	Result* head = NULL, *new;
	int index;
	for(int i = 0; i < TABLE_SIZE; i++){
		index = hashkey(hashIndex(itemName, strlen(itemName)),i);
		if((hash_table_index[index] != NULL) && (hash_table_index[index]->status == NULL_ITEM)){
			return head;
		}
		if((hash_table_index[index] != NULL) && (hash_table_index[index]->status == USED_ITEM) && (strcmp(hash_table_index[index]->name, itemName)==0)){
			if(head == NULL){//initialisation de la tete
				head = (Result*) malloc(sizeof(Result));
				head->next = NULL;
				head->item = hash_table_index[index];
			}else{//insertion en tete
				new = (Result*) malloc(sizeof(Result));
				new->next = head->next;
				head->next = new;
				new->item = hash_table_index[index];
			}
		}
	}
	return head;
}

//fonction de hashage selon une chaine de caractere
unsigned int hashIndex(const char *buffer, int size) {
	unsigned int h = 0;
	for (int i=0; i<size; i++){
		h = ( h * 1103515245u ) + 12345u + buffer[i];
	}
	return h/2;
}

//FONCTIONS test et utiles

//free la liste chainee de resultat
void freeResult(Result* result){
	if(result->next != NULL){
		freeResult(result->next);
	}
	free(result);
}

//compte les item avec le status "DELETED_ITEM"
void countDeleted(){
	int nbDeleted = 0;
	for(int i =0; i<TABLE_SIZE; i++){
		if(hash_table[i].status == DELETED_ITEM){
			nbDeleted++;
		}
	}
	printf("Nombre d'item deleted : %d\n",nbDeleted );
}

//fonction de test
void test(){
	init();
	//remplissage de la table
	for (int i = 0; i < TABLE_SIZE; i++) {
		if(insertItem(i,"ITEM",(float)i/10) == -2){
			printf("TABLE PLEINE, %d non insere\n", i);
		}
	}
	printf("=================================================================================\n\t\t\tAFFICHAGE DE LA TABLE PLEINE\n=================================================================================\n");
	dumpItems();
	//on vide la table
	for (int i = 0; i < TABLE_SIZE; i++) {
		if(suppressItem(i) == -4){
			printf("ITEM inexistant, %d non delete\n", i);
		}
	}
	printf("=================================================================================\n\t\t\tAFFICHAGE DE LA TABLE VIDE\n=================================================================================\n");
	dumpItems();
	insertItem(0,"New Item",666);
	dumpItems();
	printf("Adresse de l'item 0 : %p\n", getItem(0));
	printf("Adresse de l'item 100 : %p\n", getItem(100));
	updateItem(0,"Old Item",999);
	dumpItems();

	//test de la fonction rebuildtable
	init();
	uint32_t keys[TABLE_SIZE];
	srand( time( NULL ) );

	//remplissage aleatoire
	for (int i = 0; i < 1000; i++) {
		keys[i] = rand() % (4294967296-1);
		insertItem(keys[i], "ITEM", (float) (i%1000/10));
	}
	//suppression partielle
	for (int i = 0; i < 800; i++) {
		suppressItem(keys[i]);
	}

	//remplissage aleatoire
	for (int i = 0; i < 800; i++) {
		keys[i] = rand() % (4294967296-1);
		insertItem(keys[i], "ITEM", (float) (i%1000/10));
	}
	//suppression partielle
	for (int i = 0; i < 400; i++) {
		suppressItem(keys[i]);
	}

	printf("\n=================================================================================\n\t\t\tRESULTAS DU REBUILD\n=================================================================================\n");
	printf("AVANT REBUILD :\n");
	//on compte les item delete
	countDeleted();
	rebuildTable();
	printf("APRES REBUILD :\n");
	countDeleted();

	init();
	printf("\n=================================================================================\n\t\t\tPARTIE 2 : RECHERCHE PAR NOMS\n=================================================================================\n");
	insertItem(1,"SUCRE",1.5);
	insertItem(2,"SUCRE",2.2);
	insertItem(3,"CONFITURE",20);

	Result* tete, *next;
	tete = findItem("SUCRE");
	next = tete;
	printf("On recherche le sucre :\n");
	while(next!= NULL){
		printf("Item code : %d, Item name : %s\n", next->item->code, next->item->name);
		next = next->next;
	}
	freeResult(tete);
	tete = findItemWithIndex("SUCRE");
	next = tete;
	printf("On recherche le sucre :\n");
	while(next!= NULL){
		printf("Item code : %d, Item name : %s\n", next->item->code, next->item->name);
		next = next->next;
	}
	freeResult(tete);
	suppressItem(1);
	tete = findItemWithIndex("SUCRE");
	next = tete;
	printf("On recherche le sucre :\n");
	while(next!= NULL){
		printf("Item code : %d, Item name : %s\n", next->item->code, next->item->name);
		next = next->next;
	}
	freeResult(tete);
}
