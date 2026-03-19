#ifndef SERVER_TYPE_H
#define SERVER_TYPE_H

typedef enum {
    LIBRE = 0,
    OCCUPE = 1
} ServerState;

typedef struct {
    ServerState etat; // 0 si libre, 1 si occupé
    double instant_prochain_depart; // instant du prochain départ de la pièce depuis le serveur
    int nb_pieces_traitees; // Permet d'avoir un indicateur sur le nombre de pièces traitées par ce serveur
} Server;

#endif // SERVER_TYPE_H