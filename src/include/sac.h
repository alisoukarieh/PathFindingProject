#ifndef _SAC_H
#define _SAC_H

#include <stdio.h>
#include <stdlib.h>
#include "bmp2map.h"
#include "map.h"

/* gestion d'un sac LIFO/FIFO (circulaire) */
typedef struct sac{
  int *x; // position en col
  int *y; //pos en ligne
  int nb;
  int size;
  int begin;
  int end;
} Sac;

/* dessiner une trajectoire de la pile (sans "pop") dans l'image */
void dessine_traj(Image* I,Sac* s, char R, char G, char B);

// Alloue un sac de taille size
Sac* allocSac(int size);

// Libère la mémoire allouée pour le sac
void freeSac(Sac* s);

// Retourne 1 si le sac est plein, 0 sinon
int sacPlein(Sac* s);

// Retourne 1 si le sac est vide, 0 sinon
int sacVide(Sac* s);

// Ajoute un point au sac
int push(Sac *s,int x, int y);

// Print les points du sac
void print_sac(Sac* sac);
/* TODO : pop_fifo(), pop_lifo(), pop_min() */

#endif