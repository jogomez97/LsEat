#ifndef   _IO_H_
#define   _IO_H_

int readFile(char* path, Data* data);
char* readUntil(int fd, char cFi);
void printConnection(char* name);
void printDisconnection(char* name);

#endif
