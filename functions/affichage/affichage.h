#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "../../types/types.h"

void afficher_config(Simulation simulation);
void afficher_resultats(const Simulation simulation, const Server server, const AccumulateurStat acc_stat, const Resultat resultat);

#endif