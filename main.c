/**************************************************************/
/* auteur : Xavier Clady (xavier.clady@sorbonne-universite.fr */
/**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./src/include/bmp2map.h"
#include "./src/include/map.h"
#include "./src/include/sac.h"
#include "./src/include/liste.h"
#include "./src/include/algorithmes.h"

#define R_DISK 5
#define I_WIDTH 256
#define I_LENGTH 256


int main_print_disque(){ 
	Matrice* disque = createmask(R_DISK) ; 
	for (int i = 0; i < disque->row ; i++)
	{
		for (int j = 0; j < disque->col; j++)
		{
			printf(" %d " , disque->data[i][j]) ; 
		}
		printf("\n") ; 
	}
	freeMatrice(disque) ; 
	return 0 ; 
}


int main_test_convulation(){ 

	Image* I = Charger("./src/maps/test_obstacles.bmp") ; // test_pbstacles.bmp = img 256*256 en BMP 24 bits
	Map* map_occup = img_rgb2map_occup(I); // conversion de l'image en grille d'occupation

	Matrice* disk = createmask(R_DISK) ; 
	Map * mapDilatee = convolution(map_occup , disk) ; 
	
	Image * I2 = map_occup2img_rgb(mapDilatee) ; 
	Sauver(I2 , "./OUTPUT/test_dilatation.bmp") ; 

	freeMatrice(disk) ; 
	freeMatrice(map_occup);
	freeMatrice(mapDilatee);
	DelImage(I);
	DelImage(I2);

	return 0 ; 
}



int main_test_computefree(){ 

	Image* I = Charger("./src/maps/test_obstacles.bmp") ; // test_pbstacles.bmp = img 256*256 en BMP 24 bits
	Map* map_occup = img_rgb2map_occup(I); // conversion de l'image en grille d'occupation

	Matrice* disk = createmask(R_DISK) ; 
	Map * mapDilatee = convolution(map_occup , disk) ; 
	Map* mapFree = computeFreeMap(mapDilatee) ; 
	Image * I2 = map_occup2img_rgb(mapFree) ; 
	Sauver(I2 , "./OUTPUT/test_dilatation.bmp") ; 

	freeMatrice(disk) ; 
	freeMatrice(map_occup);
	freeMatrice(mapDilatee);
	freeMatrice(mapFree) ; 
	DelImage(I);
	DelImage(I2);

	return 0 ; 
}

int main_heuristic(){ 

	//Init Sac
	Sac* stack =  allocSac(1500) ; 

	//Init Image
	Image* I = Charger("./src/maps/test_obstacles.bmp") ; // test_pbstacles.bmp = img 256*256 en BMP 24 bits
	Map* map_occup = img_rgb2map_occup(I); // conversion de l'image en grille d'occupation

	//Init Map free
	Matrice* disk = createmask(R_DISK) ; 
	Map * mapDilatee = convolution(map_occup , disk) ; 
	Map* mapFree = computeFreeMap(mapDilatee) ; 

	//Init Map Trajet 
	Image* I2 = CopieImage(I) ; 

	//Trajectoire Finder
	heuristic_method(stack , mapFree , 20 , 20 , 230 , 230) ; 

	//Trajectoire drawer
	dessine_traj(I2 , stack , 255 , 0 , 0 );
	Sauver(I2 , "./OUTPUT/tests_obstacles_heuristicRUN.bmp") ;


	freeMatrice(disk) ; 
	freeMatrice(map_occup);
	freeMatrice(mapDilatee);
	freeMatrice(mapFree) ; 
	DelImage(I);
	DelImage(I2);
	freeSac(stack);
	return 0 ; 
	
}

int main_heuristic_min_loc(){ 

	//Init Sac
	Sac* stack =  allocSac(1500) ; 

	//Init Image
	Image* I = Charger("./src/maps/test_min_loc.bmp") ; // test_pbstacles.bmp = img 256*256 en BMP 24 bits
	Map* map_occup = img_rgb2map_occup(I); // conversion de l'image en grille d'occupation

	//Init Map free
	Matrice* disk = createmask(R_DISK) ; 
	Map * mapDilatee = convolution(map_occup , disk) ; 
	Map* mapFree = computeFreeMap(mapDilatee) ; 

	//Init Map Trajet 
	Image* I2 = CopieImage(I) ; 

	//Trajectoire Finder
	heuristic_method(stack , mapFree , 20 , 20 , 230 , 230) ; 

	//Trajectoire drawer
	dessine_traj(I2 , stack , 255 , 0 , 0 );
	Sauver(I2 , "./OUTPUT/tests_min_loc_heuristicRUN.bmp") ;


	freeMatrice(disk) ; 
	freeMatrice(map_occup);
	freeMatrice(mapDilatee);
	freeMatrice(mapFree) ; 
	DelImage(I);
	DelImage(I2);
	freeSac(stack);
	return 0 ; 
	
}

int main_mark_direction(){ 
	Matrice* test = allouerMatrice (12 , 12) ;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			test->data[i][j] = 0 ; 

		}
	}
	mark_directions(test , 3 , 3 , 11 , 11) ;
	print_map(test) ;
	freeMatrice(test) ;
	return 0 ;
}

int main_test_BFP_petiteMatrice() { 
	Map* map_dir = allouerMatrice(12 , 12) ;
	Matrice* test = allouerMatrice (12 , 12) ;
	Sac* stack =  allocSac(12*12) ; 
	pts_dispo* points = NULL ;

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			map_dir->data[i][j] = 0 ; 
			test->data[i][j] = 0 ;
		}
	}
	BFP(&points , test ,map_dir , stack , 3 , 3 , 11 , 11) ; 
		
	free_list( points ) ;
	freeMatrice(map_dir) ;
	freeMatrice(test) ;
	freeSac(stack) ;
	return 0 ; 
}

int main_BFP() {
	//Init Sac
	Sac* stack =  allocSac(I_LENGTH * I_WIDTH) ;
	//init points
	pts_dispo* points = NULL ;
	//Init map_dir
	Map* map_dir = allouerMatrice(256 , 256) ;

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			map_dir->data[i][j] = 0 ; 
		}
	}
	//Init Image
	Image* I = Charger("./src/maps/test_obstacles.bmp") ; // test_pbstacles.bmp = img 256*256 en BMP 24 bits
	Map* map_occup = img_rgb2map_occup(I); // conversion de l'image en grille d'occupation

	//Init Map free
	Matrice* disk = createmask(R_DISK) ; 
	Map * mapDilatee = convolution(map_occup , disk) ; 
	Map* mapFree = computeFreeMap(mapDilatee) ; 
	
	//Init Map Trajet 
	Image* I2 = CopieImage(I) ; 
	//algo 
	if (BFP(&points , mapFree ,map_dir ,stack , 20 , 20 , 230 , 230))
	{
		//Trajectoire drawer
		dessine_traj(I2 , stack , 0 , 0 , 255 );
		Sauver(I2 , "./OUTPUT/test_obstacles_bfpRUN.bmp") ;
	} else {
		printf("Pas de chemin possible \n");
	}
	 
	freeMatrice(disk) ; 
	freeMatrice(map_dir) ; 
	freeMatrice(map_occup);
	freeMatrice(mapDilatee);
	freeMatrice(mapFree) ; 
	DelImage(I);
	DelImage(I2);
	freeSac(stack);
	return 0 ; 
}

int main_BFP_min_loc() {
	//Init Sac
	Sac* stack =  allocSac(I_LENGTH * I_WIDTH) ;
	//init points
	pts_dispo* points = NULL ;
	//Init map_dir
	Map* map_dir = allouerMatrice(256 , 256) ;

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			map_dir->data[i][j] = 0 ; 
		}
	}
	//Init Image
	Image* I = Charger("./src/maps/test_min_loc.bmp") ; // test_pbstacles.bmp = img 256*256 en BMP 24 bits
	Map* map_occup = img_rgb2map_occup(I); // conversion de l'image en grille d'occupation

	//Init Map free
	Matrice* disk = createmask(R_DISK) ; 
	Map * mapDilatee = convolution(map_occup , disk) ; 
	Map* mapFree = computeFreeMap(mapDilatee) ; 
	
	//Init Map Trajet 
	Image* I2 = CopieImage(I) ; 
	//algo 
	if (BFP(&points , mapFree ,map_dir ,stack , 20 , 20 , 230 , 230))
	{
		//print_sac(stack) ;
		//Trajectoire drawer
		dessine_traj(I2 , stack , 0 , 0 , 255 );
		Sauver(I2 , "./OUTPUT/test_min_loc_bfpRUN.bmp") ;
	} else {
		printf("Pas de chemin possible \n");
	}
	 
	freeMatrice(disk) ; 
	freeMatrice(map_dir) ; 
	freeMatrice(map_occup);
	freeMatrice(mapDilatee);
	freeMatrice(mapFree) ; 
	DelImage(I);
	DelImage(I2);
	freeSac(stack);
	return 0 ; 
}




int main() {
	// main_print_disque() ;
	main_test_convulation() ;
	// main_test_computefree() ;
	main_heuristic() ;
	main_heuristic_min_loc() ;
	// main_mark_direction() ;
	// main_test_BFP_petiteMatrice() ; 
	main_BFP() ;
	main_BFP_min_loc() ;

	return 0 ; 
} 