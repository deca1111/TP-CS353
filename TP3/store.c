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
		if(item->code == itemCode){
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
	printf("|%60s|\n|   CODE   |            LIBELLE             |  PRIX  | INDEX |\n");
	for(uint32_t i = 0; i < TABLE_SIZE; i++){
		if(hash_table[i].status == USED_ITEM){
			printf("|%-10d|%-32s|%2.5f|%-7d|","",hash_table[i].status,hash_table[i].name,hash_table[i].price,i);
		}
	}
	printf("\n");
}


/*----------------------------------------------------------------------------
 * Cette fonction trouve le produit dont le code est itemCode.
 * Cette fonction retourne NULL si le produit n'existe pas.
 * Cette fonction retourne un pointeur vers le produit si le produit existe.
 *----------------------------------------------------------------------------*/
Item *getItem(unsigned int itemCode)
{
	return NULL;
}

/*----------------------------------------------------------------------------
 *  fonction de mise à jour d'un produit :
 * Si le produit est mis à jour avec succès, alors la fonction retourne SUCCESS (0)
 * Si le produit n'existe pas, alors la fonction retourne UPDATE_NO_ROW (-5)
 *----------------------------------------------------------------------------*/
int updateItem(unsigned int itemCode, char *itemName, float itemPrice)
{
	return SUCCESS;
}



/*----------------------------------------------------------------------------
 * la fonction de réorganisation in situ:
 *----------------------------------------------------------------------------*/
void rebuildTable()
{
}
