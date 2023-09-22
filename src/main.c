#include "header.h"

int main(int argc, char **argv)
{
    if (!argv[1]) {
        char **qr = initMatrix();  // NOTE: this memory is never freed
        // printMatrix(qr);
        int ok = fill_data(qr);
        if (ok) {
            makeQR(qr);
#ifdef __linux__
            system("xdg-open QR.ppm");   // It's better to remove system calls altogether
#else
            printf("QR code saved in file \"%s\"", file);
#endif
            // printMatrix(qr);
        }
    } else system("code config.h");  // same here
    return 0;
}