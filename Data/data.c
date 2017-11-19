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
    flota.enterprises[flota.quants].ip = strtok(NULL, "");
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
