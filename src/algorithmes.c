#include "./include/algorithmes.h"


// Fonction qui calcule la distance entre un pixel et un objet
float* computeDist(int x , int y , int x_obj , int y_obj ){
    if ( x < 0 || y < 0 || x_obj < 0 || y_obj < 0)
    {
        fprintf(stderr,"Error in computeDist : Parametres invaldes\n");
        exit(1);
    }

    // Tableau des distances
    float * res = malloc(8*sizeof(float)) ; 

    // Tableau Offset
    int offsets_xy[2][8]={{1,1,0,-1,-1,-1,0,1},{0,-1,-1,-1,0,1,1,1}};

    // Parcours de chaque Pixel Voisin
    for (int i = 0; i < 8; i++)
    {
        int x_pixelV = x + offsets_xy[0][i] ; // X du pixel voisin
        int y_pixelV = y + offsets_xy[1][i] ; // Y du pixel voisin
        res[i] = sqrt( ( x_obj - x_pixelV )*( x_obj - x_pixelV ) + ( y_obj - y_pixelV )*( y_obj - y_pixelV ) ) ;
    }
    
    return res ; 
}

// Retourne les coordonees du pixel le plus proche de l'objectif parmis les pixels adjacents
int* ppvLibre ( Map* map , int x , int y , int x_obj , int y_obj ) {
    
    // Tableau Offset
    int offsets_xy[2][8]={{1,1,0,-1,-1,-1,0,1},{0,-1,-1,-1,0,1,1,1}};

    // Tableau des distances 
    float* distances_pixels = computeDist( x , y , x_obj , y_obj ) ; 

    // Tableau contenant les coordonees du pixel le plus proche 
    int * coord = malloc( 2 * sizeof(int) ) ;

    // Initialisation du tableau au coordonees du 1er pixel
    int indice = 0 ; // Indice du pixel dont les coord se trouve dans le tableau 

    // Met les coordonees du premier pixel atteignable 
    for (int i = 0; i < 8 ; i++)
    {
        if (map->data[x + offsets_xy[0][i]][y + offsets_xy[1][i]] == 0)
        {
            indice = i ; 
            coord[0] = x + offsets_xy[0][i] ; 
            coord[1] = y + offsets_xy[1][i] ; 
            break;
        }
    }
    
    // Trouver le pixel le plus proche parmis les pixels atteignables 
    for (int i = indice; i < 8; i++)
    {
        if ( distances_pixels[indice] > distances_pixels[i] && map->data[x + offsets_xy[0][i]][y + offsets_xy[1][i]] == 0 )
        {
            indice = i ; 
            coord[0] = x + offsets_xy[0][i] ; 
            coord[1] = y + offsets_xy[1][i] ; 
        }  
    }
	free(distances_pixels) ; 
    return coord ; 
}

// Algorithme de recherche heuristique
int heuristic_method (Sac* stack , Map* map , int x , int y , int x_obj , int y_obj) {
    int new_x = x ; 
    int new_y = y ; 

    for (int i = 0; i < stack->size; i++)
    {
        int* new_point = ppvLibre(map , new_x , new_y , x_obj , y_obj) ; // Cherche le point le plus proche de l'objectif
        new_x = new_point[0] ;
        new_y = new_point[1] ; 
        push(stack , new_x , new_y ) ;  // Empile le point le plus proche de l'objectif
        stack->nb ++ ; 

        if (new_x == x_obj && new_y == y_obj)
        {
            return 1; // Retourne 1 si on a atteint l'objectif
        } 
		free(new_point) ;
    }
	
    return 0 ; // Retourne 0 si on n'a pas atteint l'objectif
}

// Calcule la trajectoire à partir de la map des directions et met les points dans sac
void calcule_trajectoire( Sac* stack , Map* M_dir , int x_init , int y_init , int x_obj , int y_obj ) {
	// Traitement de cas ou la map n'est pas initialisee ou la pile est vide
	if (stack == NULL ) {
        fprintf(stderr,"Erreur in calcule_trajectoire : Stack est vide\n");
        exit(1); 
    }
	if (M_dir == NULL) {
        fprintf(stderr,"Erreur in calcule_trajectoire : Map n'est pas initialisee\n");
        exit(1);
    }

	int back_offsets_xy[2][8]={{-1,-1,0,1,1,1,0,-1},{0,1,1,1,0,-1,-1,-1}};
	int x = x_obj ; 
	int y = y_obj ;
	push(stack , x , y) ; // Empile l'objectif

	while (x != x_init && y != y_init) { // tant qu'on n'est pas revenu à la position initiale
		int dir = M_dir->data[y][x] -1 ;
		x = x + back_offsets_xy[0][dir] ; // x du point auquel le point actuelle pointe
		y = y + back_offsets_xy[1][dir] ; // y du point auquel le point actuelle pointe
		push(stack , x , y) ; // Empile le nouveau point
	}
}

// Algorithme de Best First Path
int BFP (pts_dispo** points ,Map* map, Map* map_dir , Sac* stack ,  int x_init , int y_init , int x_obj , int y_obj)  {
	// Traitement de cas ou la map n'est pas initialisee ou la liste des points disponibles est vide
	if (map == NULL) {
        fprintf(stderr,"Erreur is BFP : Map n'est pas initialisee\n");
        exit(1);
    }
	if (map_dir == NULL) {
        fprintf(stderr,"Erreur in BFP : map_dir n'est pas initialisee\n");
        exit(1);
    }
	if (stack == NULL ) {
        fprintf(stderr,"Erreur in BFP : Stack est vide\n");
        exit(1); 
    }
	int offsets_xy[2][8]={{1,1,0,-1,-1,-1,0,1},{0,-1,-1,-1,0,1,1,1}} ;

	insert(points , x_init , y_init , distance_2q(x_init , y_init , x_obj , y_obj )) ; 	// Met le point initial dans la liste des points disponibles 	
	mark_visited(map ,x_init , y_init) ; // Marque le point initial comme visité ( mark_visited(q_init) )
	int SUCCESS = 0 ;

	while (*points != NULL && SUCCESS == 0) { // tant qu'il y a des points disponibles et qu'on n'a pas atteint l'objectif ( while (!empty(OPEN) and !SUCCESS) )
		pts_dispo* pt = first(points) ;	// Prend le point avec la plus petite distance de la liste des points disponibles ( q = GET_FIRST(OPEN) )
		int x = pt->x ;  						
		int y = pt->y ; 
		float distance = pt->dist ;
		mark_directions(map_dir , y , x, x_init , y_init) ;

		for (int i = 0; i < 8; i++) { // pour chaque point adjacent à q ( for every node q_n adjacent to q )
			int x_temp = x + offsets_xy[0][i] ;  					
			int y_temp = y + offsets_xy[1][i] ;
			float distance_adj = distance_2q( x_temp , y_temp , x_obj , y_obj) ; 

			if (distance_adj < distance && map->data[y_temp][x_temp] == 0 ) { // si le pixel adjacent est libre et plus proche de l'objectif que le pixel actuelle ( U(q_n)<S and q_n is not visited  )
				insert(points , x_temp , y_temp , distance_adj) ; // Met le point dans la liste des points disponibles 
				mark_visited(map , x_temp , y_temp) ; 				//  Marque le point comme visité 
				if ( x_temp == x_obj && y_temp == y_obj) 			// si le point adjacent est l'objectif ( q_n = q_goal )
				{
					SUCCESS = 1;
                    calcule_trajectoire(stack , map_dir , x_init , y_init , x_obj , y_obj ) ; // Calcule la trajectoire à partir de la map des directions et met les points dans sac
					printf("Chemin trouvé\n");
				}
			}
		}
	}
	if (SUCCESS == 0) {
		printf("Chemin non trouvé\n");
	}
	return SUCCESS;
}
