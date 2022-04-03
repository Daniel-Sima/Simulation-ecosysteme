#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h"

#define NB_PRED 10
#define NB_PROI 10

int main(void){
	srand(time(NULL));
	Animal *listeProies = NULL;
	Animal *listePredateurs = NULL;
	for(int i = 0; i < NB_PROI ; i++) // 5 proies aléatoires 
		listeProies = ajouter_en_tete_animal(listeProies, creer_animal((int)rand()%SIZE_X, (int)rand()%SIZE_Y, 10.00));
	for(int i = 0; i < NB_PRED; i++) // 5 predateurs aléatoires
		listePredateurs = ajouter_en_tete_animal(listePredateurs, creer_animal((int)rand()%SIZE_X, (int)rand()%SIZE_Y, 10.00));
		
	
	printf("[ECOSYSTEM 1 - SANS AGITATION]\n");
	printf("Nombre de predateurs : %d  | Nombre de proies : %d\n", compte_animal_it(listeProies), compte_animal_rec(listePredateurs));
	affichage_ecosysteme(listeProies, listePredateurs);	
	enlever_animal(&listeProies, listeProies); // on enleve la 1ere proie 
	enlever_animal(&listePredateurs, listePredateurs); //on enleve le 1er predateur
	affichage_ecosysteme(listeProies, listePredateurs);	
	printf("Nombre de predateurs : %d  | Nombre de proies : %d\n", compte_animal_it(listeProies), compte_animal_rec(listePredateurs));
	
	
	printf("\n\n[ECOSYSTEM 2 - BCP AGITATION]\n");  // pour tester bouger_animaux
	bouger_animaux(listeProies);
	bouger_animaux(listePredateurs);
	affichage_ecosysteme(listeProies, listePredateurs);	
	bouger_animaux(listeProies);
	bouger_animaux(listePredateurs);
	affichage_ecosysteme(listeProies, listePredateurs);	

	printf("\n\n[ECOSYSTEM 3 - REPRODUCTION]\n");  // leur nombre 2x
	reproduce(&listeProies, 1);
	reproduce(&listePredateurs, 1);
	bouger_animaux(listeProies);
	bouger_animaux(listePredateurs);
	affichage_ecosysteme(listeProies, listePredateurs);	
	
	
	

	listeProies = liberer_liste_animaux(listeProies);
	listePredateurs = liberer_liste_animaux(listePredateurs);
	
	
	return 0;
}
