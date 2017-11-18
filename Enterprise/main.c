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
                if (sockfd < 0) {
                    return EXIT_FAILURE;
                } else {

                    //Nova connexió Enterprise->Data
                    Trama trama;
                    int length;

                    memset(&trama, 0, sizeof(trama));

                    //Afegim les dades a trama
                    trama.type = 0x01;
                    strcpy(trama.header, "[ENT_INFO]\0");

                    length = strlen(enterprise.nom) + strlen(enterprise.ipData)
                            + sizeof(enterprise.portData) + 2 * sizeof(char);
                    char buffer[length];
                    sprintf(buffer, "%s&%d&%s", enterprise.nom, enterprise.portData, enterprise.ipData);
                    trama.data = buffer;
                    trama.length = strlen(trama.data);

                    //Enviem Trama
                    length = sizeof(trama.type) + sizeof(trama.header)
                            + sizeof(trama.length) + strlen(trama.data);

                    char b[10];
                    sprintf(b, "LENGTH: %d\n", length);
                    write(1, b, strlen(b));
                    char buffer2[length];


                    printf("%u\n", trama.length );


                    sprintf(buffer2, "%c%s%u%s", trama.type, trama.header, trama.length, trama.data);
                    write(1, buffer2, strlen(buffer2));
                    write(1, "\n", strlen("\n"));
                    write(sockfd, buffer2, strlen(buffer2));

                    close(sockfd);
                }

            }

        }

    }


    return EXIT_SUCCESS;
}
