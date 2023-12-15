/* déclaration des directives */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/* fichier d'entêtes qui contient les prototypes, les constantes et les types */
#include "definition.h"

/* fonction principale */
int main()
{   
    /* déclaration des variables */

    T_Case pion = {0, 0}; /* on place le pion sur la première case de la grille */  

    int nlig, ncol, grille[VMAX][VMAX];   /* lignes / colonnes / grille du jeu */
    short niveau, next;                   /* le niveau de l'ordinateur / 1 si l'ordinateur commence, 2 si c'est le joueur */

    /* on initialise les paramètres et on remplit la grille du jeu */
    Parametres(&nlig, &ncol, &niveau, &next);
    Remplir_Grille(grille, nlig, ncol);
    Affiche_Grille(nlig, ncol, pion);

    /* tant que le pion ne se trouve pas sur la case du puit, le jeu continue */
    while (((pion.num_l != nlig-1) || (pion.num_col != ncol-1)))
    {   
        /* L'ordinateur et le joueur jouent chacun leur tour */
        if (next == 1)
        {
            Coup_Ordi(&pion, niveau, grille, nlig, ncol);
            Affiche_Grille(nlig, ncol, pion);
            next++;
        }
        else
        {
            Coup_Joueur(&pion, nlig, ncol);
            Affiche_Grille(nlig, ncol, pion);
            next--;
        }
    }

    /* message de fin de partie */
    printf("\n\e[0;31mC'est terminé\e[0m\n\n");
    if (next == 1)
        printf("\e[0;33mBravo, tu as gagné !\e[0m \U0001F947\n\n");
    else
        printf("\e[0;34mDommage, L'ordinateur a gagné\e[0m \U0001F613\n\n");

    return 0;
}

/* déclaration des fonctions */

void Parametres(int* nlig, int* ncol, short* niveau, short* next)
{   
    /* on affecte à chaque paramètre un entier entre une borne inférieur et supérieur */
    system("clear");
    printf("\e[4;35mParamètres du jeu :\e[0m\n");
    printf("\e[0;37m\nnombre de lignes : \e[0m");
    *nlig = Lire_Entier(VMIN, VMAX);

    printf("\e[0;37m\nnombre de colonnes : \e[0m");
    *ncol = Lire_Entier(VMIN, VMAX);

    printf("\e[0;31m\nniveau de 1 à 4 : \e[0m");
    *niveau = Lire_Entier(1, 4);

    printf("\n\e[0;36mqui commence ?\nl'ordinateur (1) ou le joueur (2) : \e[0m");
    *next = Lire_Entier(1, 2);
}


int Lire_Entier(int bi, int bs)
{   
    int n;

    /* on boucle à l'infini tant que le nombre ne se trouve pas entre les deux bornes, sinon on renvoie la valeur */
    while (1)
    {   
        printf("\e[0;35m\n---> \e[0m");
        int x = scanf("%d", &n);
        printf("%d\n", n);
        if (n < bi || bs < n || x != 1) printf("\e[0;31\nmerreur !\n\e[0m");
        else return n;
    }
}


void Affiche_Grille(int nlig, int ncol, T_Case pion)
{   
    /* affichage global de la grille du jeu */  
    system("clear");
    printf("\n    \e[0;35m %s \e[0m\n\n     ", "Jeux du Nim");
    for (int j = 0; j < ncol; j++) printf("%-2d  ", j+1);

    printf("\n   ");
    for (int j = 0; j < ncol; j++) printf("----");
    printf("-\n");

    for (int i = 0; i < nlig; i++)
    {
        printf("%-2d |", i+1);
        for (int j = 0; j < ncol; j++)
        {   
            /* affichage du pion, des cases et du puit */
            if (i == pion.num_l && j == pion.num_col) printf("\e[0;37m O \e[0m|");
            else if (i == nlig-1 && j == ncol-1) printf("\e[0;37m P \e[0m|");
            else printf("\e[8;30m 0 \e[0m|"); 
        }
        printf("\n   ");
        for (int j = 0; j < ncol; j++) printf("----");
        printf("-\n");
    }
}


void Remplir_Grille(int tab[][VMAX], int nlig, int ncol)
{  
    /* on fixe le nimber du puit à 0 */
    tab[nlig-1][ncol-1] = 0;    

    for (int i = nlig - 1; i >= 0; i--)
        for (int j = ncol - 1; j >= 0; j--)
        {   
            /* on parcourt toutes les cases de la grille et on affecte à chaque fois un nimber */
            T_Case c = {i, j};
            tab[i][j] = Nimber(c, tab, nlig, ncol);
        }
}   


int Nimber(T_Case c, int tab[][VMAX], int nlig, int ncol)
{
    T_Tab_Case vois;
    int nb_vois, nimber = 0;

    /* on cherche les voisines de la case et on les parcourt */
    Voisines(c, vois, &nb_vois, nlig, ncol);
    for (int i = 0; i <= nb_vois; i++)
        /* si l'une des voisines a un nimber égal à 0, alors le nimber de case est 1, sinon il est de 0 */
        if (tab[vois[i].num_l][vois[i].num_col] == 0) nimber = 1;
    return nimber;
}


void Voisines(T_Case c, T_Tab_Case vois, int* nb_vois, int nlig, int ncol)
{   
    /* on crée un tableau qui contient 4 combinaisons. Elles représentent chacune l'ensemble des coordonnées à ajouter pour obtenir les voisines de la case */
    int vx, vy, coord_vois[4][2] = {{0, 1}, {0, 2}, {1, 0}, {2, 0}};

    *nb_vois = -1;
    for (int i = 0; i <= 3; i++)
    {   
        /* vx répresente la coordonnée de la ligne + la combinaison et vy la coordonnée de la colonne + la combinaison */
        vx = c.num_l   + coord_vois[i][0]; 
        vy = c.num_col + coord_vois[i][1];
        /* si les deux coordonnées sont encore comprises dans les limites de la grille alors on ajoute la case de coordonnées (vx, vy) aux voisines */
        if ((0 <= vx && vx <= nlig-1) && (0 <= vy && vy <= ncol-1))
        {   
            T_Case v = {vx, vy};

            vois[++*nb_vois] = v;
        }
    }
}


int Hasard(int bi, int bs)
{
    srand((unsigned int) time(NULL));

    /* formule qui permet de calculer un nombre au hasard entre 2 bornes grâce à rand() et au modulo */
    return rand() % ((bs + 1) - bi) + bi;
}


void Coup_Ordi_Hasard(T_Case* pion, T_Tab_Case vois, int nb_vois)
{   
    /* on choisit un nombre au hasard qui représente une voisine */
    int nbr = Hasard(0, nb_vois);

    /* le pion prend alors les coordonnées de la voisine */
    pion->num_l = vois[nbr].num_l;
    pion->num_col = vois[nbr].num_col;
}


void Coup_Ordi_Gagnant(T_Case* pion, T_Tab_Case vois, int nb_vois, int tab[][VMAX])
{   
    /* on fait, de base, un coup au hasard */
    Coup_Ordi_Hasard(pion, vois, nb_vois);
    for (int i = 0; i <= nb_vois; i++)
    {   
        /* on parcourt ensuite les voisines et si l'une d'elle a un nimber égal à 0, alors le pion prend les coordonnées de cette voisine */
        if (tab[vois[i].num_l][vois[i].num_col] == 0)
        {
            pion->num_l = vois[i].num_l;
            pion->num_col = vois[i].num_col;
        }
    }
}


void Coup_Ordi(T_Case* pion, short niveau, int tab[][VMAX], int nlig, int ncol)
{   
    T_Tab_Case vois;
    int nb_vois, nbr = Hasard(1, 3);
   
    /* on choisit un nombre entre 1 et 3, puis on cherche les voisines du pion */
    Voisines(*pion, vois, &nb_vois, nlig, ncol);
    switch (niveau)
    {   
        /* on fait un coup en fonction du niveau de l'ordinateur */
        case 1:
            Coup_Ordi_Hasard(pion, vois, nb_vois); 
            break;

        case 2:
            /* le joueur a 2 chances sur 3 de jouer un coup au hasard (nombre = 1 ou 2) et 1 chance sur 3 pour un coup gagnant (nombre = 1) */
            if (nbr == 1 || nbr == 2) 
                Coup_Ordi_Hasard(pion, vois, nb_vois);
            else 
                Coup_Ordi_Gagnant(pion, vois, nb_vois, tab); 
            break;

        case 3:
            /* le joueur a 1 chance sur 3 de jouer un coup gagnant (nombre = 1) et 2 chances sur 3 pour un coup au hasard (nombre = 2 ou 3) */
            if (nbr == 1) 
                Coup_Ordi_Hasard(pion, vois, nb_vois);
            else 
                Coup_Ordi_Gagnant(pion, vois, nb_vois, tab); 
            break;

        case 4:
            Coup_Ordi_Gagnant(pion, vois, nb_vois, tab); 
            break;
    }

    /* message pour avertir le joueur et un petit temps d'attente */
    printf("\n\e[0;36mAu tour de l'ordinateur !\nIl déplace le pion en (%d, %d)\n\n\e[0m", pion->num_l+1, pion->num_col+1); 
    sleep(3);
}


void Coup_Joueur(T_Case* pion, int nlig, int ncol)
{
    T_Tab_Case vois;
    int nb_vois, nbr;

    /* on cherche les voisines du pion */
    Voisines(*pion, vois, &nb_vois, nlig, ncol);
    printf("\n\e[0;33mA toi de jouer ! \n\nChoisir la destination :\e[0m");
    /* on affiche au joueur ses possibilités de déplacement */
    for (int i = 0; i <= nb_vois; i++)
        printf("\e[0;36m %d: (%d, %d)  \e[0m", i+1, vois[i].num_l+1, vois[i].num_col+1);
    
    /* le joueur choisit ensuite le coup qu'il veut jouer */
    nbr = Lire_Entier(1, nb_vois+1);
    /* le pion prend les coordonnées de la voisine ("nbr" étant la position de la voisine, dans le tableau "vois", -1) */
    pion->num_l = vois[nbr-1].num_l;
    pion->num_col = vois[nbr-1].num_col;
}

