#include "./simulation/functions/simulation.functions.h"
#include "./server/functions/server.functions.h"
#include "./accumulateurs/acc_stat.type.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
    Simulation simulation = sim_default();
    Server server = server_default();
    AccumulateurStat acc_stat = {0};

    srand(simulation.config.seed);
    verification_generation_aleatoire(simulation.config, 20000); // 10000 temps inter_arrivees + 10000 temps services
    do_simulation(&simulation, &server, &acc_stat);

    return 0;
}