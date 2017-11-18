#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ERROR_ARGC "Error! No s'han passat els arguments necessaris!\n"
#define ERROR_OPEN "Error! No s'ha pogut obrir el fitxer.\n"
#define ERROR_MESSAGE   "Opció incorrecta.\n"
#define ERROR_SOCK  "Error en crear el socket!\n"
#define ERROR_ACCEPT "Error en acceptar!\n"
#define ERROR_CONNECT "Error de connexió amb Data.\n"

typedef struct {
    char        type;
    char        header[10];
    uint16_t    length;
    char*       data;
} Trama;

int connectaData(char* ip, int port);

#endif
