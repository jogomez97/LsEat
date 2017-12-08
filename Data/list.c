//
// Created by Joan Gomez  on 3/12/17.
//

#include "list.h"

List createList() {
    List l;
    l.first = NULL;
    l.last = NULL;
    return l;
}

/**
 * Return 0 uppon success, -1 otherwise
 */
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


/*
* Retorna 0 si ha anat tot ok, -1 altrament.
* No allibera els punters de enterprise de nom i ip ja que s'han de fer servir
* per tornar a introduir-los actualitzats!
*/
int deleteNode(List* l, Enterprise e) {
    if (isEmpty(l)) {
        return -1;
    }
    Node* aux = l->first;

    while (aux != l->last) {
        if (aux->enterprise.port == e.port) {
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
    if (aux->enterprise.port == e.port) {
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

/**
 * Return 0 uppon success, -1 otherwise. Allibera punters de Enterprise
 */
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

void printList(List* l) {
    if (isEmpty(l)) {
        write(1, ERROR_PRINT, strlen(ERROR_PRINT));
    } else {
        int i = 1;
        char buff[10];
        Node *aux = l->first;

        while (aux != l->last){
            sprintf(buff, "(%d)%d-", aux->enterprise.port, aux->enterprise.nConnections);
            write(1, buff, strlen(buff));
            aux = aux->next;
            i++;
        }
        sprintf(buff, "(%d)%d\n", aux->enterprise.port, aux->enterprise.nConnections);
        write(1, buff, strlen(buff));
    }

}

int isEmpty(List* l) {
    return l->first == NULL;
}

void eraseList(List* l) {
    while (!isEmpty(l)) {
        deleteFirstNode(l);
    }
}

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

Enterprise checkFirstElement(List* l) {
    if (isEmpty(l)) {
        Enterprise e;
        e.nom = NULL;
        return e;
    } else {
        return l->first->enterprise;
    }
}

/*
 * Incrementa en 1 nConnections i reordena el primer l'element
 */
int sortFirstNode(List* l) {
    if (isEmpty(l)) {
        return -1;
    }

    Enterprise e = l->first->enterprise;
    deleteFirstNode(l);

    e.nConnections += 1;

    insertNodeSorted(l, e);

    return 0;

}

int updateNode(List* l, Enterprise e) {
    if (isEmpty(l)) {
        return -1;
    }

    Node* aux = searchNode(l, e);
    if (aux != NULL) {
        e.nom = aux->enterprise.nom;
        e.ip = aux->enterprise.ip;

        deleteNode(l, e);
        insertNodeSorted(l, e);
        return 0;
    }

    return -1;


}
