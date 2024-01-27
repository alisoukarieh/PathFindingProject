#include "./include/sac.h"


/******************************/
/* Gestion d'un sac FIFO/LIFO */

// Alloue un sac de taille size
Sac* allocSac(int size)
{
    Sac* s=(Sac*)malloc(sizeof(Sac));
    s->size=size;
    s->nb=0;
    s->begin=0;
    s->end=0;
    s->x=(int*)malloc(s->size*sizeof(int));
    s->y=(int*)malloc(s->size*sizeof(int));
    return s;
}

// Libère la mémoire allouée pour le sac
void freeSac(Sac* s)
{
    if ( s == NULL)
    {
        fprintf(stderr, "Erreur dans freeSac: s is NULL\n") ;
        exit(1) ; 
    }
    
    free(s->x);
    free(s->y);
    free(s);
}

// Retourne 1 si le sac est plein, 0 sinon
int sacPlein(Sac* s)
{
    if ( s == NULL)
    {
        fprintf(stderr, "Erreur dans sacPlein: s is NULL\n") ;
        exit(1) ; 
    }
    return(s->nb==s->size);
}

// Retourne 1 si le sac est vide, 0 sinon
int sacVide(Sac* s)
{
    if ( s == NULL)
    {
        fprintf(stderr, "Erreur dans sacVide: s is NULL\n") ;
        exit(1) ; 
    }
    return(s->nb!=0);
}

// Ajoute un point au sac
int push(Sac *s,int x, int y)
{
    if ( s == NULL)
    {
        fprintf(stderr, "Erreur dans push: s is NULL\n") ;
        exit(1) ; 
    }
    if ( x < 0 || y < 0)
    {
        fprintf(stderr, "Erreur dans push: Coordonees negatifs\n") ;
        exit(1) ; 
    }
    if(sacPlein(s)) return 0;
    s->x[s->end]=x;
    s->y[s->end]=y;
    s->nb++;
    s->end=(s->end+1)%s->size;
    return 1;
}

// Print les points du sac
void print_sac(Sac* sac) {
  for (int i = 0; i < sac->nb; i++) {
    printf("(%d, %d)\n", sac->x[i], sac->y[i]);
  }
}



