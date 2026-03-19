#include "affichage.h"
#include <stdio.h>

void afficher_config(Simulation simulation) {
    SimConfig config = simulation.config;

    printf("Temps d'inter-arrivée : [%.2lf, %.2lf]\n", config.temps_inter_arrivee_a, config.temps_inter_arrivee_b);
    printf("Temps de service : [%.2lf, %.2lf]\n", config.temps_service_a, config.temps_service_b);
    printf("Temps maximum de la simulation : %lf\n", config.temps_max);
}

void afficher_resultats(const Simulation simulation, const Server server, const AccumulateurStat acc_stat, const Resultat resultat) {
    printf("# ------------------------------------\n# Resultats de FIN de simulation\n# ------------------------------------\n");

    printf("# Periode de simulation : %.2lf minutes\n", simulation.config.temps_max);
    printf("# [temps du dernier evenement simule avant t_max : %.2lf]\n", simulation.state.temps);

    printf("# Nombre de pièces arrivees : %d\n", acc_stat.nb_pieces_arrivees);
    printf("# Nombre de pièces produites : %d\n", acc_stat.nb_pieces_produites);

    printf("# Nombre de pièces en attente : %d\n", server.nb_pieces_en_attente);
    printf("# Etat du serveur à T_MAX : %d\n", server.etat);
    
    printf("# [temps du prochain evenement - non simule : %.2lf]\n\n", 
        (simulation.state.instant_arrivee < server.instant_prochain_depart)?simulation.state.instant_arrivee:server.instant_prochain_depart
    );

    // Resultat
    printf("# Nombre moyen de pieces dans le systeme : \t%lf\n", resultat.nb_moyen_pieces);
    printf("# Temps moyen de sejour passe par une piece : \t%lf\n", resultat.temps_moyen_total);
    printf("# Temps moyen passe par une piece en attente : \t%lf\n", resultat.temps_moyen_attente);
    printf("# Pourcentage d'occupation du serveur : \t%3.2lf %%\n", resultat.taux_occupation);
    printf("# Lambda (taux d'arrivee de pieces) observe : \t%lf\n", resultat.taux_arrivee);
	printf("# MIU (taux de service du serveur) observe : \t%lf\n\n", resultat.miu_obs);
}