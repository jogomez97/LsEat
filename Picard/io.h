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
#define CONNECTED_D     "[PICARD] Connexió establerta amb Data\n"
#define DISCONNECTED_D  "[PICARD] Desconnexió amb data\n"
#define CONNECTED_E     "[PICARD] Connexió establerta amb enterprise\n"
#define DISCONNECTED_E  "[PICARD] Desconnexió amb enterprise\n"
#define ERROR_DISCON_E  "[PICARD] Error en la desconnexió amb enterprise\n"
#define ERROR_DATA      "[PICARD] Error amb la connexió amb Data\n"
#define ERROR_ESTABLISHED "[PICARD] Connexió amb Enterprise ja realitzada!\n"

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

//Variables globals
extern Picard picard;

//Funcions de lectura
int readFile(char* path, Picard* picard);
char* readUntil(int fd, char cFi);
char* readKB();
int inputFlush();

//Funcions d'escriptura
void printWelcome();
void printMoney();
void printShell();


#endif
