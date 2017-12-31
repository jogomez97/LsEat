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
#define DEBUG_LIST  0

// Definició de tipus propis
typedef struct {
    char*   nom;
    int     quants;
    int     preu;
} Plat;

typedef struct {
    char*   nom;
    int     fd;
    int     compte;
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

int addNameToElement(List* l, int fd, char* name);


#endif
