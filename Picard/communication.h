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

#define   _GNU_SOURCE 1

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
#define ERROR_CONNECT       "Error de connexió amb el servidor.\n"
#define ERROR_SOCK          "Error en crear el socket.\n"
#define ERROR_TRAMA         "Error en la trama!\n"
#define CONNECTED_D         "[PICARD] Connexió establerta amb Data\n"
#define DISCONNECTED_D      "[PICARD] Desconnexió de Data\n"
#define CONNECTED_E         "[PICARD] Connexió establerta amb Enterprise\n"
#define DISCONNECTED_E      "[PICARD] Desconnexió amb Enterprise\n"
#define ERROR_DISCON_E      "[PICARD] Error en la desconnexió amb Enterprise\n"
#define ERROR_DATA          "[PICARD] Error amb la connexió amb Data\n"
#define ERROR_ESTABLISHED   "[PICARD] Connexió amb Enterprise ja realitzada!\n"
#define ERROR_E_DOWN        "[PICARD] Error! Enterprise ha caigut\n"
#define ERROR_NO_E_AVAIL    "[PICARD] Error! No hi ha cap Enterprise disponible\n"
#define MENU_DISP           "**** MENU DISPONIBLE ****\n"
#define ORD_CORRECT         "Rebut. S'ha reservat la seva comanda\n"
#define ORD_INCORRECT       "Error! No és possible servir aquesta comanda\n"
#define ORD_KO              "El plat demanat no es troba dins del Menu\n"
#define ORD_KO2             "No queden suficients unitats per abastir la comanda\n"
#define ORD_KO3             "No pots demanar un nombre negatiu de plats.\n"
#define DEL_CORRECT         "OK! Anotat el canvi en la comanda\n"
#define DEL_KO              "El plat no havia estat reservat\n"
#define DEL_KO2             "No s'havien reservat tantes unitats d'aquest plat\n"
#define PAY_KO              "Error en processar el pagament.\n"
#define REPEAT              "\nSi us plau, torni a introduir la comanda\n"

#define PIC_NAME    "[PIC_NAME]\0"
#define PIC_INF     "[PIC_INF]\0"
#define ENT_INF     "[ENT_INF]\0"
#define CONOK       "[CONOK]\0"
#define CONKO       "[CONKO]\0"
#define SHW_MENU    "[SHW_MENU]\0"
#define DISH        "[DISH]\0"
#define END_MENU    "[END_MENU]\0"
#define ORDOK       "[ORDOK]\0"
#define ORDKO       "[ORDKO]\0"
#define ORDKO2      "[ORDKO2]\0"
#define ORDKO3      "[ORDKO3]\0"
#define NEW_ORD     "[NEW_ORD]\0"
#define DEL_ORD     "[DEL_ORD]\0"
#define PAY_C       "[PAY]\0"
#define PAYOK       "[PAYOK]\0"
#define PAYKO       "[PAYKO]\0"
#define ENT_KO      "[ENT_KO]\0"
#define DISH_KO     "[DISH_KO]\0"

#define PAGAR       0x06
#define ELIMINA     0x05
#define DEMANA      0x04
#define M_MENU      0x03
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

/*******************************************************************************
*
* @Name     showDishFromTrama
* @Purpose  Funció que mostrarà un plat per pantalla que li ha enviat enterprise
* @Param    In: data  informació de la trama que ha enviat Enterprise
*           Out: -
* @return   Retorna 0 si la trama era correcta, -1 altrament
*
*******************************************************************************/
int showDishFromTrama(char* data);

/*******************************************************************************
*
* @Name     show
* @Purpose  Funció que mostrarà el Menu d'Enterprise per pantalla
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void show();

/*******************************************************************************
*
* @Name     order
* @Purpose  Funció que demanara un plat a Enterprise
* @Param    In:  plat       nom del plat a demanar
*                units      unitats del plat a demanar
*                reorder    indica si es tracta d'una reconnexió o no
*           Out: -
* @return   -
*
*******************************************************************************/
void order(char* plat, char* units, int reorder);

/*******************************************************************************
*
* @Name     delete
* @Purpose  Funció que eliminarà un nombre de plats de la comanda actual, si és
*           possible
* @Param    In: plat    Nom del plat a eliminar
*               units   Unitats del plat a eliminar de la comanda
*           Out: -
* @return   -
*
*******************************************************************************/
void delete(char* plat, char* units);

/*******************************************************************************
*
* @Name     pay
* @Purpose  Funció que pagarà la comanda actual, si és possible
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void pay();

/*******************************************************************************
*
* @Name     enviaTotsElsPlats
* @Purpose  Funció que enviarà tots els plats d'una comanda a una Enterprise per
*           a recuperar tots els encàrrecs possibles
* @Param    In: -
*           Out: -
* @return   -
*
*******************************************************************************/
void enviaTotsElsPlats();

/*******************************************************************************
*
* @Name     disconnect
* @Purpose  Funció que desconnectarà Picard d'Enterprise (amb el protocol corresponent)
* @Param    In:  -
*           Out: -
* @return   -
*
*******************************************************************************/
void disconnect();


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
