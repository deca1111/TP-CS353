/******************************************************************************
 * Implementation du module gestion de stock d'un magasin
 * avec une table de hachage
 ******************************************************************************/

#include "store.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static Item hash_table[TABLE_SIZE];
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
	Item* selectedItem, *newLocation;
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
				newLocation->code = selectedItem->code;
				newLocation->status = USED_ITEM;
				strcpy(newLocation->name, selectedItem->name);
				newLocation->price = selectedItem->price;
				newLocation->dirty = 0;
				selectedItem->status = NULL_ITEM;
			}else if((newLocation->status == USED_ITEM) && (newLocation->dirty == 1)){
				
			}
		}
	}
}


//FONCTIONS test et utiles


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
}
