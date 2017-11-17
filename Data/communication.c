#include "communication.h"

int connectPicard (Data d) {
  int sockfd;
  int clientfd;
  int eotx = 0;
  Trama trama;

  /* Obrir servidor */
  //Creació socket
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0) {
    write(1, ERROR_SOCK, strlen(ERROR_SOCK));
    return -1;
  }

  //Bind
  struct sockaddr_in s_addr;
  memset(&s_addr, 0, sizeof(s_addr));
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(port);

  int error = inet_aton(d.ip, &s_addr.sin_addr);
  if (error < 0) {
    write(1, ERROR_CONNECT, strlen(ERROR_CONNECT));
    return -1;
  };

  if (bind(sockfd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
    write(1, ERROR_BIND, strlen(ERROR_BIND));
    return -1;
  }

  //Listen
  listen(sockfd, NCONN);

  //Accept
  socklen_t len = sizeof(s_addr);

  while (1) {
    clientfd = accept(sockfd, (struct sockaddr*) &s_addr, &len);
    if (clientfd < 0) {
      write(1, ERROR_ACCEPT, strlen(ERROR_ACCEPT));
    } else {

      read(clientfd, &trama.type, sizeof(trama.type));
      read(clientfd, &trama.header, sizeof(trama.header));
      read(clientfd, &trama.lenght, sizeof(trama.lenght));
      trama.data = (char*) malloc(sizeof(char) * trama.lenght);
      if (trama.data == NULL) {
        return -1;
      }
      read(clientfd, &trama.data, sizeof(char) * trama.lenght));

      switch (trama.type) {
        case 0x01:
          write(1, "WE IN BOYZ", strlen("WE IN BOYZ"));
          break;
        default:
          write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
          break;
      }
  }
  return 0
}
