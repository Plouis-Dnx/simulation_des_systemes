#include "./functions/simulation/simulation.functions.h"
#include "./functions/defaults/defaults.h"

int main(){
    Simulation simulation = sim_default();
    Server server = server_default();
    AccumulateurStat acc_stat = acc_stat_default();

    do_simulation(&simulation, &server, &acc_stat);

    return 0;
}