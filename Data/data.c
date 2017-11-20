#include "data.h"

int gestionaFlota(char* data) {
    if (flota.quants == 0) {
        flota.enterprises = (Enterprise*) malloc(sizeof(Enterprise));
    } else {
        flota.enterprises = (Enterprise*) realloc(flota.enterprises,
            sizeof(Enterprise) * (flota.quants + 1));
    }

    flota.enterprises[flota.quants].nom = strtok(data, "&");
    char* aux = strtok(NULL, "&");
    flota.enterprises[flota.quants].ip = strtok(NULL, "\n");
    if ((flota.enterprises[flota.quants].nom != NULL) & (aux != NULL)
        & (flota.enterprises[flota.quants].ip != NULL)) {

        flota.enterprises[flota.quants].port = atoi(aux);
        flota.quants++;
        //printaFlota();
        return 0;
    }
    return -1;
}

void printaFlota() {
    int i;
    for (i = 0; i < flota.quants; i++) {
        write(1, flota.enterprises[i].nom, strlen(flota.enterprises[i].nom));
        write(1, "\n", 1);
    }
}

char* getEnterprise() {
    Enterprise e = flota.enterprises[flota.quants - 1];

    int length = strlen(e.nom) + strlen(e.ip)
            + sizeof(e.port) + 2 * sizeof(char);
    // S'haura de alliberar somewhere
    char* buffer = (char*)malloc(sizeof(char) * length);
    sprintf(buffer, "%s&%d&%s", e.nom, e.port, e.ip);
    return buffer;
}
