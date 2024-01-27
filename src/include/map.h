#ifndef _MAP_H
#define _MAP_H

#include <stdio.h>
#include <stdlib.h>
#include "bmp2map.h"
#include "sac.h"
#include <math.h>       

/* definition d'une matrice pour la map d'occupation */
typedef struct s_matrice{
    int row;
    int col;
    int **data;
} Matrice;
typedef Matrice Map; /* C'est plus clair ainsi ? */


/* convertir une image rgb en carte d'occupation (blanc=(255,255,255)->0, autre->1) */
Map *img_rgb2map_occup(Image* I);

/* convertion inverse de la pr�c�dente */
Image *map_occup2img_rgb(Map* m);

/* allouer_matrice */
Matrice *allouerMatrice(int row, int col);

/* free carte d'occupation */
void freeMatrice(Matrice *m);

/* copier une matrice dans une autre */
void copy_Matrice(Matrice *source, Matrice *destination);

//creer un mask de type disque de rayon r 
Matrice* createmask(int r);

// Fonction qui fait la convolution d'une image par un filtre
Matrice* convolution (Matrice* orig , Matrice* filtre ) ;

// Fonction qui calcule les pixels libres
Map * computeFreeMap ( Map * mapDilatee) ; 

// Marque le pixel comme visite
void mark_visited ( Map * map , int x , int y ) ;

// Fonction qui print la map / Matrice
void print_map(Matrice* map) ; 

// Fonction qui marque les directions
void mark_directions(Map* map , int x , int y , int x_init , int y_init) ;

#endif