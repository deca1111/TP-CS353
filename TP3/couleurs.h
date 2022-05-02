#ifndef COULEURS
#define COULEURS

#include <stdio.h>

//clear screen
#define clrscr() printf("\033[H\033[2J")

//defini la couleur d'ecriture dans la console
#define couleur(param) printf("\033[%sm",param)

#endif
