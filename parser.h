#ifndef parser_h
#define parser_h


	#include "stdio.h"
	#include "stdlib.h"
	#include "string.h"
	#include "ctype.h"
	#include "executor.h"


//La taille du buffer pour la lecture de lignes
extern int SIZE;

//Fonction qui prend une ligne de commande et essaye de l'executer
int traitement_ligne_de_commande(char *line);


int checkFormat(char *format);
int checkRGB(char *rgb);
//Vérifie si coords est de la forme (coord1,coord2)
int checkCoords(char *coords);

// Fonction qui prend une line, et la décompose en plusieurs arguments
char **line_split(char *line);

//Fonction qui l'absence d'une fonction
int error(char *cmd);

#endif