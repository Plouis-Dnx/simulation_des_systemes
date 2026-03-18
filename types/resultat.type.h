#ifndef RESULTAT_H
#define RESULTAT_H

typedef struct {
    double temps_moyen_total;
    double temps_moyen_attente;
    double nb_moyen_pieces;
    double taux_occupation;
    double taux_arrivee;
    double miu_obs;
} Resultat;

#endif // RESULTAT_H