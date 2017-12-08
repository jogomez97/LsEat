/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Data
* Curs 2017-2018
*
* @File     list.h
* @Purpose  Modul que conté la llista ordenada i les seves funcions per a implementar
*           fàcilment el balancejador de càrrega.
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
#include "data.h"

// Declaració de constants
#define ERROR_PRINT "Error en printar. La llista està buida!\n"
#define DEBUG_LIST  0

// Definició de tipus propis
typedef struct {
    char* nom;
    int   port;
    char* ip;
    int   nConnections;
} Enterprise;

typedef struct node_t {
    Enterprise  enterprise;
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
*                  e Element a afegir (Enterprise)
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int insertNode(List* l, Enterprise e);

/*******************************************************************************
*
* @Name     insertNodeSorted
* @Purpose  Funció que afegirà un element a la llista de manera ordenada
* @Param    In:    l Llista on afegirem l'element
*                  e Element a afegir (Enterprise)
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int insertNodeSorted(List* l, Enterprise e);

/*******************************************************************************
*
* @Name     deleteNode
* @Purpose  Funció que elimina un element de la llista
* @Param    In:    l Llista on esborrarem l'element
*                  e Element a eliminar (Enterprise)
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int deleteNode(List* l, Enterprise e);

/*******************************************************************************
*
* @Name     deleteFirstNode
* @Purpose  Funció que elimina el primer element de la llista
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
* @Purpose  Funció que busca una Enterprise a la llista
* @Param    In:     l  Llista on buscar l'Enterprise
*                   e  Enterprise a buscar
*           Out: -
* @return   Retorna l'element de la llista en cas que el trobi
*
*******************************************************************************/
Node* searchNode(List* l, Enterprise e);

/*******************************************************************************
*
* @Name     searchBiggerNode
* @Purpose  Funció que busca una Enterprise de la llista amb més connexions que
*           una Enterprise concreta
* @Param    In:     l  Llista on buscar l'Enterprise
*                   e  Enterprise amb el que comparar el nombre de connexions
*           Out: -
* @return   Retorna una Enterprise amb més connexions que la donada, en cas que
*           n'hi hagi
*
*******************************************************************************/
Node* searchBiggerNode(List* l, Enterprise e);

/*******************************************************************************
*
* @Name     checkFirstElement
* @Purpose  Funció que retorna el primer element (Enterprise) de la llista, sempre
*           i quan aquesta no estigui buida.
* @Param    In:     l  Llista on consultar l'Element
*           Out: -
* @return   Retorna el primer element (Enterprise) de la llista
*
*******************************************************************************/
Enterprise checkFirstElement(List* l);

/*******************************************************************************
*
* @Name     sortFirstNode
* @Purpose  Funció que afegeix una connexió al primer Enterprise i el torna a inserir
*           per a reordenar la llista
* @Param    In:     l  Llista a modificar
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int sortFirstNode(List* l);

/*******************************************************************************
*
* @Name     sortFirstNode
* @Purpose  Funció que actualitza la informació d'un Enterprise
* @Param    In:     l  Llista on modificar l'Enterprise
*                   e  Enterprise a actualitzar
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int updateNode(List* l, Enterprise e);

#endif
