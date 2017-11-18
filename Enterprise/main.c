/*∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗
   @Autor: Juan Gómez Gómez - juan.gomez.2015
           Jordi Malé Carbonell - jordi.male.2015
           PORTS: 8180-8189 / 8260-8269
∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗*/

/*  TODO:
*       - Alliberar memoria Enterprise
*       - Alliberar tots els plats
*
*/

// Type 1 byte, header 10 bytes, length 2 bytes, data length bytes


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "io.h"
#include "dades.h"
#include "communication.h"

#define NPARAM          3
#define ERROR_ARG       "Error! No s'ha passat el nombre de paràmetres que pertoca.\n"
#define C_MENU          "Carregat Menu!\n"
#define WAIT_CLIENT     "Esperant clients...\n"
#define ERROR_DATA      "Error en connectar amb Data\n"

int main(int argc, char const *argv[]) {

    if (argc != NPARAM) {
        write(1, ERROR_ARG, strlen(ERROR_ARG));
        return EXIT_FAILURE;
    } else {
        int error;
        Enterprise enterprise;

        error = readConfig((char*)argv[1], &enterprise);

        if (error) {
            return EXIT_FAILURE;
        } else {
            Menu menu;

            printWelcome(enterprise.nom);
            error = readMenu((char*)argv[2], &menu);

            if (error) {
                return EXIT_FAILURE;
            } else {
                write(1, C_MENU, strlen(C_MENU));

                //Connectar-se a Data
                int sockfd = connectaData(enterprise.ipData, enterprise.portData);
                if (sockfd > 0) {
                    return EXIT_FAILURE;
                } else {
                    write(1, "CONNECTED\n", strlen("CONNECTED\n"));

                    //Nova connexió Enterprise->Data
                    Trama trama;

                    trama.type = 0x01;
                    strcpy(trama.header, "[ENT_INF]");


                    //Mirar pq el strlen para on l'espai de Enterprise


                    char str[10];
                    trama.data = strcat(enterprise.nom, "&");
                    write(1, trama.data, sizeof(trama.data));
                    write(1, "\n", 1);
                    sprintf(str, "%d", enterprise.portData);
                    trama.data = strcat(trama.data, str);
                    write(1, trama.data, sizeof(trama.data));
                    write(1, "\n", 1);
                    trama.data = strcat(trama.data, "&");
                    write(1, trama.data, sizeof(trama.data));
                    write(1, "\n", 1);
                    trama.data = strcat(trama.data, enterprise.ipData);
                    write(1, trama.data, 100);
                    write(1, "\n", 1);

                    trama.length = sizeof(trama.data);



                    close(sockfd);
                }

            }

        }

    }


    return EXIT_SUCCESS;
}
