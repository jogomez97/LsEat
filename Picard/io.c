/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
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
*           Out: picard Picard on guardarem les dades del fitxer
* @return   Retorna un enter que indica els possibles errors a l'hora de llegir
*           el fitxer
*
*******************************************************************************/
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

/*******************************************************************************
*
* @Name     readUntil
* @Purpose  Funció llegeix fins a un caràcter donat
* @Param    In: fd  File Descriptor del qual llegirem
                cFi Caràcter fins el qual llegirem
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
    buffer[i - 1] = '\0';
    return buffer;
}

/*******************************************************************************
*
* @Name     readKB
* @Purpose  Funció llegeix el teclat
* @Param    In:  -
*           Out: -
* @return   Retorna els caràcters que l'usuari ha entrat pel teclat.
*
*******************************************************************************/
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
        printShell();
        buffer = readKB();
    }
    return buffer;
}

/*******************************************************************************
*
* @Name     inputFlush
* @Purpose  Funció que neteja els caràcters no llegits del teclat
* @Param    In:  -
*           Out: -
* @return   Retorna el nombre de caràcters netejats.
*
*******************************************************************************/
int inputFlush() {
    char c = '0';
    int i = 0;

    while (c != '\n') {
        read(1, &c, sizeof(char));
        i++;
    }
    return i;
}


/******************************************************************************/
/************************** FUNCIONS D'ESCRIPTURA *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     printWelcome
* @Purpose  Funció que mostrarà per pantalla el missatge de benvinguda
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void printWelcome() {
    int mida = strlen("Benvingut.\n") + strlen(picard.nom);
    char buffer[mida];
    sprintf(buffer, "Benvingut %s.\n", picard.nom);
    write(1, buffer, strlen(buffer));
}

/*******************************************************************************
*
* @Name     printMoney
* @Purpose  Funció que mostrarà per pantalla els diners disponibles del Picard
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void printMoney() {
    char buffer[strlen("Teeuros disponibles\n") + 3];
    sprintf(buffer, "Té %d euros disponibles\n", picard.saldo);
    write(1, buffer, strlen(buffer));
}

/*******************************************************************************
*
* @Name     printShell
* @Purpose  Funció que mostrarà per pantalla la Shell de Picard
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void printShell() {
    int mida = strlen(picard.nom) + 2;
    char buffer[mida];
    sprintf(buffer, "%s> ", picard.nom);
    write(1, buffer, strlen(buffer));
}
