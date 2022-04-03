#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "ecosys.h"


/*--------------------------------------------------------------------------*/
/* Parametres globaux de l'ecosysteme */
const float p_ch_dir = 0.1; 
const int gain_energie_proie = 6;
const int gain_energie_predateur = 20;
const float p_reproduce_proie = 0.4;
const float p_reproduce_predateur = 0.5;
const int temps_repousse_herbe = 1;
const float p_manger = 0.1;
/*--------------------------------------------------------------------------*/
Animal *creer_animal(int x, int y, float energie){
	Animal *am = (Animal*) malloc(sizeof(Animal));
	assert(am); // On verifie que l'allocation dynamique a bien fonctionné
	am->x = x;
	am->y = y;
	am->energie = energie;
	am->dir[0] = rand()%3-1;  // rand()%(MAX-MIN+1)+MIN;
	am->dir[1] = rand()%3-1;
	am->suivant = NULL;
	
	return am;
}
/*--------------------------------------------------------------------------*/
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal){
	assert(animal);
	assert(!animal->suivant);  // verifie que c'est animal n'est pas une liste
	animal->suivant = liste;
	
	return animal;
}
/*--------------------------------------------------------------------------*/
unsigned int compte_animal_it(Animal *la){
	unsigned int cpt = 0;
	while(la){
		cpt++;
		la = la->suivant;
	}
	
	return cpt;
}
/*--------------------------------------------------------------------------*/
unsigned int compte_animal_rec(Animal *la){
	if(!la)
		return 0;
	
	return 1 + compte_animal_rec(la->suivant);
}
/*--------------------------------------------------------------------------*/
void affichage_ecosysteme(Animal *liste_proies, Animal *liste_predateur){
	int nbProies = 0, nbPredateurs = 0;
	char static ecos[SIZE_X][SIZE_Y];
	
	// Initialisation du tableau 
	for(int i = 0; i < SIZE_X; i++)
		for(int j = 0; j < SIZE_Y; j++)
			ecos[i][j] = ' ';
	
	/* On ajoute les proies */
	while(liste_proies){
		nbProies++;
		assert((liste_proies->x >= 0) && (liste_proies->x < SIZE_X) && (liste_proies->y >= 0) && (liste_proies->y < SIZE_Y));
		ecos[liste_proies->x][liste_proies->y] = '*';
		liste_proies = liste_proies->suivant;
	}
	
	/* On ajoute les predateurs */ 
	while(liste_predateur){
		nbPredateurs++;
		assert((liste_predateur->x >= 0) && (liste_predateur->x < SIZE_X) && (liste_predateur->y >= 0) && (liste_predateur->y < SIZE_Y));
		if((ecos[liste_predateur->x][liste_predateur->y] == '*') || (ecos[liste_predateur->x][liste_predateur->y] == '@'))
			ecos[liste_predateur->x][liste_predateur->y] = '@';
		else 
			ecos[liste_predateur->x][liste_predateur->y] = 'O';
		liste_predateur = liste_predateur->suivant;
	}
	  
	/* Affichage tableau */
	printf("+");
	for(int j = 0; j < SIZE_Y; j++)
		printf("-");
	printf("+\n");
	
	for(int i = 0; i < SIZE_X; i++){
		printf("|");
		for(int j = 0; j < SIZE_Y; j++)
			printf("%c", ecos[i][j]);
		printf("|\n");
	}
	
	printf("+");
	for(int j = 0; j < SIZE_Y; j++)
		printf("-");
	printf("+\n");
	
	/* Affichage données */
	printf("Nombre de proies (*): %d\t", nbProies);
	printf("Nombre de predateurs (O): %d\n", nbPredateurs);
	printf("(@) => predateurs + proies\n\n");
} 
/*--------------------------------------------------------------------------*/
void ajouter_animal(int x, int y, Animal **list_animal){
	assert((x >= 0) && (x < SIZE_X) && (y >= 0) && (y < SIZE_Y));
	*list_animal = ajouter_en_tete_animal(*list_animal, creer_animal(x, y, 10.));	
}
/*--------------------------------------------------------------------------*/
Animal* liberer_liste_animaux(Animal *liste){
	Animal *tmp;
	while(liste){
		tmp = liste;
		liste = liste->suivant;
		free(tmp);
	}
	
	return NULL;
}
/*--------------------------------------------------------------------------*/
void enlever_animal(Animal **liste, Animal *animal){
	assert(liste);  // on verifie que la liste n'est pas nulle 
	assert(animal); // on verifie que animal n'est pas null
	Animal* copie = *liste;
	if(*liste == animal){  // si animal est le 1er élément de la liste
		*liste = (*liste)->suivant;
		free(copie);
		return;
	}
	while(copie){  //si animal n'est pas le 1er élément de la liste 
		if(copie->suivant == animal){
			Animal *tmp = copie->suivant;
			copie->suivant = animal->suivant;
			free(tmp);
			return;
		}
		copie = copie->suivant;
	}
}
/*--------------------------------------------------------------------------*/
void bouger_animaux(Animal *la){
 	while(la){
 		if ((float)rand()/RAND_MAX < p_ch_dir){
    		la->x = ((la->x - la->dir[0])%SIZE_X + SIZE_X)%SIZE_X;  // pour le garder positif
    		la->y = ((la->y - la->dir[1])%SIZE_Y + SIZE_Y)%SIZE_Y;  // pour le garder positif
    	}
  			la = la->suivant;
 	}
}
/*--------------------------------------------------------------------------*/
void reproduce(Animal **liste_animale, float p_reproduce) {
	Animal *copie = *liste_animale;
	while(copie){
    	if ((float)rand()/RAND_MAX < p_reproduce) {
      		ajouter_animal(copie->x, copie->y, liste_animale);
      		copie->energie /= 2.0;
      		(*liste_animale)->energie = copie->energie;
    	}
    	copie = copie->suivant;
	}
}		 
/*--------------------------------------------------------------------------*/	
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
	for(int i = 0; i < SIZE_X; i++){
		for(int j = 0; j < SIZE_Y; j++)
			monde[i][j]++;
	}
}
/*--------------------------------------------------------------------------*/	
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]){
	Animal *copie = *liste_proie;
	bouger_animaux(*liste_proie);
	while(copie){   // on parcourt une fois la liste pour mettre a jour les energies
    	copie->energie--;
    	if (monde[copie->x][copie->y] >= 0){
      		copie->energie += gain_energie_proie;
      		monde[copie->x][copie->y] = temps_repousse_herbe;
      	}
      	copie = copie->suivant;	
      }
      Animal *tmp = *liste_proie;
      while(tmp){ // on parcourt une deuxieme fois pour eliminer les proies sans energie 
     	 if (tmp->energie < 0.0)
      		enlever_animal(liste_proie, tmp);
      	tmp = tmp->suivant;
      }
  		
  	reproduce(liste_proie, p_reproduce_proie);
}
/*--------------------------------------------------------------------------*/		
Animal *animal_en_XY(Animal *l, int x, int y){
	while(l){
		if((l->x = x) && (l->y = y)) 
			return l;
		l = l->suivant;
	}
	
	return NULL;
}		
/*--------------------------------------------------------------------------*/		
void rafraichir_predateurs(Animal **list_predateur, Animal **list_proie){
	Animal *copie = *list_predateur;  //on crée une copie pour parcourir la liste
	bouger_animaux(*list_predateur);  // on bouge toutes les predateurs
	while(copie){
		copie->energie -= 1.0;
		Animal *proieExiste = animal_en_XY(*list_proie, copie->x, copie->y);
		float nrAleatoire;
		nrAleatoire = (float)rand()/RAND_MAX;
		assert((nrAleatoire >= 0) && (nrAleatoire <= 1));
		if(proieExiste && (nrAleatoire < p_manger)){
			copie->energie += gain_energie_predateur;
			enlever_animal(list_proie, proieExiste);
		}
		copie = copie->suivant;
	}
	Animal *tmp = *list_predateur;
	while(tmp){ // on parcourt une deuxieme fois pour eliminer les proies sans energie 
		if(tmp->energie < 0.0)
			enlever_animal(list_predateur, tmp);
		tmp = tmp->suivant;
	}
	
	reproduce(list_predateur, p_reproduce_predateur);
}
/*--------------------------------------------------------------------------*/	
	
