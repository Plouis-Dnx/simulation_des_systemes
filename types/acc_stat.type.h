#ifndef ACC_STAT_H
#define ACC_STAT_H

typedef struct {
    int nb_pieces_produites; // Le nombre de pièces produites
    int nb_pieces_arrivees; // Nombre de pièces qui sont arrivées dans le système

    double temps_total; // Le temps total passé dans le système 

    double superficie_sous_qt; // Aire sous Q(t) = temps d'attente cumulé dans la file
    double superficie_sous_bt; // Aire sous B(t) = temps d'occupation cumulé du serveur
} AccumulateurStat;

#endif // ACC_STAT_H