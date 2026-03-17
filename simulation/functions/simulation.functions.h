#ifndef SIM_FUNC_H
#define SIM_FUNC_H

#include "../simulation.type.h"
#include "../../accumulateurs/acc_stat.type.h"
#include "../../server/server.type.h"

Simulation sim_default(); // initialise une simulation avec des valeurs par défaut
void verification_generation_aleatoire(SimConfig, int); // Vérifie si la génération aléatoire fonctionne correctement
void afficher_config(SimConfig); // Affiche la configuration acutelle de la simulation
void do_simulation(Simulation*, Server*, AccumulateurStat*); // Lancement de la simulation

#endif // SIM_FUNC_H