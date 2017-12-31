/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Picard
* Curs 2017-2018
*
* @File     list.c
* @Purpose  Modul que conté la llista no ordenada i les seves funcions per a poder
            gestionar els clients picard
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

// Llibreries pròpies
#include "list.h"

/*******************************************************************************
*
* @Name     createList
* @Purpose  Funció que crearà la llista i la inicialitzarà
* @Param    In:  -
*           Out: -
* @return   Retorna la llista creada
*
*******************************************************************************/
List createList() {
    List l;
    l.first = NULL;
    l.last = NULL;
    return l;
}

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
int insertNode(List* l, Picard p) {
    //Create Node
    Node* aux;
    aux = (Node*)malloc(sizeof(Node));
    if (aux == NULL) {
        return -1;
    }
    aux->picard = p;
    aux->next = NULL;

    //Modify list structure
    if (isEmpty(l)) {
        aux->pre = NULL;
        l->first = aux;
        l->last = aux;
        return 0;
    }
    aux->pre = l->last;
    aux->next = NULL;
    l->last->next = aux;
    l->last = aux;

    return 0;

}

/*******************************************************************************
*
* @Name     deleteNode
* @Purpose  Funció que elimina un element de la llista
* @Param    In:    l    Llista on esborrarem l'element
*                  int  file descriptor de l'Element a eliminar (Picard)
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int deleteNode(List* l, int fd) {
    if (isEmpty(l)) {
        return -1;
    }
    Node* aux = l->first;

    while (aux != l->last) {
        if (aux->picard.fd == fd) {
            if (aux == l->first) {
                aux->next->pre = NULL;
                l->first = aux->next;
                freeNodeInfo(aux);
                free(aux);
                return 0;
            }

            aux->pre->next = aux->next;
            aux->next->pre = aux->pre;
            freeNodeInfo(aux);
            free(aux);
            return 0;
        }
        aux = aux->next;
    }

    //Si el que s'elimina és l'últim element
    if (aux->picard.fd == fd) {
        if (aux == l->first) {
            freeNodeInfo(aux);
            free(aux);
            l->last = NULL;
            l->first = NULL;
            return 0;
        }
        aux->pre->next = NULL;
        l->last = aux->pre;
        freeNodeInfo(aux);
        free(aux);
        return 0;
    }

    return -1;
}

/*******************************************************************************
*
* @Name     freeNodeInfo
* @Purpose  Funció que allibera tots els recursos d'un Picard
* @Param    In:    Node* node del qual volem alliberar recursos
*           Out: -
* @return   -
*
*******************************************************************************/
void freeNodeInfo(Node* aux) {
    int i;

    free(aux->picard.nom);
    for (i = 0; i < aux->picard.nPlats; i++) {
        free(aux->picard.plats[i].nom);
    }

    free(aux->picard.plats);
}


 /*******************************************************************************
 *
 * @Name     deleteFirstNode
 * @Purpose  Funció que elimina el primer element de la llista tot tancant el fd
 * @Param    In:    l Llista on esborrarem l'element
 *           Out: -
 * @return   Enter de control d'errors. 0 = success, -1 otherwise
 *
 *******************************************************************************/
int deleteFirstNode(List* l) {
    if (isEmpty(l)) {
        return -1;
    }
    Node* aux = l->first;

    if (aux == l->last) {
        l->last = NULL;
    }

    l->first = aux->next;
    close(aux->picard.fd);
    freeNodeInfo(aux);
    free(aux);

    return 0;

}

/*******************************************************************************
*
* @Name     printList
* @Purpose  Funció que printa la Llista per pantalla
* @Param    In:    l Llista a printar
*           Out: -
* @return   -
*
*******************************************************************************/
void printList(List* l) {
    if (isEmpty(l)) {
        write(1, ERROR_PRINT, strlen(ERROR_PRINT));
    } else {
        int i = 1;
        char buff[50];
        Node *aux = l->first;

        while (aux != l->last){
            sprintf(buff, "fd: %d plats:%d-", aux->picard.fd, aux->picard.nPlats);
            write(1, buff, strlen(buff));
            aux = aux->next;
            i++;
        }
        sprintf(buff, "fd: %d plats:%d\n", aux->picard.fd, aux->picard.nPlats);
        write(1, buff, strlen(buff));
    }

}

/*******************************************************************************
*
* @Name     isEmpty
* @Purpose  Funció que ens diu si la llista està o no buida
* @Param    In:    l Llista a mirar si està buida
*           Out: -
* @return   Enter que indica si la llista està buida
*
*******************************************************************************/
int isEmpty(List* l) {
    return l->first == NULL;
}

/*******************************************************************************
*
* @Name     eraseList
* @Purpose  Funció que destrueix la llista
* @Param    In:    l Llista a destruir
*           Out: -
* @return   -
*
*******************************************************************************/
void eraseList(List* l) {
    while (!isEmpty(l)) {
        deleteFirstNode(l);
    }
}

/*******************************************************************************
*
* @Name     searchNode
* @Purpose  Funció que busca una Picard a la llista
* @Param    In:     l  Llista on buscar l'Picard
*                   fd file descriptor del Picard a trobar
*           Out: -
* @return   Retorna l'element de la llista en cas que el trobi
*
*******************************************************************************/
Node* searchNode(List* l, int fd) {
    Node* aux = l->first;

    while (aux != l->last) {
        if (aux->picard.fd == fd) {
            return aux;
        }
        aux = aux->next;
    }

    if (aux->picard.fd == fd) {
        return aux;
    }
    return NULL;
}

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
Picard checkFirstElement(List* l) {
    if (isEmpty(l)) {
        Picard p;
        p.fd = -1;
        return p;
    } else {
        return l->first->picard;
    }
}

/*******************************************************************************
*
* @Name     checkLastElementFd
* @Purpose  Funció que retorna el fd de l'ultim element (Picard) de la llista, sempre
*           i quan aquesta no estigui buida.
* @Param    In:     l  Llista on consultar l'Element
*           Out: -
* @return   Retorna el fd l'ultim element (Picard) de la llista
*
*******************************************************************************/
int* checkLastElementFd(List* l) {
    if (isEmpty(l)) {
        int* fd = (int*) malloc(sizeof(int));
        *fd = -1;
        return fd;
    } else {
        return &l->last->picard.fd;
    }
}

/******************************************************************************/
/************************ FUNCIONS ACTUALITZACIÓ PICARD ***********************/
/******************************************************************************/

int addNameToElement(List* l, int fd, char* name) {
    if (isEmpty(l)) {
        return -1;
    }

    Node* aux = searchNode(l, fd);
    if (aux == NULL) {
        return -1;
    }

    aux->picard.nom = strdup(name);

    return 0;
}
