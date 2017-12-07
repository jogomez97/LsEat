/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Enterprise
* Curs 2017-2018
*
* @File     communication.h
* @Purpose  Modul que conté les funcions relacionades amb les diferents connexions
*           de Enterprise a Data i de Picard a Enterprise
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
*******************************************************************************/

#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

// Llibreries del sistema
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

// Llibreries pròpies
#include "dades.h"

// Declaració de constants
#define ERROR_BIND          "Error en fer el bind!\n"
#define NCONN               10

#define ERROR_ARGC          "Error! No s'han passat els arguments necessaris!\n"
#define ERROR_OPEN          "Error! No s'ha pogut obrir el fitxer.\n"
#define ERROR_MESSAGE       "Opció incorrecta.\n"
#define ERROR_SOCK          "Error en crear el socket!\n"
#define ERROR_ACCEPT        "Error en acceptar!\n"
#define ERROR_CONNECT       "Error de connexió amb Data.\n"
#define ERROR_TRAMA         "Error en la trama!\n"
#define ERROR_DISCONNECTED  "Error! Data s'ha desconnectat\n"
#define ERROR_DISCONNECTEDP "Error! Picard s'ha desconnectat\n"
#define CONNECTED_D         "[ENTERPRISE] Connexió establerta amb Data\n"
#define DISCONNECTED_D      "[ENTERPRISE] Desconnexió de Data\n"
#define CONNECTED_P         "[ENTERPRISE] Connexió establerta amb Picard\n"
#define DISCONNECTED_P      "[ENTERPRISE] Desconnexió de Picard\n"
#define WAITING             "Esperant clients...\n"

#define NEW_CONN            1

#define PIC_INF             "[PIC_INF]"
#define PIC_NAME            "[PIC_NAME]"
#define ENT_INF             "[ENT_INF]"
#define CONOK               "CONOK"
#define CONKO               "CONKO"
#define CONOKb              "[CONOK]"       //b de brackets
#define CONKOb              "[CONKO]"
#define UPDATE              "[UPDATE]"
#define UPDATEOK            "[UPDATEOK]"
#define UPDATEKO            "[UPDATEKO]"

// Definició de tipus propis
typedef struct {
    char        type;
    char        header[10];
    uint16_t    length;
    char*       data;
} Trama;

// Variables globals externes
extern Enterprise enterprise;
extern int connectionFlag;
extern pthread_mutex_t mtx;

/******************************************************************************/
/************************ FUNCIONS CONNEXIÓ AMB DATA **************************/
/******************************************************************************/
void gestionaConnexioData(int new);
int desconnecta(int sockfd, int new);
int connectaData();
int enviaNovaConnexio(int sockfd, int new);


/******************************************************************************/
/************************ FUNCIONS CONNEXIÓ DE PICARDS ************************/
/******************************************************************************/
void* engegaServidor(void* arg);
void* threadPicard(void * arg);

/******************************************************************************/
/**************************** FUNCIONS GENÈRIQUES *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     readTrama
* @Purpose  Funció llegirà una Trama donat un fd associat a un socket
* @Param    In: clientfd    Socket del que rebrem la trama
*           Out: error      Variable de control d'errors
* @return   Retorna la Trama llegida en cas de no haver-hi errors
*
*******************************************************************************/
Trama readTrama(int sockfd, int* error);

/*******************************************************************************
*
* @Name     writeTrama
* @Purpose  Funció escriurà una Trama donat un fd associat a un socket
* @Param    In: clientfd    Socket al que escriurem la trama
*               type        Type de la trama a enviar
*               header      Header de la trama a enviar
*               data        Data de la trama a enviar
*           Out: -
* @return   Retorna la Trama llegida en cas de no haver-hi errors
*
*******************************************************************************/
void writeTrama(int sockfd, char type, char header[10], char* data);

#endif
