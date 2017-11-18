#ifndef   _IO_H_
#define   _IO_H_

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "dades.h"

#define ERROR_ARG       "Error! No s'ha passat el nombre de paràmetres que pertoca.\n"
#define INTRO_COMAND    "Introdueixi comandes...\n"
#define DIS_MSG         "Gràcies per fer servir LsEat. Fins la propera.\n"
#define ERROR_COMAND    "Comanda no reconeguda\n"
#define ERROR_CONN      "Error! Ja estàs connectat.\n"
#define ERROR_NCONN     "Error! No t'has connectat.\n"
#define ERROR_CONNECT   "Error de connexion con el servidor.\n"
#define ERROR_SOCK      "Error en crear el socket.\n"

#define CONNECT         "CONNECTA"
#define SHOW            "MOSTRA"
#define MENU            "MENU"
#define ORDER           "DEMANA"
#define DELETE          "ELIMINA"
#define PAY             "PAGAR"
#define DISCONNECT      "DESCONNECTA"

#define COMANDA_OK      "[Comanda OK]\n"

#define ERROR_FILE1 "Error! No s'ha pogut obrir el fitxer especificat.\n"
#define ERROR_FILE2 "Error! El fitxer especificat està buit o no compleix el format.\n"

//Funcions de lectura
int readFile(char* path, Picard* picard);
char* readUntil(int fd, char cFi);
char* readKB();
int inputFlush();

//Funcions d'escriptura
void printWelcome(char* name);
void printMoney(int money);
void printShell(char* name);


#endif
