#include "header.h"

int main(int argc, char **argv)
{
    if (!argv[1]) {
        char **qr = initMatrix();
        // printMatrix(qr);
        int ok = fill_data(qr);
        if (ok) {
            makeQR(qr);
            system("xdg-open QR.ppm");
        }
    } else system("code config.h");
    
    return 0;
}