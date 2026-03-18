#include "server.functions.h"
#include <stdlib.h>

Server server_default() {
    Server server = {
        .etat = LIBRE,
        .instant_prochain_depart = (double)RAND_MAX,
        .nb_pieces_en_attente = 0
    };
    
    return server;
}