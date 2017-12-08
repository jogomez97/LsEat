/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @File     communication.h
* @Purpose  Modul que conté les funcions relacionades amb les diferents connexions
*           de Picard a Data i Enterprise
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

// Llibreries del sistema
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

// Llibreries pròpies
#include "io.h"
#include "dades.h"
#include "utils.h"

// Declaració de constants
#define ERROR_CONNECT   "Error de connexió amb el servidor.\n"
#define ERROR_SOCK      "Error en crear el socket.\n"
#define CONNECTED_D     "[PICARD] Connexió establerta amb Data\n"
#define DISCONNECTED_D  "[PICARD] Desconnexió amb data\n"
#define CONNECTED_E     "[PICARD] Connexió establerta amb Enterprise\n"
#define DISCONNECTED_E  "[PICARD] Desconnexió amb enterprise\n"
#define ERROR_DISCON_E  "[PICARD] Error en la desconnexió amb Enterprise\n"
#define ERROR_DATA      "[PICARD] Error amb la connexió amb Data\n"
#define ERROR_ESTABLISHED "[PICARD] Connexió amb Enterprise ja realitzada!\n"

#define PIC_NAME    "[PIC_NAME]\0"
#define PIC_INF     "[PIC_INF]\0"
#define ENT_INF     "[ENT_INF]\0"
#define CONOK       "[CONOK]\0"
#define CONKO       "[CONKO]\0"
#define DSC_ENTERP  2
#define DATA        1
#define ENTERPRISE  0

// Definició de tipus propis
typedef struct {
    char        type;
    char        header[10];
    uint16_t    length;
    char*       data;
} Trama;

// Variables globals
extern char* comanda;
extern int   connectat;
extern int   sockfd;

/*******************************************************************************
*
* @Name     connectaServidor
* @Purpose  Funció que connectarà Picard a Enterprise o Data
* @Param    In: connectat   Indica si el Picard està o no connectat
*               picard      Picard a connectar
*               mode        Indica si hem de connectar a Data o Enterprise
*               e           Enterprise a la que ens hem de connectar
*           Out: -
* @return   Retorna diferents valors, indicant error o èxit en la connexió.
*           En cas d'èxit, l'enter retornat és el fd a emprar del Picard.
*
*******************************************************************************/
int connectaServidor(int connectat, Picard picard, int mode, Enterprise* e);

void show(int connectat);

void order(int connectat);

void delete(int connectat);

void pay(int connectat);

/*******************************************************************************
*
* @Name     disconnect
* @Purpose  Funció que desconnectarà Picard d'Enterprise (amb el protocol corresponent)
* @Param    In: connectat   Indica si el Picard està o no connectat
*               sockfd      File descriptor del que ens hem de desconnectar
*           Out: -
* @return   -
*
*******************************************************************************/
void disconnect(int connectat, int sockfd);

/*******************************************************************************
*
* @Name     gestionaTrama
* @Purpose  Funció interpretarà les trames i actuarà en funció del que calgui realitzar
* @Param    In: t       Trama a gestionar
*               mode    Mode de gestió de la trama
*           Out: -
* @return   Retorna un enter que indica error o èxit
*
*******************************************************************************/
int gestionaTrama(Trama t, int mode);

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

#endif
