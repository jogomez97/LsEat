#include "communication.h"

int connectPicard(Data d) {
    int sockfd;
    int clientfd;
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
    s_addr.sin_port = htons(d.portPicard);

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
            read(clientfd, &trama.length, sizeof(trama.length));
            trama.data = (char*) malloc(sizeof(char) * trama.length);
            if (trama.data == NULL) {
                return -1;
            }
            read(clientfd, &trama.data, sizeof(char) * trama.length);


            char buffer[500];

            sprintf(buffer, "%c&%s&%d&%s\n", trama.type, trama.header, trama.length, trama.data);
            write(1, buffer, strlen(buffer));
            switch (trama.type) {
                case 0x01:
                    write(1, "WE IN BOYZ", strlen("WE IN BOYZ"));
                    break;
                default:
                    write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                    break;
            }
        }
        return 0;
    }
}

int connectEnterprise(Data d) {
    int sockfd;
    int clientfd;
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
    s_addr.sin_port = htons(d.portEnterprise);

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

    //De moment 1 connexió que sino despres t'has d'esperat per fer el bind
    //while (1) {
        write(1, WAIT_CONNECT, strlen(WAIT_CONNECT));
        clientfd = accept(sockfd, (struct sockaddr*) &s_addr, &len);
        if (clientfd < 0) {
            write(1, ERROR_ACCEPT, strlen(ERROR_ACCEPT));
            return -1;
        } else {
            write(1, CONNECTED, strlen(CONNECTED));


            read(clientfd, &trama.type, sizeof(trama.type));
            read(clientfd, &trama.header, sizeof(trama.header));
            read(clientfd, &trama.length, sizeof(trama.length));
            trama.data = (char*) malloc(sizeof(char) * trama.length);
            if (trama.data == NULL) {
                return -1;
            }
            read(clientfd, trama.data, sizeof(char) * trama.length);

            //DEBUG
            int length = sizeof(trama.type) + strlen(trama.header)
                    + sizeof(trama.length) + strlen(trama.data);
            char b[10];
            sprintf(b, "LENGTH: %d\n", length);
            write(1, b, strlen(b));
            char buffer2[length];
            sprintf(buffer2, "%X/%s/%d/%s\n", trama.type, trama.header, trama.length, trama.data);
            write(1, buffer2, strlen(buffer2));

            switch (trama.type) {
                case 0x01:
                    write(1, "WE IN BOYZ\n", strlen("WE IN BOYZ\n"));
                    break;
                default:
                    write(1, ERROR_TRAMA, strlen(ERROR_TRAMA));
                    break;
            }
            close(clientfd);
            close(sockfd);
        }
    //}
    return 0;
}
