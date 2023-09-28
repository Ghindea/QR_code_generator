#include "header.h"

int main(int argc, char **argv)
{
    if (!argv[1]) {
        char **qr = initMatrix();
        int ok = fill_data(qr);
        if (ok) {
            mask_matrix(qr);
            makeQR(qr);
            system("xdg-open QR.ppm");  // TODO: find cross-compatible alternative (if possible)
        }

        free_matrix(qr);
    } else system("code config.h");  // TODO: implement runtime configuration
    
    return 0;
}
