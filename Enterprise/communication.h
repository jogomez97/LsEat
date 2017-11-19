#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "dades.h"

#define ERROR_ARGC "Error! No s'han passat els arguments necessaris!\n"
#define ERROR_OPEN "Error! No s'ha pogut obrir el fitxer.\n"
#define ERROR_MESSAGE   "Opció incorrecta.\n"
#define ERROR_SOCK  "Error en crear el socket!\n"
#define ERROR_ACCEPT "Error en acceptar!\n"
#define ERROR_CONNECT "Error de connexió amb Data.\n"
#define ERROR_TRAMA "Error en la trama!\n"
#define ERROR_1STDATA  "Error en connectar-se per primer cop a Data\n"

#define ENT_INF     "[ENT_INF]\0"
#define CONOK       "CONOK\0"
#define CONKO       "CONKO\0"

typedef struct {
    char        type;
    char        header[10];
    uint16_t    length;
    char*       data;
} Trama;

//Variables globals externes
extern Enterprise enterprise;

void gestionaNovaConnexio();
int connectaData();
int enviaNovaConnexio(int sockfd);
Trama readTrama(int sockfd);
void writeTrama(int sockfd, char type, char header[10], char* data);

#endif
