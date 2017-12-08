#include "data.h"

List flota;

int gestionaFlota(char* data) {

    char* nom = strtok(data, "&");
    char* aux = strtok(NULL, "&");
    char* ip = strtok(NULL, "");
    if ((nom != NULL) & (aux != NULL) & (ip != NULL)) {
        Enterprise e;
        e.nom = strdup(nom);
        e.ip = strdup(ip);
        e.port = atoi(aux);
        e.nConnections = 0;

        insertNode(&flota, e);
        //printList(&flota);
        return 0;
    }
    return -1;
}

char* getEnterprise() {
    Enterprise e = checkFirstElement(&flota);

    int length = strlen(e.nom) + strlen(e.ip)
            + sizeof(e.port) + 2 * sizeof(char);

    char* buffer = (char*)malloc(sizeof(char) * length);
    sprintf(buffer, "%s&%d&%s", e.nom, e.port, e.ip);
    return buffer;
}
