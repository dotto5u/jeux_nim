#ifndef DEFINITON_H
#define DEFINITON_H

/* déclaration des constantes pour la valeur max et min de la grille */
#define VMIN 5 
#define VMAX 15

/* création de la structure "T_Case" */
typedef struct 
{
    int num_l;
    int num_col;
}   T_Case;

/* création du type "T_Tab_Case" */
typedef T_Case T_Tab_Case[4];

/* déclaration des prototypes */
void Voisines(T_Case c, T_Tab_Case vois, int* nb_vois, int nlig, int ncol);
int Nimber(T_Case c, int tab[][VMAX], int nlig, int ncol);
int Hasard(int bi, int bs);
int Lire_Entier(int bi, int bs);
void Parametres(int* nlig, int* ncol, short* niveau, short* next);
void Remplir_Grille(int tab[][VMAX], int nlig, int ncol);
void Affiche_Grille(int nlig, int ncol, T_Case pion);
void Coup_Joueur(T_Case* pion, int nlig, int ncol);
void Coup_Ordi(T_Case* pion, short niveau, int tab[][VMAX], int nlig, int ncol);
void Coup_Ordi_Hasard(T_Case* pion, T_Tab_Case vois, int nb_vois);
void Coup_Ordi_Gagnant(T_Case* pion, T_Tab_Case vois, int nb_vois, int tab[][VMAX]);

#endif
