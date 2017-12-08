/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Data
* Curs 2017-2018
*
* @File     communication.h
* @Purpose  Modul que conté les funcions relacionades amb les diferents connexions
*           de Enterprise a Data i de Picard a Data
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
*******************************************************************************/

#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

// Llibreries del sistema
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

// Llibreries pròpies
#include "data.h"
#include "list.h"

// Declaració de constants
#define CONNECTEDP          "[DATA] Connexió amb picard establerta\n"
#define DESCONP             "[DATA] Desconnexió de Picard\n"
#define WAIT_CONNECTE       "[DATA] Esperant una nova connexió d'Enterprise\n"
#define WAIT_CONNECTP        "[DATA] Esperant una nova connexió de Picard\n"
#define CONNECTED_E         "[DATA] Connexió establerta amb enterprise\n"
#define DISCONNECTED_E      "[DATA] Desconnexió d'enterprise\n"
#define CONNECTED_P         "[DATA] Connexió amb Picard establerta\n"
#define DISCONNECTED_P      "[DATA] Desconnexió de Picard\n"
#define ERROR_SOCK          "Error en crear el socket!\n"
#define ERROR_CONNECT       "Error de connexion con el servidor.\n"
#define ERROR_ACCEPT        "Error en acceptar!\n"
#define ERROR_BIND          "Error en fer el bind!\n"
#define ERROR_TRAMA         "Error en la trama!\n"
#define ERROR_DISCONNECTED  "Error! S'ha perdut la connexió\n"
#define NCONN 10
#define PORT 8260

#define ENT_INF             "[ENT_INF]\0"
#define CONOK               "CONOK\0"
#define CONKO               "CONKO\0"
#define CONOKb              "[CONOK]\0"       //b de brackets
#define CONKOb              "[CONKO]\0"
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
extern List flota;
extern Data d;
extern pthread_t threadEnterprise;
extern int sockfd;
extern int clientfd;
extern int sockfdPicard;
extern int clientfdPicard;
extern Trama tramaPicard;
extern Trama trama;

/******************************************************************************/
/***************************** FUNCIONS DE PICARD *****************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     connectPicard
* @Purpose  Funció que realitzarà la connexió amb Picard per a poder després
*           gestionar-lo
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
int connectPicard();

/*******************************************************************************
*
* @Name     gestionaPicard
* @Purpose  Funció que gestionarà la interacció Data-Picard mitjançant sockets
* @Param    In:  clientfd   File Descriptor associat al Picard
*           Out: -
* @return   -
*
*******************************************************************************/
void gestionaPicard();

/******************************************************************************/
/*************************** FUNCIONS DE ENTERPRISE ***************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     connectEnterprise
* @Purpose  Funció que realitzarà la connexió estable amb Enterprise per a poder
*           després gestionar-la.
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
int connectEnterprise();

/*******************************************************************************
*
* @Name     gestionaEnterprise
* @Purpose  Funció que ens gestionarà tota la connexió relacionada amb una Enterprise,
*           escoltant el que envia i responent-li com demana el protocol.
* @Param    In: -
*           Out: -
* @return   -
*
*******************************************************************************/
void gestionaEnterprise();
void gestionaPicard();
Trama readTrama(int clientfd, int* error);
void writeTrama(int sockfd, char type, char header[10], char* data);

/*******************************************************************************
*
* @Name     threadFunc
* @Purpose  Funció que ens realitzarà el thread, encarregada de connectar una
*           Enterprise a Data.
* @Param    In: arg Paràmetre que sempre demana les funcions associades a Threads
*           Out: -
* @return   -
*
*******************************************************************************/
void * threadFunc(void * arg);

/*******************************************************************************
*
* @Name     creaThread
* @Purpose  Funció que crea un thread amb la nostra funció assignada a els Threads
*           de Data.
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void creaThread();

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
Trama readTrama(int clientfd, int* error);
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
Enterprise getEnterpriseFromTrama(char* data);

#endif
