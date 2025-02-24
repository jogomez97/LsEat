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

#define   _GNU_SOURCE 1

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
#include <ctype.h>

// Llibreries pròpies
#include "dades.h"
#include "list.h"

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
#define REJECTED_D          "[ENTERPRISE] Data ha rebutjat la teva peticio\n"
#define WAITING             "Esperant clients...\n"
#define SENDING_MENU        "Enviant Menu...\n"
#define SENT_MENU           "Menu enviat\n"

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
#define SHW_MENU            "[SHW_MENU]\0"
#define DISH                "[DISH]\0"
#define END_MENU            "[END_MENU]\0"
#define NEW_ORD             "[NEW_ORD]\0"
#define ORDOK               "[ORDOK]\0"
#define ORDKO               "[ORDKO]\0"
#define ORDKO2              "[ORDKO2]\0"
#define ORDKO3              "[ORDKO3]\0"
#define DEL_ORD             "[DEL_ORD]\0"
#define PAY_C               "[PAY]\0"
#define PAYOK               "[PAYOK]\0"
#define PAYKO               "[PAYKO]\0"
#define ENT_KO              "[ENT_KO]\0"
#define DISH_KO             "[DISH_KO]\0"

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
extern pthread_mutex_t mtxMenu;
extern List clients;

/******************************************************************************/
/************************ FUNCIONS CONNEXIÓ AMB DATA **************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     gestionaConnexioData
* @Purpose  Funció que gestiona tota la primera connexió amb data
* @Param    In: new     indica si és la primera connexió (1) o no (0)
*           Out: -
* @return   -
*
*******************************************************************************/
void gestionaConnexioData(int new);

/*******************************************************************************
*
* @Name     desconnecta
* @Purpose  Funció que ens permet desconnectar-nos de Data de dues maneres:
*               - enviant la trama de desconnexió si és la primera connexió
*               - tancant el socket directament si és un update
* @Param    In: sockfd    Socket de Data del qual ens volem desconnectar
*               dead      indica si hem de communicar-nos amb Data (1) o no (0)
*           Out: -
* @return  Retorna 1 si s'ha tancat el socket, 0 altrament
*
*******************************************************************************/
int desconnecta(int sockfd, int new);

/*******************************************************************************
*
* @Name     connectaData
* @Purpose  Funció que ens permet establir connexió amb Data mitjançant un Socket
* @Param    In:  -
*           Out: -
* @return   Retorna el valor del fd del Socket si s'ha pogut establir connexió,
*           -1 altrament
*
*******************************************************************************/
int connectaData();

/*******************************************************************************
*
* @Name     enviaNovaConnexio
* @Purpose  Funció que permet gestiona l'enviament i tractament de les
*           diferents Trames de Data, depenent de si és el primer cop que ens
*           connectem o no.
* @Param    In: sockfd   Socket de Data al qual estem connectats
*               new      indica si és la primera connexió (1) o no (0)
*           Out: -
* @return   Retorna 0 si la communicació ha resultat satisfactòria, -1 altrament.
*
*******************************************************************************/
int enviaNovaConnexio(int sockfd, int new);


/******************************************************************************/
/************************ FUNCIONS CONNEXIÓ DE PICARDS ************************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     engegaServidor
* @Purpose  Thread corresponent al servidor general que atén les noves
*           peticions de connexió de Picards, tot creant un servidor dedicat
*           per cadascun d'ells.
* @Param    In: arg    Argument void* per prototip
*           Out: -
* @return   Retorna un void* per prototip
*
*******************************************************************************/
void* engegaServidor(void* arg);

/*******************************************************************************
*
* @Name     threadPicard
* @Purpose  Thread en funció de servidor dedicat que gestiona la communicació
*           amb un sol Picard
* @Param    In: arg     Argument void* per prototip, es fa servir per
*                       obtindre el fd del Socket del Picard
*           Out: -
* @return   Retorna un void* per prototip
*
*******************************************************************************/
void* threadPicard(void * arg);

/*******************************************************************************
*
* @Name     getPlatFromTrama
* @Purpose  Funció que retornarà un plat a partir de dades d'una trama
* @Param    In:  data       dades de la trama
*           Out: -
* @return   Retorna el plat amb les dades de la trama
*
*******************************************************************************/
Plat getPlatFromTrama(char* data);

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
