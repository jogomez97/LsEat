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
            picard->ip = readUntil(fd, '\n');
            aux = readUntil(fd, '\n');
            picard->port = atoi(aux);
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

    bufferKB = (char*) malloc(sizeof(char));
    while (c != '\n') {
        read(0, &c, sizeof(char));
        if (c != '\n') {
            bufferKB[count] = c;
            bufferKB = (char*) realloc(bufferKB, sizeof(char) * (count + 2));
        }
        count++;
    }
    bufferKB[count - 1] = '\0';
    if (count == 1) {
        free(bufferKB);
        printShell();
        bufferKB = readKB();
    }
    return bufferKB;
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
    char* buffer;
    asprintf(&buffer, "Benvingut %s.\n", picard.nom);
    write(1, buffer, strlen(buffer));
    free(buffer);
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
    char* buffer;
    asprintf(&buffer, "Té %d euros disponibles\n", picard.saldo);
    write(1, buffer, strlen(buffer));
    free(buffer);
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
    char* buffer;
    asprintf(&buffer, "%s> ", picard.nom);
    write(1, buffer, strlen(buffer));
    free(buffer);
}

/*******************************************************************************
*
* @Name     printBill
* @Purpose  Funció que mostrarà per pantalla el total a pagar
* @Param    In:  money  total a pagar
*           Out: -
* @return   -
*
*******************************************************************************/
void printBill(int money) {
    char* buffer;
    asprintf(&buffer, "Son %d euros. Carregat en el seu compte.\n", money);
    write(1, buffer, strlen(buffer));
    free(buffer);
}


/*******************************************************************************
*
* @Name     gestionaShell
* @Purpose  Funció gestionarà el Picard i actuarà en funció del que hagi introduit
*           l'usuari per teclat
* @Param    In:  -
*           Out: -
* @return   Enter que error o èxit
*
*******************************************************************************/
int gestionaShell() {
    char* split;
    char* num;
    int   quantitat;
    char* plat;
    int   error;

    printShell(picard.nom);
    comanda = readKB();
    stringToUpper(comanda);
    split = strtok(comanda, " ");
    if (strcmp(CONNECT, split) == 0 && strtok(NULL, " ") == NULL) {
        if (!connectat) {
            write(1, CONNECTING, strlen(CONNECTING));
            int a = connectaServidor(connectat, picard, DATA, NULL);
            if (a >= 1) {
                sockfd = a;
                connectat = 1;
                write(1, CONNECTED, strlen(CONNECTED));
                if (picard.nPlats > 0) {
                    enviaTotsElsPlats();
                }
            }
        } else {
            write(1, ERROR_ESTABLISHED, strlen(ERROR_ESTABLISHED));
        }
    } else if (strcmp(SHOW, split) == 0) {
        split = strtok(NULL, " ");
        if (split != NULL && strtok(NULL, " ") == NULL) {
            if (strcmp(split, MENU) == 0) {
                show();
            } else {
                write(1, ERROR_COMAND, strlen(ERROR_COMAND));
            }
        } else {
            write(1, ERROR_COMAND, strlen(ERROR_COMAND));
        }
    } else if (strcmp(ORDER, split) == 0) {
        num = strtok(NULL, " ");
        plat = strtok(NULL, "\n");
        if (num != NULL && plat != NULL) {
            quantitat = atoi(num);
            error = isAllSpaces(plat);
            if (quantitat > 0 && !error) {
                plat = deleteExtraSpaces(plat);
                order(plat, num, 0);
                free(plat);
                plat = NULL;
            } else if (quantitat == 0 && !error) {
                write(1, ERROR_DEMANA2, strlen(ERROR_DEMANA2));
            } else if (error) {
                write(1, ERROR_DEMANA3, strlen(ERROR_DEMANA3));
            } else {
                write(1, ERROR_DEMANA, strlen(ERROR_DEMANA));
            }
        } else {
            write(1, ERROR_COMAND, strlen(ERROR_COMAND));
        }
    } else if (strcmp(DELETE, split) == 0) {
        num = strtok(NULL, " ");
        plat = strtok(NULL, "\n");
        if (num != NULL && plat != NULL) {
            quantitat = atoi(num);
            error = isAllSpaces(plat);
            if (quantitat > 0 && !error) {
                plat = deleteExtraSpaces(plat);
                delete(plat, num);
                free(plat);
                plat = NULL;
            } else if (quantitat == 0 && !error) {
                write(1, ERROR_ELIMINA2, strlen(ERROR_ELIMINA2));
            } else if (error) {
                write(1, ERROR_ELIMINA3, strlen(ERROR_ELIMINA3));
            } else {
                write(1, ERROR_ELIMINA, strlen(ERROR_ELIMINA));
            }
        } else {
            write(1, ERROR_COMAND, strlen(ERROR_COMAND));
        }
    } else if (strcmp(PAY, split) == 0 && strtok(NULL, " ") == NULL) {
        pay();
    } else if (strcmp(DISCONNECT, split) == 0 && strtok(NULL, " ") == NULL) {
        disconnect();
        return 1;
    } else {
        write(1, ERROR_COMAND, strlen(ERROR_COMAND));
    }
    return 0;
}
