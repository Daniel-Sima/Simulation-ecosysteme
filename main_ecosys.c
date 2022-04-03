#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "ecosys.h"

#define TEMPS_ATTENTE 1000000 // 1sec
#define ITERATIONS_MAX 200
#define NB_PRED 20
#define NB_PROI 20




int main(void){
	
	FILE *fichier = fopen("Evol_Pop.txt", "w");
	
	srand(time(NULL));
	static int tab[SIZE_X][SIZE_Y] = {0};
	Animal *listProies = NULL;
	Animal *listPredateurs = NULL;
	for(int i = 0; i < NB_PROI; i++)
		listProies = ajouter_en_tete_animal(listProies, creer_animal((int)rand()%SIZE_X, (int)rand()%SIZE_Y, 10.0));
	for(int i = 0; i < NB_PRED; i++)
		listPredateurs = ajouter_en_tete_animal(listPredateurs, creer_animal((int)rand()%SIZE_X, (int)rand()%SIZE_Y, 10.0));
	

	for(int i = 0; i < ITERATIONS_MAX && compte_animal_rec(listProies) > 0; i++){
		fprintf(fichier, "%d %d %d\n", i, compte_animal_rec(listProies), compte_animal_rec(listPredateurs));
		printf("CPT = %d\n", i);
		rafraichir_monde(tab);
		rafraichir_predateurs(&listPredateurs, &listProies);
		rafraichir_proies(&listProies, tab);
		usleep(TEMPS_ATTENTE); // 1 sec
		affichage_ecosysteme(listProies, listPredateurs);
	} 
	
	liberer_liste_animaux(listProies);
	liberer_liste_animaux(listPredateurs);
	
	fclose(fichier);
	
	
	return 0;
} 
