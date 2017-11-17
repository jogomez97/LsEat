#ifndef   _COMMUNICATION_H_
#define   _COMMUNICATION_H_

#define ERROR_SOCK  "Error en crear el socket!\n"
#define ERROR_CONNECT "Error de connexion con el servidor.\n"
#define ERROR_ACCEPT "Error en acceptar!\n"
#define ERROR_BIND "Error en fer el bind!\n"
#define ERROR_TRAMA "Error en la trama!\n"
#define NCONN 10

typedef struct {
  char     type;
  char[10] header;
  int      lenght;
  char*    data;
} Trama;

#endif
