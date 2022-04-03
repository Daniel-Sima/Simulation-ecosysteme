#ifndef __ECOSYS__H__
#define __ECOSYS__H__

#define SIZE_X 25
#define SIZE_Y 25

/*--------------------------------------------------------------------------*/
extern const float p_ch_dir; 
extern const int gain_energie_proie;
extern const int gain_energie_predateur;
extern const float p_reproduce_proie;
extern const float p_reproduce_predateur;
extern const int temps_repousse_herbe;
extern const float p_manger;
/*--------------------------------------------------------------------------*/
typedef struct _animal{
	int x, y;
	float energie; // mort si < 0
	int dir[2];    // represente la direction
	struct _animal *suivant;
} Animal; 
/*--------------------------------------------------------------------------*/
Animal *creer_animal(int x, int y, float energie);
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal);
unsigned int compte_animal_it(Animal *la);
unsigned int compte_animal_rec(Animal *la);
void affichage_ecosysteme(Animal *liste_proies, Animal *liste_predateur);
void ajouter_animal(int x, int y, Animal **liste_animal);
Animal* liberer_liste_animaux(Animal *liste);
void enlever_animal(Animal **liste, Animal *animal);
void bouger_animaux(Animal *head);
void reproduce(Animal **list_animal, float p_reporduce);
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]);
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]);
Animal *animal_en_XY(Animal*l, int x, int y);
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie);
/*--------------------------------------------------------------------------*/
#endif
