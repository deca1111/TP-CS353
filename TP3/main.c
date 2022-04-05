/******************************************************************************
 * Programme de test du gestionnaire de magasin                               *                                                               *
 ******************************************************************************/

#include "store.h"
#include <stdio.h>


int main()
{
		clrscr();
		
    printf("****** Gestionnaire de magasin ******\n");
    init();
		insertItem(10001,"SUCRE - 500G",1.5);
		insertItem(10002,"SUCRE - 1KG",2.2);
		insertItem(10003,"SUCRE - 10KG",20);
		insertItem(583,"POIVRE - VRAC 100G",1.27);
    dumpItems();
    printf("\n\n=======Gestionnaire de magasin ======\n");

    return 0;
}
