# **QR code generator**
made by *Daniel Ghindea*

###  `SYNOPSIS:`
        ./project [OPTION]
###  `DESCRIPTION:`
        --config
                opens header file "config.h" to edit program parameters.

        - [version]: there are fixed configurations of QR code sizes that range from 1 to 40:
                1: 21x21; can encode up to 17 ASCII characters
                2: 25x25; can encode up to 32 ASCII characters
                3: 29x29; can encode up to 53 ASCII characters
                ...
                40: 177x177; can encode up to 2953 ASCII characters
                (for more information about character capacities go [here](https://www.thonky.com/qr-code-tutorial/character-capacities))
                
        - [mask]: certain patterns in the QR code matrix can make it difficult for QR code scanners to correctly read the code. to counteract this, the QR code specification defines 8 mask patterns:
                0: (i*j) % 2 + (i*j) % 3 == 0
                1: (i/2 + j/3) % 2 == 0
                2: [(i*j) % 3 + i + j] % 2 == 0
                3: [(i*j) % 3 + i*j ] % 2 == 0
                4: i % 2 == 0
                5: (i + j) % 2 == 0
                6: (i + j) % 3 == 0
                7: j % 3 == 0

        - [error_correction]: there are 4 levels of error correction that helps QR code to stay readable even if some pixels can't be recognised by the scanner:
                0: level M - up to 15%
                1: level L - up to 7%
                2: level H - up to 30%
                3: level Q - up to 25% 
                # [needs more research]

        - [data_type]: QR code can hold 4 different types of data:
                1: numeric
                2: alphanumeric
                3: kanji
                4: bytes
        
        - the color of the QR code is determined by the given amount of red, green and blue color. their values range between 0 and 255.

        - [file]: string that defines output file's name.

### `BIBLIOGRAPHY:`
- [https://www.thonky.com/qr-code-tutorial/]
- [https://www.youtube.com/watch?v=142TGhaTMtI&t=95s]
- [https://developers.google.com/chart/infographics/docs/qr_codes]