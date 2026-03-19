#ifndef SIM_FUNC_H
#define SIM_FUNC_H

#include "../../types/simulation.type.h"
#include "../../types/acc_stat.type.h"
#include "../../types/server.type.h"

void do_simulation(Simulation*, Server*, AccumulateurStat*); // Lancement de la simulation

#endif // SIM_FUNC_H