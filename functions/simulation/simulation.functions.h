#ifndef SIM_FUNC_H
#define SIM_FUNC_H

#include "../../types/simulation.type.h"
#include "../../types/acc_stat.type.h"
#include "../../types/server.type.h"

Simulation sim_default(); // initialise une simulation avec des valeurs par défaut
void afficher_config(Simulation); // Affiche la configuration acutelle de la simulation
void do_simulation(Simulation*, Server*, AccumulateurStat*); // Lancement de la simulation

#endif // SIM_FUNC_H