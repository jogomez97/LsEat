/*∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗
   @Autor: Juan Gómez Gómez - juan.gomez.2015
           Jordi Malé Carbonell - jordi.male.2015
∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗*/

#include "io.h"

/* FUNCIONS LECTURA */
int readConfig(char* path, Enterprise* enterprise) {
    int fd;

    fd = open(path, O_RDONLY);

    if (fd < 0) {
        write(1, ERROR_FILE1, sizeof(ERROR_FILE1));
        return -1;
    } else {
        //Comprovem que el fitxer no estigui buit
        char c;
        int n = read(fd, &c, sizeof(char));
        if (n > 0) {
            //Si no està buit llegim el fitxer
            char* aux;

            lseek(fd, -1, SEEK_CUR);
            enterprise->nom = readUntil(fd, '\n');
            aux = readUntil(fd, '\n');
            enterprise->seg = atoi(aux);
            free(aux);
            aux = NULL;
            enterprise->ipData = readUntil(fd, '\n');
            aux = readUntil(fd, '\n');
            enterprise->portData = atoi(aux);
            free(aux);
            aux = NULL;
            enterprise->ipPicard = readUntil(fd, '\n');
            aux = readUntil(fd, '\n');
            enterprise->portPicard = atoi(aux);
            free(aux);
            aux = NULL;

            close(fd);
            return 0;
        } else {
            write(1, ERROR_FILE2, sizeof(ERROR_FILE2));
            return -1;
        }
    }

}

int readMenu(char* path, Menu* menu) {
    int fd;

    fd = open(path, O_RDONLY);

    if (fd < 0) {
        write(1, ERROR_FILE1, sizeof(ERROR_FILE1));
        return -1;
    } else {
        //Comprovem que el fitxer no estigui buit
        char c;
        int n = read(fd, &c, sizeof(char));
        if (n > 0) {
            //Si no està buit llegim el fitxer
            int i = 0;
            char c;
            char* aux;

            lseek(fd, -1, SEEK_CUR);
            menu->plats = (Plat*) malloc(sizeof(Plat));

            while (n > 0) {

                if (i > 0) {
                    menu->plats = (Plat*) realloc(menu->plats, sizeof(Plat) * (i + 1));
                }

                menu->plats[i].nom = readUntil(fd, '\n');
                aux = readUntil(fd, '\n');
                menu->plats[i].quants = atoi(aux);
                free(aux);
                aux  = readUntil(fd, '\n');
                menu->plats[i].preu = atoi(aux);
                free(aux);

                n = read(fd, &c, sizeof(char));
                if (n > 0) {
                    lseek(fd, -1, SEEK_CUR);
                }
                i++;

            }

            menu->nPlats = i;
            close(fd);
            return 0;
        } else {
            write(1, ERROR_FILE2, sizeof(ERROR_FILE2));
            return -1;
        }
    }

}

char* readUntil(int fd, char cFi) {
    int i = 0;
    char c = '0';
    char* buffer = (char*)malloc(sizeof(char));

    while (c != cFi) {

        read(fd, &c, sizeof(char));

        if (c != cFi) {
            buffer[i] = c;
            buffer = (char*)realloc(buffer, sizeof(char) * (i + 2));
        }
        i++;
    }
    buffer[i - 1] = '\0';
    return buffer;
}

char* readKB() {
    char c = '0';
    int  count = 0;

    char* buffer = (char*) malloc(sizeof(char));
    while (c != '\n') {
        read(1, &c, sizeof(char));
        if (c != '\n') {
            buffer[count] = c;
            buffer = (char*) realloc(buffer, sizeof(char) * (count + 1));
        }
        count++;

    }
    buffer[count - 1] = '\0';
    if (count == 1) {
        free(buffer);
        buffer = readKB();
    }
    return buffer;
}

int inputFlush() {
    char c = '0';
    int i = 0;

    while (c != '\n') {
        read(1, &c, sizeof(char));
        i++;
    }
    return i;
}

/* FUNCIONS ESCRIPTURA */

void printWelcome() {
    int mida = strlen("Benvingut.\n") + strlen(enterprise.nom);
    char buffer[mida];
    sprintf(buffer, "Benvingut %s.\n", enterprise.nom);
    write(1, buffer, strlen(buffer));
}
