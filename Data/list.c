/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Data
* Curs 2017-2018
*
* @File     list.c
* @Purpose  Modul que conté la llista ordenada i les seves funcions per a implementar
*           fàcilment el balancejador de càrrega.
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
 *                  e Element a afegir (Enterprise)
 *           Out: -
 * @return   Enter de control d'errors. 0 = success, -1 otherwise
 *
 *******************************************************************************/
int insertNode(List* l, Enterprise e) {
    //Create Node
    Node* aux;
    aux = (Node*)malloc(sizeof(Node));
    if (aux == NULL) {
        return -1;
    }
    aux->enterprise = e;
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
* @Name     insertNodeSorted
* @Purpose  Funció que afegirà un element a la llista de manera ordenada
* @Param    In:    l Llista on afegirem l'element
*                  e Element a afegir (Enterprise)
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int insertNodeSorted(List* l, Enterprise e) {
    //Create Node
    Node* aux;
    aux = (Node*)malloc(sizeof(Node));
    if (aux == NULL) {
        return -1;
    }
    aux->enterprise = e;
    aux->next = NULL;

    //Insert al principi de tot
    if (isEmpty(l)) {
        aux->pre = NULL;
        l->first = aux;
        l->last = aux;
        return 0;
    }

    Node* after = searchBiggerNode(l, e);
    //Insert al final de tot
    if (after == NULL) {
        aux->next = NULL;
        aux->pre =  l->last;
        l->last->next = aux;
        l->last = aux;
        return 0;
    }

    //Insert davant de tot
    if (after == l->first) {
        aux->next = after;
        aux->pre = NULL;
        after->pre = aux;
        l->first = aux;
        return 0;
    }

    //Insert pel mig
    aux->next = after;
    aux->pre = after->pre;
    after->pre->next = aux;
    after->pre = aux;

    return 0;

}


/*******************************************************************************
*
* @Name     deleteNode
* @Purpose  Funció que elimina un element de la llista
* @Param    In:    l    Llista on esborrarem l'element
*                  int  Port de l'Element a eliminar (Enterprise)
*           Out: -
* @return   Enter de control d'errors. 0 = success, -1 otherwise
*
*******************************************************************************/
int deleteNode(List* l, int port) {
    if (isEmpty(l)) {
        return -1;
    }
    Node* aux = l->first;

    while (aux != l->last) {
        if (aux->enterprise.port == port) {
            if (aux == l->first) {
                aux->next->pre = NULL;
                l->first = aux->next;
                free(aux->enterprise.nom);
                free(aux->enterprise.ip);
                free(aux);
                return 0;
            }

            aux->pre->next = aux->next;
            aux->next->pre = aux->pre;
            free(aux->enterprise.nom);
            free(aux->enterprise.ip);
            free(aux);
            return 0;
        }
        aux = aux->next;
    }

    //Si el que s'elimina és l'últim element
    if (aux->enterprise.port == port) {
        if (aux == l->first) {
            free(aux->enterprise.nom);
            free(aux->enterprise.ip);
            free(aux);
            l->last = NULL;
            l->first = NULL;
            return 0;
        }
        aux->pre->next = NULL;
        l->last = aux->pre;
        free(aux->enterprise.nom);
        free(aux->enterprise.ip);
        free(aux);
        return 0;
    }

    return -1;

}


 /*******************************************************************************
 *
 * @Name     deleteFirstNode
 * @Purpose  Funció que elimina el primer element de la llista
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
    free(aux->enterprise.nom);
    free(aux->enterprise.ip);
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
        char buff[100];
        Node *aux = l->first;

        while (aux != l->last){
            sprintf(buff, "(%d)%d(%s.%s)-", aux->enterprise.port, aux->enterprise.nConnections,  aux->enterprise.nom, aux->enterprise.ip);
            write(1, buff, strlen(buff));
            aux = aux->next;
            i++;
        }
        sprintf(buff, "(%d)%d(%s.%s)\n", aux->enterprise.port, aux->enterprise.nConnections, aux->enterprise.nom, aux->enterprise.ip);
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
* @Purpose  Funció que busca una Enterprise a la llista
* @Param    In:     l  Llista on buscar l'Enterprise
*                   e  Enterprise a buscar
*           Out: -
* @return   Retorna l'element de la llista en cas que el trobi
*
*******************************************************************************/
Node* searchNode(List* l, Enterprise e) {
    Node* aux = l->first;

    while (aux != l->last) {
        if (aux->enterprise.port == e.port) {
            return aux;
        }
        aux = aux->next;
    }

    if (aux->enterprise.port == e.port) {
        return aux;
    }
    return NULL;
}

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
Node* searchBiggerNode(List* l, Enterprise e) {
    Node* aux = l->first;

    while (aux != l->last) {
        if (aux->enterprise.nConnections > e.nConnections) {
            return aux;
        }
        aux = aux->next;
    }

    if (aux->enterprise.nConnections > e.nConnections) {
        return aux;
    }

    return NULL;

}

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
Enterprise checkFirstElement(List* l) {
    if (isEmpty(l)) {
        Enterprise e;
        e.nom = NULL;
        return e;
    } else {
        return l->first->enterprise;
    }
}

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
int sortFirstNode(List* l) {
    if (isEmpty(l)) {
        return -1;
    }

    Enterprise e;
    e.nom = strdup(l->first->enterprise.nom);
    e.port = l->first->enterprise.port;
    e.ip =  strdup(l->first->enterprise.ip);
    e.nConnections = l->first->enterprise.nConnections + 1;

    deleteFirstNode(l);
    insertNodeSorted(l, e);

    return 0;

}

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
int updateNode(List* l, Enterprise e) {
    if (isEmpty(l)) {
        return -1;
    }

    Node* aux = searchNode(l, e);
    if (aux != NULL) {
        e.nom = strdup(aux->enterprise.nom);
        e.ip = strdup(aux->enterprise.ip);

        deleteNode(l, e.port);
        insertNodeSorted(l, e);
        return 0;
    }

    return -1;


}
