/*∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗
   @Autor: Juan Gómez Gómez - juan.gomez.2015
           Jordi Malé Carbonell - jordi.male.2015
∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗*/

#include "io.h"

/* FUNCIONS LECTURA */
int readFile(char* path, Data* data) {
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
            data->ip = readUntil(fd, '\n');
            aux = readUntil(fd, '\n');
            data->portPicard = atoi(aux);
            aux = readUntil(fd, '\n');
            data->portEnterprise = atoi(aux);

            free(aux);
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
    buffer[i] = '\0';
    return buffer;
}

/* FUNCIONS ESCRIPTURA */

void printConnection(char* name) {
    int mida = strlen("Connectant \n") + strlen(name);
    char buffer[mida];
    sprintf(buffer, "Connectant %s.\n", name);
    write(1, buffer, strlen(buffer));
}

void printDisconnection(char* name) {
    int mida = strlen("Desconnectant \n") + strlen(name);
    char buffer[mida];
    sprintf(buffer, "Desconnectant %s.\n", name);
    write(1, buffer, strlen(buffer));
}
