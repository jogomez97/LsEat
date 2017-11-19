#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>


#include "dades.h"

#define ERROR_ARGC          "Error! No s'han passat els arguments necessaris!\n"
#define ERROR_OPEN          "Error! No s'ha pogut obrir el fitxer.\n"
#define ERROR_MESSAGE       "Opció incorrecta.\n"
#define ERROR_SOCK          "Error en crear el socket!\n"
#define ERROR_ACCEPT        "Error en acceptar!\n"
#define ERROR_CONNECT       "Error de connexió amb Data.\n"
#define ERROR_TRAMA         "Error en la trama!\n"
#define ERROR_DISCONNECTED  "Error! Data s'ha desconnectat\n"
#define CONNECTED_D         "[ENTERPRISE] Connexió establerta amb Data\n"
#define DISCONNECTED_D      "[ENTERPRISE] Desconnexió de Data\n"

#define ENT_INF             "[ENT_INF]"
#define CONOK               "CONOK"
#define CONKO               "CONKO"
#define CONOKb              "[CONOK]"       //b de brackets
#define CONKOb              "[CONKO]"
#define UPDATE              "[UPDATE]"
#define UPDATEOK            "[UPDATEOK]"
#define UPDATEKO            "[UPDATEKO]"

#define NEW_CONN            1

typedef struct {
    char        type;
    char        header[10];
    uint16_t    length;
    char*       data;
} Trama;

//Variables globals externes
extern Enterprise enterprise;
extern int connectionFlag;

void gestionaConnexioData(int new);
int desconnecta(int sockfd, int new);
int connectaData();
int enviaNovaConnexio(int sockfd, int new);
Trama readTrama(int sockfd, int* error);
void writeTrama(int sockfd, char type, char header[10], char* data);


void * threadFunc(void * arg);
void creaThread();
void alarmSignal();

#endif
