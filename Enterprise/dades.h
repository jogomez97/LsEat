#ifndef   _DADES_H_
#define   _DADES_H_

typedef struct {
    char*   nom;
    int     seg;
    char*   ipData;
    int     portData;
    char*   ipPicard;
    int     portPicard;
    int     nConnections;
} Enterprise;

typedef struct {
    char*   nom;
    int     quants;
    int     preu;
} Plat;

typedef struct {
    int     nPlats;
    Plat*   plats;
} Menu;


#endif
