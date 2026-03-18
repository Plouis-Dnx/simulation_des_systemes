#include "./functions/simulation/simulation.functions.h"
#include "./functions/server/server.functions.h"

int main(){
    Simulation simulation = sim_default();
    Server server = server_default();
    AccumulateurStat acc_stat = {0};

    do_simulation(&simulation, &server, &acc_stat);

    return 0;
}