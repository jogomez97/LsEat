#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "data.h"

#define CONNECTEDP          "[DATA] Connexió amb picard establerta\n"
#define DESCONP             "[DATA] Desconnexió de Picard\n"
#define WAIT_CONNECT        "[DATA] Esperant una nova connexió\n"
#define CONNECTED_E         "[DATA] Connexió establerta amb enterprise\n"
#define DISCONNECTED_E      "[DATA] Desconnexió d'enterprise\n"
#define ERROR_SOCK          "Error en crear el socket!\n"
#define ERROR_CONNECT       "Error de connexion con el servidor.\n"
#define ERROR_ACCEPT        "Error en acceptar!\n"
#define ERROR_BIND          "Error en fer el bind!\n"
#define ERROR_TRAMA         "Error en la trama!\n"
#define ERROR_DISCONNECTED  "Error! S'ha perdut la connexió\n"
#define NCONN 10
#define PORT 8260

#define ENT_INF     "[ENT_INF]\0"
#define CONOK       "CONOK\0"
#define CONKO       "CONKO\0"
#define CONOKb      "[CONOK]\0"       //b de brackets
#define CONKOb      "[CONKO]\0"

typedef struct {
    char        type;
    char        header[10];
    uint16_t    length;
    char*       data;
} Trama;

int connectPicard(Data d);
int connectEnterprise(Data d);
void gestionaEnterprise(int clientfd);
void gestionaPicard(int clientfd);
Trama readTrama(int clientfd, int* error);
void writeTrama(int sockfd, char type, char header[10], char* data);

#endif
