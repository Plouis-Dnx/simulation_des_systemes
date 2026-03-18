#include "aleatoire.h"
#include <stdio.h>
#include <stdlib.h>

double generation_aleatoire(double borne_inferieure, double borne_superieure) {
    // Génère un nombre aléatoire entre 2 bornes données
    double random = rand() / (double)RAND_MAX;
    return random * (borne_superieure - borne_inferieure) + borne_inferieure;
}

void aleatoire_calibrage(Simulation simulation, int tirages) {
    // Calibre le générateur aléatoire en estimant la moyenne des tirages
    double somme = 0;
    for (int i = 0; i < tirages; i++) 
        somme += generation_aleatoire(simulation.config.temps_inter_arrivee_a, simulation.config.temps_inter_arrivee_b);
    
    printf("Valeur moyenne des temps générés aléatoirement : %lf\n", somme/tirages);
}