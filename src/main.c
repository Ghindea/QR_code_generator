#include "header.h"

int main(int argc, char **argv)
{

    // printf("%c\n", file[strlen(file)-2]);
    char format[] = {file[strlen(file)-3], file[strlen(file)-2], file[strlen(file)-1], '\0'};
    if (!argv[1]) {
        char **qr = initMatrix();
        int ok = fill_data(qr);
        if (ok) {
            mask_matrix(qr);

            if (!strcmp(format,"ppm")) make_PPM(qr);
            if (!strcmp(format,"png")) make_PNG(qr);
            if (!strcmp(format,"jpg")) make_JPG(qr);

            system("xdg-open "file);  // TODO: find cross-compatible alternative (if possible)
        }

        free_matrix(qr);
    } else system("code config.h");  // TODO: implement runtime configuration
    
    return 0;
}
