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

#define WAIT_CONNECT "Esperant una nova connexió\n"
#define CONNECTED    "Connexió establerta\n"
#define ERROR_SOCK  "Error en crear el socket!\n"
#define ERROR_CONNECT "Error de connexion con el servidor.\n"
#define ERROR_ACCEPT "Error en acceptar!\n"
#define ERROR_BIND "Error en fer el bind!\n"
#define ERROR_TRAMA "Error en la trama!\n"
#define ERROR_DISCONNECTED "Error! S'ha perdut la connexió\n"
#define NCONN 10
#define PORT 8260

typedef struct {
    char        type;
    char        header[10];
    uint16_t    length;
    char*       data;
} Trama;

int connectPicard(Data d);
int connectEnterprise(Data d);

#endif
