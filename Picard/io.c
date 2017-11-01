/*∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗
   @Autor: Juan Gómez Gómez - juan.gomez.2015
           Jordi Malé Carbonell - jordi.male.2015
∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗*/

#include "io.h"

/* FUNCIONS LECTURA */
int readFile(char* path, Picard* picard) {
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
            picard->nom = readUntil(fd, '\n');
            aux = readUntil(fd, '\n');
            picard->saldo = atoi(aux);
            free(aux);
            aux = NULL;
            picard->ip = readUntil(fd, '\n');
            aux = readUntil(fd, '\n');
            picard->port = atoi(aux);
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

void printWelcome(char* name) {
    int mida = strlen("Benvingut.\n") + strlen(name);
    char buffer[mida];
    sprintf(buffer, "Benvingut %s.\n", name);
    write(1, buffer, strlen(buffer));
}

void printMoney(int money) {
    char buffer[strlen("Teeuros disponibles\n") + 3];
    sprintf(buffer, "Té %d euros disponibles\n", money);
    write(1, buffer, strlen(buffer));
}

void printShell(char* name) {
    int mida = strlen(name) + 2;
    char buffer[mida];
    sprintf(buffer, "%s> ", name);
    write(1, buffer, strlen(buffer));
}
