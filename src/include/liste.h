#ifndef _LISTE_H
#define _LISTE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct pts_dispo {
  int x;
  int y;
  float dist;
  struct pts_dispo *prev;
  struct pts_dispo *next;
} pts_dispo;

// Met un point dans la liste
void insert(pts_dispo **head,  int x , int y , float dist) ;

// Libere la memoire de la liste
void free_list(pts_dispo *head) ;

// Retourne le noeud avec la plus petite distance
pts_dispo* first(pts_dispo** head) ;

// Calcule la distance entre deux points
float distance_2q(int x1, int y1, int x2, int y2) ;

#endif