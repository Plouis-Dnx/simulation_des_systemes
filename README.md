# Simulation des Systèmes

## Introduction
Ce travail me sert dans un but uniquement personnel. En essayant d'écrire un code propre et compréhensible réitérant les différentes simulations
vues en cours, TD et TP, cela m'a permis d'approfondir mes connaissances et de mieux comprendre les concepts abordés.

## Documentation

### Types
Afin de correctement architecturer mon projet, j'ai créé plusieurs types qui permettront de stocker plusieurs variables.
 - **AccumulateurStat** : Stocke les accumulateurs statistiques (nombre de pièces produites, le nombre de pièces arrivées dans le système, le temps d'attente total passé dans la file pour une pièce, ...)

 - **Server** : Simule le serveur en calculant le nombre de pièces en attente dans la file (et donc à traiter), l'état (libre ou occupé) et l'instant du prochain départ de la pièce en cours de traitement (généré aléatoirement)

 - **Simulation** : Contient la configuration de la simulation (les temps d'inter-arrivée, les temps de service, la graine (pour la génération aléatoire) et le temps de simulation) ainsi que les variables qui seront modifiées tout au long de la simulation (le temps courant ou l'horloge, l'instant d'arrivée d'une pièce dans le système et l'intervalle de temps entre deux événements successifs)

 - **Resultat** : Afin d'avoir un code propre, les variables de ce type sont calculées dans une fonction. Cela évite également d'avoir une multitude de paramètres dans la fonction d'affichage des résultats.

Le fichier types.h permet de réunir tous ces types afin de pouvoir tous les appeler en une seule ligne : 
```C
// Exemple : functions/affichage/affichage.h
#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "../../types/types.h"

// ...déclaration des fonctions...

#endif
```


### Fonctions
 - **Affichage** : Contient toutes les fonctions d'affichage (affichage de la configuration de la simulation et des résultats notamment).
 - **Aléatoire** : Les fonctions de ce module permettent de générer des nombres aléatoires entre 2 bornes données ainsi que de faire le calibrage du système aléatoire.
 - **Defaults** : Contient toutes les fonctions permettant d'initialiser les types avec des valeurs par défaut.
 - **Simulation** : Le coeur du projet. C'est dans ce module qu'est contenue toute la logique de la simulation : lancement d'une simulation, calcul des résultats, calcul des accumulateurs statistiques, ...

### Compilation
#### 1. Cloner le dépôt Git
```bash 
git clone https://github.com/Plouis-Dnx/simulation_des_systemes
```

#### 2. Se déplacer vers le dossier clôné
```bash 
cd .../simulation_des_systemes
```

#### 3. Compiler et lancer le programme
```bash 
gcc main.c functions/*/*.c -o main && ./main
```

## Comment j'ai utilisé l'IA
Dans ce projet, je me suis permis d'utiliser Claude afin de pouvoir mieux comprendre certains points du cours, comme par exemple à quoi servent certains accumulateurs statistiques et certaines variables du code proposé par le professeur.  
Cela m'a également permis de construire une architecture propre et lisible de mon projet.

## Structure du Projet
.
├── functions/
│   ├── affichage/
│   ├── aleatoire/
│   ├── defaults/
│   └── simulation/
└── types/

## Comparaison avec le code du professeur

Lors du TP, j'ai trouvé le code donné par notre professeur difficile à lire en raison de noms de variables peu explicites, d'une architecture en un seul fichier
et de l'absence de commentaires.
Mon objectif en le refactorisant était de réduire le grand nombre de variables en les assemblant dans des types ainsi que d'améliorer l'architecture en créant plusieurs fichiers, chacun avec leur responsabilité respective (logique de la simulation dans simulation.c, gestion des affichages dans affichage.c, génération aléatoire dans aleatoire.c, ...).

### Ce que j'ai amélioré
#### 1. Noms des variables 
Au lieu de a_ia, b_s, r, q, p, ..., j'ai opté pour des noms plus explicites comme (respectivement) temps_inter_arrivee_a, temps_service_b, random, nb_pieces_arrivees, nb_pieces_produites, ...

##### Mon code
```C title="aleatoire.c"
double generation_aleatoire(double borne_inferieure, double borne_superieure) {
    // Génère un nombre aléatoire entre 2 bornes données
    double random = rand() / (double)RAND_MAX;
    return random * (borne_superieure - borne_inferieure) + borne_inferieure;
}
```

J'ai ensuite pu appeller cette fonction dans ```simulation.functions.c``` :  
```C title="simulation.functions.c"
void traiter_arrivee(...) {
    // ...

    double inter_arrivee_a = simulation->config.temps_inter_arrivee_a;
    double inter_arrivee_b = simulation->config.temps_inter_arrivee_b;
    simulation->state.instant_arrivee = simulation->state.temps + generation_aleatoire(inter_arrivee_a, inter_arrivee_b);

    // ...
}
```

##### Code du professeur
```C
//On prend l'intervalle [a_ia, b_ia]
double generer_inter_arrivee(){
    //double a = a_ia;
    //double b = b_ia;
    double r = rand()/(double) RAND_MAX;
    return r*(b_ia-a_ia) + a_ia;
}
```

#### 2. Création de types 
Afin de mieux m'y retrouver, j'ai préféré créer des types pour stocker le grand nombre de variables et permettre de rendre mon code lisible et structuré.

##### Mon code
```C title="simulation.type.h"

#ifndef SIMULATION_H
#define SIMULATION_H

typedef struct {
    double temps_inter_arrivee_a, temps_inter_arrivee_b;
    double temps_service_a, temps_service_b;
    double temps_max; // Temps maximum de la simulation
    int seed; // Graine du générateur aléatoire
} SimConfig;

typedef struct {
    double temps; // Temps courant dans la simulation
    double instant_arrivee; // Instant de la prochaine arrivée d'une pièce
    double delta; // l'intervalle de temps entre deux événements successifs, utilisé pour mettre à jour les accumulateurs
} SimState;

typedef struct {
    SimConfig config;
    SimState state;
} Simulation;

#endif // SIMULATION_H
```

```C title="acc_stat.type.h"
#ifndef ACC_STAT_H
#define ACC_STAT_H

typedef struct {
    int nb_pieces_produites; // Le nombre de pièces produites
    double temps_attente_total; // Temps total d’attente passé dans la file
    int nb_pieces_arrivees; // Nombre de pièces qui sont arrivées dans le système
    double temps_maximum_file; // Le temps maximum passé dans la file
    double temps_total; // Le temps total passé dans le système
    double temps_maximum_systeme; //  Le temps maximum passé dans le système
    double superficie_sous_qt;  // La superficie sous la courbe de longueur de la file Q(t)
    int maximum_qt; // Le maximum de Q(t) 
    double superficie_sous_bt; // La superficie sous la courbe d’occupation du serveur B(t)
} AccumulateurStat;

#endif // ACC_STAT_H
```

```C title="resultat.type.h"
#ifndef RESULTAT_H
#define RESULTAT_H

typedef struct {
    double temps_moyen_total;
    double temps_moyen_attente;
    double nb_moyen_pieces;
    double taux_occupation;
    double taux_arrivee;
    double miu_obs;
} Resultat;

#endif // RESULTAT_H
```

##### Code du professeur
```C
double t = 0.0; // temps d'horloge
double t_arr = 0.0; //instant de la prochaine arrivee
double t_max = T_MAX; // duree maximale de la simulation (en unites de temps, e.g. minutes)
int long_file = 0; //nombre de pieces en attente dans la file, avant d'etre traites
double t_dep = (double) RAND_MAX; //instant du prochain depart de piece depuis le serveur
int etat_serveur = 0; //etat du serveur : 1 occupe, 0 libre
double t_att_cum = 0; //somme de tous les temps d'attente des pieces
double t_cum = 0.0; //somme de tous les temps de sejour des pieces dans le systeme
double t_occ = 0.0; //somme des temps d'occupation du serveur
double T_moy; // variable pour calculer le temps moyen passe par une piece dans le systeme
double T_att_moy; // variable pour calculer le temps moyen d'attente d'une piece
double N_moy; // variable pour calculer le nombre moyen de pieces dans le systeme a un instant quelconque
double taux_arr;// le taux d'arrivee des cliens dans le systeme (le nombre de clients arrives par unite de temps)
double taux_occ; //variable pour calculer l'occupation du serveur
double miu_obs; //variable pour calculer le nombre de clients traites par unite de temps
double delta; //variable pour calculer le temps entre deux evenements successifs
int q = 0; //compteur du nombre de pieces arrivees dans le systeme
int p = 0; //compteur du nombre de pieces produites
```


#### 3. Responsabilité unique
Je me suis permis de séparer chaque partie du code en plusieurs fichiers en respectant le principe de responsabilité unique.

##### Mon code
.
├── main.c
├── functions/...
│   ├── affichage/...
│   ├── aleatoire/...
│   ├── defaults/...
│   └── simulation/...
└── types/...

##### Code du professeur
main.c

#### 4. Creation d'un type enum pour les serveurs
Toujours pour une question de clarté du code, j'ai créé un type enum qui remplace les 0/1 du serveur pour connaître son état par LIBRE/OCCUPE

##### Mon Code
```C title="simulation.functions.c"
void traiter_arrivee(...){
    // ...

    if(server->etat == LIBRE) {
        //rendre le serveur occupe
        server->etat = OCCUPE;

        // programmer le nouveau depart
        double service_a = simulation->config.temps_service_a;
        double service_b = simulation->config.temps_service_b;
        server->instant_prochain_depart = simulation->state.temps + generation_aleatoire(service_a, service_b); 
    }

    // ...
}
```

##### Code du professeur
```C
if(etat_serveur==0) {
    etat_serveur = 1; //rendre le serveur occupe
    t_dep = t + generer_tmps_service(); // programmer le nouveau depart
}
```

#### 5. Ajout de commentaires
J'ai également ajouté des commentaire pour bien comprendre ce que font les fonctions et les algorithmes

##### Mon code
```C title="aleatoire.c"
void aleatoire_calibrage(Simulation simulation, int tirages) {
    // Calibre le générateur aléatoire en estimant la moyenne des tirages
    double somme = 0;
    for (int i = 0; i < tirages; i++) 
        somme += generation_aleatoire(simulation.config.temps_inter_arrivee_a, simulation.config.temps_inter_arrivee_b);
    
    printf("Valeur moyenne des temps générés aléatoirement : %lf\n", somme/tirages);
}
```

##### Code du professeur
```C
for (i=0;i<10000;i++){
    sum_tia=sum_tia+generer_inter_arrivee();
    sum_ts=sum_ts+generer_tmps_service();
}

printf("\n\nla val moyenne des temps inter-arr \t= %lf\n", sum_tia/10000);
printf("la val moyenne des temps de service \t= %lf\n", sum_ts/10000);
```

### Code Originel
```C
//file GG1 - correction ICB - version du 25.02.25

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//distrib unif temps inter-arrivee (en minutes)
#define a_ia 1.0
#define b_ia 20.0

//distrib unif temps de service (en minutes)
#define a_s 5
#define b_s 17

//critere d'arret : t_max de simulation (en minutes)
#define T_MAX 4000

//On prend l'intervalle [a_ia, b_ia]
double generer_inter_arrivee(){
    //double a = a_ia;
    //double b = b_ia;
    double r = rand()/(double) RAND_MAX;
    return r*(b_ia-a_ia) + a_ia;
}

//On prend l'intervalle [a_s, b_s]
double generer_tmps_service(){
    //double a = as;
    //double b = bs;
    double r = rand()/(double) RAND_MAX;
    return r*(b_s-a_s) + a_s;
}

int main(void)
{
    double t = 0.0; // temps d'horloge
    double t_arr = 0.0; //instant de la prochaine arrivee
    double t_max = T_MAX; // duree maximale de la simulation (en unites de temps, e.g. minutes)
    int long_file = 0; //nombre de pieces en attente dans la file, avant d'etre traites
    double t_dep = (double) RAND_MAX; //instant du prochain depart de piece depuis le serveur
    int etat_serveur = 0; //etat du serveur : 1 occupe, 0 libre
    double t_att_cum = 0; //somme de tous les temps d'attente des pieces
    double t_cum = 0.0; //somme de tous les temps de sejour des pieces dans le systeme
    double t_occ = 0.0; //somme des temps d'occupation du serveur
    double T_moy; // variable pour calculer le temps moyen passe par une piece dans le systeme
    double T_att_moy; // variable pour calculer le temps moyen d'attente d'une piece
    double N_moy; // variable pour calculer le nombre moyen de pieces dans le systeme a un instant quelconque
    double taux_arr;// le taux d'arrivee des cliens dans le systeme (le nombre de clients arrives par unite de temps)
  	double taux_occ; //variable pour calculer l'occupation du serveur
  	double miu_obs; //variable pour calculer le nombre de clients traites par unite de temps
    double delta; //variable pour calculer le temps entre deux evenements successifs
    int q = 0; //compteur du nombre de pieces arrivees dans le systeme
    int p = 0; //compteur du nombre de pieces produites

    double sum_tia=0;
    double sum_ts=0;
    int i;


    //srand (12345); //initialisation du generateur
    srand (10); //initialisation du generateur
    //srand (42); //initialisation du generateur
    //srand (33); //initialisation du generateur

    for (i=0;i<10000;i++){
        sum_tia=sum_tia+generer_inter_arrivee();
        sum_ts=sum_ts+generer_tmps_service();
    }
    
    printf("\n\nla val moyenne des temps inter-arr \t= %lf\n", sum_tia/10000);
    printf("la val moyenne des temps de service \t= %lf\n", sum_ts/10000);


    printf("\n# RAND_MAX = %d,\t valeur sur 32 bits: %10.2lf ou, sur 16 bits: %10.2lf\n", RAND_MAX, pow(2.0,31.0)-1, pow (2.0,15.0)-1);

    // Pour une arrivee a un temps t > 0
    //t_arr = t + generer_inter_arrivee(); //generation du temps pour la premiere arrivee de piece dans le systeme


    // Pour une arrivee au temps t = 0
    t_arr = 0.0;
    printf("\n\n#\thorloge\tfile\tetat\tq\tp\n");
    printf("INIT: \t%.2lf\t%d\t%d\t%d\t%d\n******************************************\n", t, long_file, etat_serveur, q,p); //affichage de l'etat global du systeme
    printf("#\thorloge\tfile\tetat\tq\tp\n");

    while(t<=t_max && (t_dep<=t_max || t_arr<=t_max)) //fin de la simulation?
    {
        //traitement d'une arrivee
        if (t_arr<t_dep){
            q++;
            delta=t_arr-t;
            t=t_arr;
            t_arr=t + generer_inter_arrivee();
            t_cum=t_cum+delta*(long_file+etat_serveur);
            t_att_cum=t_att_cum + delta*long_file;
            t_occ=t_occ + delta*etat_serveur;

            if(etat_serveur==0) {
                etat_serveur = 1; //rendre le serveur occupe
                t_dep = t + generer_tmps_service(); // programmer le nouveau depart
            }
            else{ //serveur deja occupe
                long_file++;
            }

           // printf("ARR : \t%.2lf\t%d\t%d\t%d\n", t, long_file, etat_serveur, q);
        }

        //traitement d'un départ
        else{
            //if (t_dep<=t_arr){
                p++;
                delta=t_dep-t;
                t=t_dep;
                t_cum=t_cum+delta*(long_file+etat_serveur);
                t_att_cum=t_att_cum + delta*long_file;
                t_occ=t_occ + delta*etat_serveur;

                if(long_file>0) {
                    long_file--;
                    t_dep = t + generer_tmps_service(); // programmer le nouveau depart
                }
                else{ //il n'y a plus de pieces en attente
                    etat_serveur=0;
                    t_dep = RAND_MAX;
                }
            //}
           // printf("DEP : \t%.2lf\t%d\t%d\t\t\%d\n", t, long_file, etat_serveur, p);

        }




    } //fin de la boucle de simulation

    //evenement de Fin la simulation, à t_max//
    delta = t_max - t; //delta entre le dernier evenement simule et la fin de simulation
    t_cum=t_cum+delta*(long_file+etat_serveur);
    t_att_cum=t_att_cum + delta*long_file;
    t_occ=t_occ + delta*etat_serveur;


    //calcul des resultats
    T_moy = t_cum/q;
    T_att_moy = t_att_cum/q;
    N_moy = t_cum/t_max;
    
    //printf("t_occ : %3.1lf\n", t_occ);
    taux_occ = 100*t_occ/t_max;
    taux_arr = q / t_max;
    miu_obs = p / t_occ;

    printf("# ------------------------------------\n# Rappel des valeurs moyennes des generateurs :\n# ------------------------------------\n");
	    
    printf("la val moyenne des temps inter-arr \t= %lf\n", sum_tia/10000);
    printf("la val moyenne des temps de service \t= %lf\n", sum_ts/10000);


    //affichage des resultats
    printf("# ------------------------------------\n# Resultats de FIN de simulation\n# ------------------------------------\n");
    printf("# Periode de simulation : %.2lf minutes\n", t_max);
    printf("# Nombre de pièces arrivees : %d\n", q);
    printf("# Nombre de pièces produites : %d\n", p);
    printf("# Nombre de pièces en attente : %d\n", long_file);
    printf("# Etat du serveur à T_MAX : %d\n", etat_serveur);
    printf("# [temps du dernier evenement simule avant t_max : %.2lf]\n", t);
    printf("# [temps du prochain evenement - non simule : %.2lf]\n\n", (t_arr<t_dep)?t_arr:t_dep);
    printf("# Nombre moyen de pieces dans le systeme : \t%lf\n", N_moy);
    printf("# Temps moyen de sejour passe par une piece : \t%lf\n", T_moy);
    printf("# Temps moyen passe par une piece en attente : \t%lf\n", T_att_moy);
    printf("# Pourcentage d'occupation du serveur : \t%3.2lf %%\n", taux_occ);
    printf("# Lambda (taux d'arrivee de pieces) observe : \t%lf\n", taux_arr);
	printf("# MIU (taux de service du serveur) observe : \t%lf\n\n", miu_obs);



    return 0;
}
```