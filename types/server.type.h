#ifndef SERVER_TYPE_H
#define SERVER_TYPE_H

typedef enum {
    LIBRE = 0,
    OCCUPE = 1
} ServerState;

typedef struct {
    ServerState etat; // 0 si libre, 1 si occupé
    int nb_pieces_en_attente; // nombre de pièces en attente dans la file, avant d'être traitées
    double instant_prochain_depart; // instant du prochain départ de la pièce depuis le serveur
} Server;

#endif // SERVER_TYPE_H