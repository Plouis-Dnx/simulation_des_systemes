#include "defaults.h"
#include <stdlib.h>

Simulation sim_default() {
    Simulation simulation = {
        .config = {
            .temps_inter_arrivee_a = 1,
            .temps_inter_arrivee_b = 20,

            .temps_service_a = 5,
            .temps_service_b = 17,

            .temps_max = 4000,

            .seed = 10
        },

        .state = {
            .temps = 0,
            .instant_arrivee = 0,
            //.delta = 0
        }
    };

    return simulation;
}

Server server_default() {
    Server server = {
        .etat = LIBRE,
        .instant_prochain_depart = (double)RAND_MAX,
        .nb_pieces_traitees = 0
    };
    
    return server;
}

AccumulateurStat acc_stat_default() {
    AccumulateurStat acc_stat = {0};
    return acc_stat;
}