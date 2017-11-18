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

int connectaData(int connectat, Picard picard);

void show(int connectat);

void order(int connectat);

void delete(int connectat);

void pay(int connectat);

void disconnect(int connectat);

#endif
