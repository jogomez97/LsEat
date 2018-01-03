/*******************************************************************************
*
* Practica Sistemes Operatius - LsEat - Package Data
* Curs 2017-2018
*
* @File     data.h
* @Purpose  Inclou les funcions i structs que gestionen i implementen les dades
*            del programa.
* @Author   Jordi Malé Carbonell  (jordi.male.2015)
* @Author   Juan Gómez Gómez  (juan.gomez.2015)
*
********************************************************************************/

#ifndef   _DATA_H_
#define   _DATA_H_

#define _GNU_SOURCE 1

// Llibreries del sistema
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

// Llibreries pròpies
#include "list.h"

// Definició de tipus propis
typedef struct {
    char* ip;
    int portPicard;
    int portEnterprise;
} Data;

// Variables globals externes
extern pthread_mutex_t mtx;

/*******************************************************************************
*
* @Name     gestionaFlota
* @Purpose  Funció afegirà una Enterprise al nostre conjunt
* @Param    In: path    Path del fitxer a llegir
*           Out: -
* @return   Retorna un enter que indica els possibles errors a l'hora de llegir
*           el fitxer
*
*******************************************************************************/
int gestionaFlota(char* data);

/*******************************************************************************
*
* @Name     getEnterprise
* @Purpose  Funció que retornarà una Enterprise per a que Picard s'hi connecti
* @Param    In:  -
*           Out: -
* @return   Retorna el codi de l'Enterprise on s'haurà de connectar Picard
*
*******************************************************************************/
char* getEnterprise();

#endif
