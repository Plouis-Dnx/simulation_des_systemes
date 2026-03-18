#ifndef SIMULATION_H
#define SIMULATION_H

typedef struct {
    double temps_inter_arrivee_a, temps_inter_arrivee_b;
    double temps_service_a, temps_service_b;
    double temps_max; // Temps maximum de la simulation
    int seed; // Graine du générateur aléatoire
} SimConfig;

typedef struct {
    double temps; // Temps courant dans la simulation
    double instant_arrivee; // Instant de la prochaine arrivée d'une pièce
    double delta; // l'intervalle de temps entre deux événements successifs, utilisé pour mettre à jour les accumulateurs
} SimState;

typedef struct {
    SimConfig config;
    SimState state;
} Simulation;

#endif // SIMULATION_H