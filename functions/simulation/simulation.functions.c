#include <stdio.h>
#include <stdlib.h>

#include "simulation.functions.h"
#include "../aleatoire/aleatoire.h"
#include "../affichage/affichage.h"
#include "../defaults/defaults.h"

#include "../../types/resultat.type.h"

void mise_a_jour_acc_stat(Simulation* simulation, Server* server, AccumulateurStat* acc_stat){
    // Mise à jour des accumulateurs statistiques pour l'intervalle delta
    acc_stat->temps_total += simulation->state.delta * (simulation->state.nb_pieces_en_attente + server->etat);
    acc_stat->superficie_sous_qt += simulation->state.delta * simulation->state.nb_pieces_en_attente;
    acc_stat->superficie_sous_bt += simulation->state.delta * server->etat;
}

void traiter_arrivee(Simulation* simulation, Server* server, AccumulateurStat* acc_stat) {
    // Une pièce arrive dans le système -> On incrémente le nombre de pièces arrivées
    acc_stat->nb_pieces_arrivees++;

    // Calcul de delta = temps écoulé entre le dernier événement et l'arrivée de cette pièce
    simulation->state.delta = simulation->state.instant_arrivee - simulation->state.temps;

    // On avance l'horloge jusqu'à l'instant d'arrivée
    simulation->state.temps = simulation->state.instant_arrivee;

    // Génération aléatoire d'un temps d'arrivée
    double inter_arrivee_a = simulation->config.temps_inter_arrivee_a;
    double inter_arrivee_b = simulation->config.temps_inter_arrivee_b;
    simulation->state.instant_arrivee = simulation->state.temps + generation_aleatoire(inter_arrivee_a, inter_arrivee_b);

    mise_a_jour_acc_stat(simulation, server, acc_stat);

    if(server->etat == LIBRE) {
        //rendre le serveur occupe
        server->etat = OCCUPE;

        // programmer le nouveau depart
        double service_a = simulation->config.temps_service_a;
        double service_b = simulation->config.temps_service_b;
        server->instant_prochain_depart = simulation->state.temps + generation_aleatoire(service_a, service_b); 
    }
    else{ //serveur deja occupe
        simulation->state.nb_pieces_en_attente++;
    }
}

void traiter_depart(Simulation* simulation, Server* server, AccumulateurStat* acc_stat) {
    // Une pièce a été traitée -> On incrémente le nombre de pièces produites
    acc_stat->nb_pieces_produites++;

    // Calcul de delta = temps écoulé entre le dernier événement et le départ de cette pièce
    simulation->state.delta = server->instant_prochain_depart - simulation->state.temps;

    // On avance l'horloge jusqu'au prochain événement
    simulation->state.temps = server->instant_prochain_depart;

    mise_a_jour_acc_stat(simulation, server, acc_stat);

    if(simulation->state.nb_pieces_en_attente > 0) {
        // On traite une pièce -> On décrémente le nombre de pièces en attente
        simulation->state.nb_pieces_en_attente--;

        // Programmer le nouveau départ
        double service_a = simulation->config.temps_service_a;
        double service_b = simulation->config.temps_service_b;
        server->instant_prochain_depart = simulation->state.temps + generation_aleatoire(service_a, service_b); 
    }
    else{
        // S'il n'y a aucune pièce en attente, alors le serveur est libre et le prochain départ est fixé à +INFINI
        server->etat = LIBRE;
        server->instant_prochain_depart = (double)RAND_MAX;
    }
}

Resultat calcul_fin_simulation(Simulation* simulation, Server* server, AccumulateurStat* acc_stat) {
    // Calcul de delta = temps écoulé entre le temps de simulation max et le temps d'exécution de la simulation
    simulation->state.delta = simulation->config.temps_max - simulation->state.temps;

    mise_a_jour_acc_stat(simulation, server, acc_stat);

    // Calcul des résultats
    Resultat resultat = {
        .temps_moyen_total = acc_stat->temps_total / acc_stat->nb_pieces_arrivees,
        .temps_moyen_attente = acc_stat->superficie_sous_qt / acc_stat->nb_pieces_arrivees,
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