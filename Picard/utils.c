#include "utils.h"

void stringToUpper(char* string) {
    int i;
    int fi = strlen(string);

    for (i = 0; i < fi; i++) {
        string[i] = toupper(string[i]);
    }

}
