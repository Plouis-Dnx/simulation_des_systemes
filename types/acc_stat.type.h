#ifndef ACC_STAT_H
#define ACC_STAT_H

typedef struct {
    int nb_pieces_produites; // Le nombre de pièces produites
    double temps_attente_total; // Temps total d’attente passé dans la file
    int nb_pieces_arrivees; // Nombre de pièces qui sont arrivées dans le système
    double temps_maximum_file; // Le temps maximum passé dans la file
    double temps_total; // Le temps total passé dans le système
    double temps_maximum_systeme; //  Le temps maximum passé dans le système
    double superficie_sous_qt;  // La superficie sous la courbe de longueur de la file Q(t)
    int maximum_qt; // Le maximum de Q(t) 
    double superficie_sous_bt; // La superficie sous la courbe d’occupation du serveur B(t)
} AccumulateurStat;

#endif // ACC_STAT_H