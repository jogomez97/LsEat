#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "io.h"
#include "dades.h"

#define PIC_NAME    "[PIC_NAME]\0"
#define PIC_INF     "[PIC_INF]\0"
#define ENT_INF     "[ENT_INF]\0"
#define DATA        1
#define ENTERPRISE  0

typedef struct {
    char        type;
    char        header[10];
    uint16_t    length;
    char*       data;
} Trama;

int connectaServidor(int connectat, Picard picard, int mode, Enterprise* e);

void show(int connectat);

void order(int connectat);

void delete(int connectat);

void pay(int connectat);

void disconnect(int connectat);

int gestionaTrama(Trama t, int mode);

Trama readTrama(int clientfd, int* error);

void writeTrama(int sockfd, char type, char header[10], char* data);

#endif
