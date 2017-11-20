#ifndef   _SYSTEMFUNC_H_
#define   _SYSTEMFUNC_H_

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "dades.h"
#include "communication.h"

#define NEW_CONN            1

//Variables globals externes
extern int connectionFlag;

void * threadFunc(void * arg);
void * threadPicard(void * arg);
void creaThread();
void alarmSignal();

#endif
