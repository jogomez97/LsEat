#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ERROR_ARGC "Error! No se han pasado los arg necesarios!\n"
#define ERROR_OPEN "Error! No se ha podido abrir un fichero.\n"
#define ERROR_MESSAGE   "Opcion incorrecta.\n"
#define ERROR_SOCK  "Error en crear el socket!\n"
#define ERROR_ACCEPT "Error en acceptar!\n"
#define ERROR_CONNECT "Error de connexion con el servidor.\n"

typedef struct {
    char        type;
    char        header[10];
    uint16_t    length;
    char*       data;
} Trama;

int connectaData(char* ip, int port);

#endif
