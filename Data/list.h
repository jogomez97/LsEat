//
// Created by Joan Gomez  on 3/12/17.
//

#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data.h"

#define ERROR_PRINT "Error en printar. La llista està buida!\n"

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

List createList();
int insertNode(List* l, Enterprise e);
int insertNodeSorted(List* l, Enterprise e);
int deleteNode(List* l, Enterprise e);
int deleteFirstNode(List* l);
void printList(List* l);
int isEmpty(List* l);
void eraseList(List* l);
Node* searchNode(List* l, Enterprise e);
Node* searchBiggerNode(List* l, Enterprise e);
Enterprise checkFirstElement(List* l);
int sortFirstNode(List* l);

#endif
