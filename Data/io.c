/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Data
* Curs 2017-2018
*
* @File     io.c
* @Purpose  Modul que conté les funcions relacionades amb la gestió de fitxers i
*           interacció amb l'usuari.
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

// Llibreries pròpies
#include "io.h"

/******************************************************************************/
/**************************** FUNCIONS DE LECTURA *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     readFile
* @Purpose  Funció llegirà el fitxer de configuració i ens donarà les seves dades
* @Param    In: path    Path del fitxer a llegir
*           Out: d      Data on guardarem les dades del fitxer
* @return   Retorna un enter que indica els possibles errors a l'hora de llegir
*           el fitxer
*
*******************************************************************************/
int readFile(char* path, Data* d) {
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
            d->ip = readUntil(fd, '\n');
            aux = readUntil(fd, '\n');
            d->portPicard = atoi(aux);
            free(aux);
            aux = readUntil(fd, '\n');
            d->portEnterprise = atoi(aux);

            free(aux);
            close(fd);
            return 0;
        } else {
            close(fd);
            write(1, ERROR_FILE2, sizeof(ERROR_FILE2));
            return -1;
        }
    }

}

/*******************************************************************************
*
* @Name     readUntil
* @Purpose  Funció llegeix fins a un caràcter donat
* @Param    In: fd  File Descriptor del qual llegirem
*               cFi Caràcter fins el qual llegirem
*           Out: -
* @return   Retorna tots els caràcters llegits fins a arribar al cFi
*
*******************************************************************************/
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

/******************************************************************************/
/************************** FUNCIONS D'ESCRIPTURA *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     printConnection
* @Purpose  Funció que printa una connexió per pantalla
* @Param    In: name  Nom de l'usuari que s'ha connectat a Data
*           Out: -
* @return   -
*
*******************************************************************************/
void printConnection(char* name) {
    int mida = strlen("Connectant \n") + strlen(name);
    char buffer[mida];
    sprintf(buffer, "Connectant %s.\n", name);
    write(1, buffer, strlen(buffer));
}

/*******************************************************************************
*
* @Name     printDisconnection
* @Purpose  Funció que printa una desconnexió per pantalla
* @Param    In: name  Nom de l'usuari que s'ha desconnectat de Data
*           Out: -
* @return   -
*
*******************************************************************************/
void printDisconnection(char* name) {
    int mida = strlen("Desconnectant \n") + strlen(name);
    char buffer[mida];
    sprintf(buffer, "Desconnectant %s.\n", name);
    write(1, buffer, strlen(buffer));
}
