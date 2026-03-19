#include <stdio.h>
#include <stdlib.h>

#include "simulation.functions.h"
#include "../aleatoire/aleatoire.h"
#include "../affichage/affichage.h"
#include "../defaults/defaults.h"

#include "../../types/resultat.type.h"

void traiter_arrivee(Simulation* simulation, Server* server, AccumulateurStat* acc_stat) {
    acc_stat->nb_pieces_arrivees++;
    simulation->state.delta = simulation->state.instant_arrivee - simulation->state.temps;
    simulation->state.temps = simulation->state.instant_arrivee;

    double inter_arrivee_a = simulation->config.temps_inter_arrivee_a;
    double inter_arrivee_b = simulation->config.temps_inter_arrivee_b;
    simulation->state.instant_arrivee = simulation->state.temps + generation_aleatoire(inter_arrivee_a, inter_arrivee_b);

    acc_stat->temps_total += simulation->state.delta * (server->nb_pieces_en_attente + server->etat);
    acc_stat->temps_attente_total += simulation->state.delta * server->nb_pieces_en_attente;
    acc_stat->superficie_sous_bt += simulation->state.delta * server->etat;

    if(server->etat == LIBRE) {
        //rendre le serveur occupe
        server->etat = OCCUPE;
        // programmer le nouveau depart
        double service_a = simulation->config.temps_service_a;
        double service_b = simulation->config.temps_service_b;
        server->instant_prochain_depart = simulation->state.temps + generation_aleatoire(service_a, service_b); 
    }
    else{ //serveur deja occupe
        server->nb_pieces_en_attente++;
    }
}

void traiter_depart(Simulation* simulation, Server* server, AccumulateurStat* acc_stat) {
    acc_stat->nb_pieces_produites++;
    simulation->state.delta = server->instant_prochain_depart - simulation->state.temps;
    simulation->state.temps = server->instant_prochain_depart;
    acc_stat->temps_total += simulation->state.delta * (server->nb_pieces_en_attente + server->etat);
    acc_stat->temps_attente_total += simulation->state.delta * server->nb_pieces_en_attente;
    acc_stat->superficie_sous_bt += simulation->state.delta * server->etat;

    if(server->nb_pieces_en_attente > 0) {
        server->nb_pieces_en_attente--;
        // Programmer le nouveau départ
        double service_a = simulation->config.temps_service_a;
        double service_b = simulation->config.temps_service_b;
        server->instant_prochain_depart = simulation->state.temps + generation_aleatoire(service_a, service_b); 
    }
    else{ //il n'y a plus de pieces en attente
        server->etat = LIBRE;
        server->instant_prochain_depart = (double)RAND_MAX;
    }
}

Resultat calcul_fin_simulation(Simulation* simulation, Server* server, AccumulateurStat* acc_stat) {
    simulation->state.delta = simulation->config.temps_max - simulation->state.temps;
    
    acc_stat->temps_total += simulation->state.delta * (server->nb_pieces_en_attente + server->etat);
    acc_stat->temps_attente_total += simulation->state.delta * server->nb_pieces_en_attente;
    acc_stat->superficie_sous_bt += simulation->state.delta * server->etat;
    

    // Calcul des résultats
    Resultat resultat = {
        .temps_moyen_total = acc_stat->temps_total / acc_stat->nb_pieces_arrivees,
        .temps_moyen_attente = acc_stat->temps_attente_total / acc_stat->nb_pieces_arrivees,
        .nb_moyen_pieces = acc_stat->temps_total / simulation->config.temps_max,
        .taux_occupation = acc_stat->superficie_sous_bt * 100 / simulation->config.temps_max,
        .taux_arrivee = (double)acc_stat->nb_pieces_arrivees / simulation->config.temps_max,
        .miu_obs = (double)acc_stat->nb_pieces_produites / acc_stat->superficie_sous_bt
    };
    
    return resultat;
}

void do_simulation(Simulation* simulation, Server* server, AccumulateurStat* acc_stat) {
    // Initialisation de la génération aléatoire (seed + calibrage)
    srand(simulation->config.seed);
    aleatoire_calibrage(*simulation, 20000); // 10000 temps inter_arrivees + 10000 temps services

    // Affichage de la configuration initiale de la simulation
    afficher_config(*simulation);

    while( 
        simulation->state.temps <= simulation->config.temps_max
        && ( server->instant_prochain_depart <= simulation->config.temps_max || simulation->state.instant_arrivee <= simulation->config.temps_max)
    ) {
        if (simulation->state.instant_arrivee < server->instant_prochain_depart) traiter_arrivee(simulation, server, acc_stat);
        else traiter_depart(simulation, server, acc_stat);
    }

    // Calcul et affichage des résultats
    Resultat resultat = calcul_fin_simulation(simulation, server, acc_stat);
    afficher_resultats(*simulation, *server, *acc_stat, resultat);
}