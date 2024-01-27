#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "sac.h"
#include "liste.h"

// Retourne les coordonees du pixel le plus proche de l'objectif parmis les pixels adjacents
int* ppvLibre ( Map * map , int x , int y , int x_obj , int y_obj ) ; 

// Fonction qui calcule la distance entre un pixel et un objet
float* computeDist(int x , int y , int x_obj , int y_obj ) ; 

// Algorithme de recherche heuristique
int heuristic_method (Sac* stack , Map* map , int x , int y , int x_obj , int y_obj) ;

// Algorithme de Best First Path
int BFP (pts_dispo** points ,Map* map, Map* map_dir ,Sac * stack , int x_init , int y_init , int x_obj , int y_obj) ;

// Calcule la trajectoire à partir de la map des directions et met les points dans sac
void calcule_trajectoire( Sac* stack , Map* M_dir , int x_init , int y_init , int x_obj , int y_obj ) ; 


