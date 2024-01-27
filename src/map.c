#include "./include/map.h"

/*******************************/
/* Fcts interface avec l'image */

void dessine_traj(Image* I,Sac* s, char R, char G, char B)
{
    int i=s->begin;
    Pixel p;
	p.r = R;
	p.g = G;
	p.b = B;

	while(i!=s->end) // parcours du sac sans "pop"
    {
        SetPixel(I,s->x[i],s->y[i],p);
        i=(i+1)%s->size;
    }
}

/************************************/
/* Gestion de la carte d'occupation */

/* convertir une image rgb en carte d'occupation (blanc=(255,255,255)->0, autre->1) */
Matrice *img_rgb2map_occup(Image* I)
{
    int i,j;
    Pixel p;
    Matrice *map_occup=NULL;

    if (I==NULL)
    {
        fprintf(stderr,"Error in img_rgb2map_occup : none image\n");
        exit(1);
    }

    map_occup=allouerMatrice(I->h,I->w);

    for (i=0;i<I->h;i++)
        for(j=0;j<I->w;j++)
        {
          p=GetPixel(I,i,j);
          if (p.r==255&&p.b==255&&p.r==255)
            map_occup->data[i][j]=0;
          else
            map_occup->data[i][j]=1;
        }

    return map_occup;
}

/* convertion inverse de la pr�c�dente */
Image *map_occup2img_rgb(Map* m)
{
    int i,j;
    Pixel p_blanc,p_noir;
    Image *I=NULL;

    if (m==NULL)
    {
        fprintf(stderr,"Error in map_occup2img_rgb : none map\n");
        exit(1);
    }

    I=NouvelleImage(m->row,m->col);
    p_blanc.r=255;p_blanc.g=255;p_blanc.b=255;
    p_noir.r=0;p_noir.g=0;p_noir.b=0;

    for (i=0;i<I->h;i++)
        for(j=0;j<I->w;j++)
        {
          if (m->data[i][j])
            SetPixel(I,i,j,p_noir);
          else
            SetPixel(I,i,j,p_blanc);
        }

    return I;
}

/* allouer_matrice */
Matrice *allouerMatrice(int row, int col)
{   
    if (row <= 0 || col <= 0) 
    {
        fprintf(stderr,"Error in allouerMatrice : Parametres invaldes\n");
        exit(1);
    }
    
    int i;
    Matrice *m=(Matrice*)malloc(sizeof(Matrice));
    m->row=row;
    m->col=col;

    m->data=(int**)malloc(row*sizeof(int*));
    for (i=0;i<row;i++)
        m->data[i]=(int*)malloc(col*sizeof(int));

    return m;
}
/* free carte d'occupation */
void freeMatrice(Matrice *m)
{
    if (m==NULL)
    {
        fprintf(stderr,"Error in freeMatrice : Matrice mal initialisee\n");
        exit(1);
    }
    int i;
    for (i=0;i<m->row;i++)
        free(m->data[i]);
    free(m->data);
    free(m);
}

/* copier une matrice dans une autre */
void copy_Matrice(Matrice *source, Matrice * destination)
{

    if (source == NULL || destination == NULL)
    {
        fprintf(stderr,"Error in copy_Matrice : Matrice mal initialisee\n");
        exit(1);
    }

    if ( source->row != destination->row || source->col != destination->col)
    {
        fprintf(stderr,"Error in copy_Matrice : Parametres invaldes\n");
        exit(1);
    }
    int i,j;
    for (i=0;i<source->row;i++)
    for(j=0;j<source->col;j++)
        destination->data[i][j]=source->data[i][j];

}


Matrice* createmask(int r) {
    if ( r <= 0)
    {
        fprintf(stderr,"Error in createmask : r invalde\n");
        exit(1);
    }
    
    int size = 2*r + 1 ; 
    Matrice* res = allouerMatrice(size , size) ;

    //p(x,y) appartient au disque si x^2 + y^2 < R
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            res->data[i][j] = (i-r)*(i-r) + (j-r)*(j-r) <= (r*r) ; 
        }
        
    }

    return res ; 
}

// Fonction qui fait la convolution d'une image par un filtre
Matrice* convolution(Matrice* orig,Matrice* filtre){
    if ( orig == NULL )
    {
        fprintf(stderr,"Error in convolution : Matrice orig mal initialisee\n");
        exit(1);
    }
    if (filtre == NULL)
    {
        fprintf(stderr,"Error in convolution : Matrice filtre mal initialisee\n");
        exit(1);
    }
    
    Matrice* res=allouerMatrice(orig->row,orig->col);
    int N=(filtre->row - 1) / 2; //1/2 taille horizontale
    int M=(filtre->row - 1) / 2; //1/2 taille verticale

    int i,j,n,m;

    
    for(i = 0; i < orig->row; i++)
        for(j = 0; j < orig->col; j++)
            res->data[i][j] = 1;

    
    // Parcours de l'image       
    for (i = N;i < orig->row - N; i++){ // Enleve les bords pour la convolution
        for(j = M; j < orig->col - M; j++){ // Assure que 0 <= i + n < orig->row
            // Initialisation
            res->data[i][j] = 0;
            // Convolution
            for(n = -N; n <= N; n++){
                for(m = -M; m <= M; m++){
                    res->data[i][j] += orig->data[i+n][j+m] * filtre->data[n+N][m+M];
                }
            }
        }
    }
    return res;
}

// Fonction qui calcule les pixels libres
Map * computeFreeMap ( Map * mapDilatee) {
    if ( mapDilatee == NULL )
    {
        fprintf(stderr,"Error in computeFreeMap : Matrice mapDilatee mal initialisee\n");
        exit(1);
    }

    Map * res = allouerMatrice(mapDilatee->row , mapDilatee->col) ; 
    for (int i = 0; i < mapDilatee->row; i++)
    {
        for (int j = 0; j < mapDilatee ->col; j++)
        {
            res->data[i][j] = mapDilatee ->data[i][j] > 0 ; // Seuille le resultat de la convulation
        }
        
    }
    return res ; 
}


// Marque le pixel comme visite
void mark_visited(Map *map, int x, int y){
    map->data[y][x] = 2 ; 
}



// Fonction qui print la map / Matrice
void print_map(Matrice* map) {
    if (map == NULL) {
        fprintf(stderr, "Error in print_map : Matrice map mal initialisee\n");
        exit(1);
    }
    int i, j;

    for (i = 0; i < map->row; i++) {
        for (j = 0; j < map->col; j++) {
            printf("%d ", map->data[i][j]);
        }
        printf("\n");
    }
}

// Fonction qui marque les directions
void mark_directions(Map* map , int x , int y , int x_init , int y_init) {
    if (map == NULL) {
        fprintf(stderr, "Error in mark_directions : Matrice map mal initialisee\n");
        exit(1);
    }
    if (x < 0 || y < 0 || x_init < 0 || y_init < 0) {
        fprintf(stderr, "Error in mark_directions : Parametres invaldes\n");
        exit(1);
    }
	int offsets_xy[2][8]={{1,1,0,-1,-1,-1,0,1},{0,-1,-1,-1,0,1,1,1}};

	for (int i = 0; i < 8; i++) // pour chaque point adjacent au point actuel
	{	
		int x_temp = x + offsets_xy[0][i] ;  // x du point adjacent
		int y_temp = y + offsets_xy[1][i] ;  //	y du point adjacent

		if ( map->data[y_temp][x_temp] ==  0 && (x_temp != x_init || y_temp != y_init) ) {  // si le point n'a pas ete marque et qu'il n'est pas le point initial
			map->data[y_temp][x_temp] =  i + 1 ; // on marque la direction du point initial
		}
	}
}