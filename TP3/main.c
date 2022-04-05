/******************************************************************************
 * Programme de test du gestionnaire de magasin                               *                                                               *
 ******************************************************************************/

#include "store.h"
#include <stdio.h>


int main()
{
    //
    printf("****** Gestionnaire de magasin ******\n");
    init();
		printf("Insertion : %d\n", insertItem(10001,"SUCRE",10));
		printf("Insertion : %d\n", insertItem(10001,"SUCRE",10));
    dumpItems();
    printf("=======Gestionnaire de magasin ======\n");

    return 0;
}
