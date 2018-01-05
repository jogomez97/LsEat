/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @File     list.h
* @Purpose  Modul que conté la llista no ordenada i les seves funcions per a poder
            gestionar els clients picard
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

#ifndef _LIST_H_
#define _LIST_H_

// Llibreries del sistema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Llibreries pròpies

// Declaració de constants
#define ERROR_PRINT "Error en printar. La llista està buida!\n"
#define NO_DISHES   "Aquest Picard no té cap plat reservat\n"
#define DEBUG_LIST  0
#define DEBUG_LIST2 1

// Definició de tipus propis
typedef struct {
    char*   nom;
    int     quants;
    int     preu;
} Plat;

typedef struct {
    char*   nom;
    int     fd;
    int     targeta;
    int     nPlats;
    Plat*   plats;
} Picard;

typedef struct node_t {
    Picard      picard;
    struct      node_t* next;
    struct      node_t* pre;
} Node;

typedef struct {
    Node*   last;
    Node*   first;
} List;

typedef struct {
    int   nPlats;
    Plat* plats;
} Plats;

/*******************************************************************************
*
* @Name     createList
* @Purpose  Funció que crearà la llista i la inicialitzarà
* @Param    In:  -
*           Out: -
* @return   Retorna la llista creada
*
*******************************************************************************/
List createList();

/*******************************************************************************
*
* @Name     insertNode
* @Purpose  Funció que afegirà un element a la llista
* @Param    In:    l Llista on afegirem l'element
*                  p Element a afegir (Picard)
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int insertNode(List* l, Picard p);

/*******************************************************************************
*
* @Name     deleteNode
* @Purpose  Funció que elimina un element de la llista
* @Param    In:    l    Llista on esborrarem l'element
*                  fd   file descriptor de l'Element a eliminar (Picard)
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int deleteNode(List* l, int fd);

/*******************************************************************************
*
* @Name     freeNodeInfo
* @Purpose  Funció que allibera tots els recursos d'un Picard
* @Param    In:    Node* node del qual volem alliberar recursos
*           Out: -
* @return   -
*
*******************************************************************************/
void freeNodeInfo(Node* aux);

/*******************************************************************************
*
* @Name     deleteFirstNode
* @Purpose  Funció que elimina el primer element de la llista tot tancant el fd
* @Param    In:    l Llista on esborrarem l'element
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int deleteFirstNode(List* l);

/*******************************************************************************
*
* @Name     printList
* @Purpose  Funció que printa la Llista per pantalla
* @Param    In:    l Llista a printar
*           Out: -
* @return   -
*
*******************************************************************************/
void printList(List* l);

/*******************************************************************************
*
* @Name     isEmpty
* @Purpose  Funció que ens diu si la llista està o no buida
* @Param    In:    l Llista a mirar si està buida
*           Out: -
* @return   Enter que indica si la llista està buida
*
*******************************************************************************/
int isEmpty(List* l);

/*******************************************************************************
*
* @Name     eraseList
* @Purpose  Funció que destrueix la llista
* @Param    In:    l Llista a destruir
*           Out: -
* @return   -
*
*******************************************************************************/
void eraseList(List* l);

/*******************************************************************************
*
* @Name     searchNode
* @Purpose  Funció que busca un Picard a la llista
* @Param    In:     l  Llista on buscar l'Picard
*                   fd file descriptor del Picard a trobar
*           Out: -
* @return   Retorna l'element de la llista en cas que el trobi
*
*******************************************************************************/
Node* searchNode(List* l, int fd);

/*******************************************************************************
*
* @Name     checkFirstElement
* @Purpose  Funció que retorna el primer element (Picard) de la llista, sempre
*           i quan aquesta no estigui buida.
* @Param    In:     l  Llista on consultar l'Element
*           Out: -
* @return   Retorna el primer element (Picard) de la llista
*
*******************************************************************************/
Picard checkFirstElement(List* l);

/*******************************************************************************
*
* @Name     checkLastElement
* @Purpose  Funció que retorna el fd de l'ultim element (Picard) de la llista, sempre
*           i quan aquesta no estigui buida.
* @Param    In:     l  Llista on consultar l'Element
*           Out: -
* @return   Retorna el fd l'ultim element (Picard) de la llista
*
*******************************************************************************/
int* checkLastElementFd(List* l);


/******************************************************************************/
/************************ FUNCIONS ACTUALITZACIÓ PICARD ***********************/
/******************************************************************************/

/*******************************************************************************
*
* @Name     addNameToElement
* @Purpose  Funció que afegirà el nom a l'element que coincideixi amb el fd
* @Param    In:     l       Llista on consultar l'Element
                    fd      file descriptor propi de l'Element (Picard)
                    name    nom que volem atribuir a l'Element
                    money   diners al compte bancari que té un Element
*           Out:    int en control d'errors
* @return   Retorna el fd l'ultim element (Picard) de la llista
*
*******************************************************************************/
int addNameToElement(List* l, int fd, char* name, int money);

/*******************************************************************************
*
* @Name     addDishToElement
* @Purpose  Funció que afegirà el plat a l'element que coincideixi amb el fd
* @Param    In:     l       Llista on consultar l'Element
                    fd      file descriptor propi de l'Element (Picard)
                    p       Plat que volem afegir a la reserva de l'Element
*           Out:    -
* @return   retorna un int en control d'errors (-1), 0 si tot va bé
*
*******************************************************************************/
int addDishToElement(List* l, int fd, Plat p);

/*******************************************************************************
*
* @Name     removeDishFromElement
* @Purpose  Funció que eliminara el plat a l'element que coincideixi amb el fd
* @Param    In:     l       Llista on consultar l'Element
                    fd      file descriptor propi de l'Element (Picard)
                    p       Plat que volem eliminar de la reserva de l'Element
*           Out:    -
* @return   retorna 0 si s'ha pogut eliminar el plat correctament, -1 altrament
*
*******************************************************************************/
int removeDishFromElement(List* l, int fd, Plat p);

/*******************************************************************************
*
* @Name     searchForDish
* @Purpose  Funció que consultarà si un plat ja s'havia demanat amb anterioritat
* @Param    In:     aux     punter a l'element on volem consultar el plat
                    name    nom del plat amb el que volem comparar
*           Out:    -
* @return   retorna -1 si el plat no es troba encara reservat, 0 altrament
*
*******************************************************************************/
int searchForDish(Node* aux, char* name);

/*******************************************************************************
*
* @Name     payToAccount
* @Purpose  Funció que cobra l'import dels plats i elimina la reserva
* @Param    In:     l       Llista on consultar l'Element
                    fd      file descriptor propi de l'Element (Picard)
*           Out:    -
* @return   retorna l'import si s'ha pogut pagar, -1 altrament
*
*******************************************************************************/
int payToAccount(List* l, int fd);

/*******************************************************************************
*
* @Name     getDishInfo
* @Purpose  Funció que retorna la informació de tots els plats que ha demanat un
*           Picard
* @Param    In:     l       Llista on consultar l'Element
                    fd      file descriptor propi de l'Element (Picard)
*           Out:    -
* @return   retorna l'struct amb la informació dels plats
*
*******************************************************************************/
Plats getDishInfo(List* l, int fd);

/*******************************************************************************
*
* @Name     printDishes
* @Purpose  Funció pinta tots els plats reservats d'un picard
* @Param    In:     l       Llista on consultar l'Element
                    fd      file descriptor propi de l'Element (Picard)
*           Out:    -
* @return   retorna -1 si hi ha algun error, 0 altrament
*
*******************************************************************************/
int printDishes(List* l, int fd);

#endif
