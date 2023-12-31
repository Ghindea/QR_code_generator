#include "header.h"

char len_bit_no() {         // determines len encoding length based on data type
    switch (data_type)
    {
        case 1: // numeric
            if (version < 10) return 10;
                else if (version < 27) return 12;
                        else return 14;
            break;
        case 2: // alphanumeric
            if (version < 10) return 9;
                else if (version < 27) return 11;
                        else return 13;
            break;
        case 3: // byte
            if (version < 10) return 8;
                else return 16;
            break;
        case 4: // kanji
            if (version < 10) return 8;
                else if (version < 27) return 10;
                        else return 12;
            break;
        default:
        break;
    }
}
unsigned int load_capacity_data(FILE *in) {
    int x = 0;
    for (int i = 1; i < version; i++) {
        fscanf(in, "%d", &x);   // L
        fscanf(in, "%d", &x);   // M
        fscanf(in, "%d", &x);   // Q
        fscanf(in, "%d", &x);   // H
    }
    switch (error_correction_level) {
        case 0:
            fscanf(in, "%d", &x);   // L
            fscanf(in, "%d", &x);   // M
            break;
        case 1:
            fscanf(in, "%d", &x);   // L
            break;
        case 2:
            fscanf(in, "%d", &x);   // L
            fscanf(in, "%d", &x);   // M
            fscanf(in, "%d", &x);   // Q
            fscanf(in, "%d", &x);   // H
            break;
        case 3:
            fscanf(in, "%d", &x);   // L
            fscanf(in, "%d", &x);   // M
            fscanf(in, "%d", &x);   // Q
            break;
        default:
            break;
    }
    return x;
}
int available(char **qr, unsigned char x, unsigned char y) {        // checks if module coordonates are available to place data
    unsigned char aux = size;
    if (x < 0 || x >= aux || y < 0 || y >=aux) return 2;    // matrix limits
    if (x > -1 && x < 9 && y >-1 && y < 9) return 2;        // find pttrn up left
    if (x > size-9 && x < size && y > -1 && y < 9) return 2;// find pttrn dwn left
    if (x > -1 && x < 9 && y > size-9 && y < size) return 2;// find pttrn up right
    if (version >= 7) {
        if (x > -1 && x < 6 && y > size - 12 && y < size - 8) return 2; // special format pttrn up right 
        if (x > size - 12 && x < size - 8 && y > -1 && y < 6) return 2; // special format pttrn up right 
    }
    if (qr[x][y]) return 0;                                 // format pttrn + alignment pttrn
    if (y == 6 || x == 6) return 0;                         // timing pttrn

    return 1;
}
void position_up(int *prev, unsigned char *x, unsigned char *y) {
    if ((*prev) % 2 != 0) {
        (*x)--; (*y)++;
    } else (*y)--;
    (*prev)++;
}
void position_down(int *prev, unsigned char *x, unsigned char *y) {
    if ((*prev) % 2 != 0) {
        (*x)++; (*y)++;
    } else (*y)--;
    (*prev)++;
}
void load_codeword(char **qr, _bit_coord_ *bit, int val) {
   
    int i = 7, msk = 1;
    // prev % 2 == 0 -> left 
    // prev % 2 == 1 -> upper-right/down-right
    while (i >= 0) {
        if (bit->x < 6 && bit->y == size - 12 && version >= 7) {
            while (i >= 0 && bit->x < 6) {
                msk = 1 << i;
                if (msk & val) qr[bit->x][bit->y] = 1;
                i--; bit->x++;
            }
            if (bit->x == 6) {
                bit->x = 7; bit->y++; bit->type = 2; bit->prev = 0;
            }
        } else {
            if (bit->type == 1 && available(qr, bit->x, bit->y) == 2) {
                (bit->x)++; bit->y -= 2; bit->type = 2;
            }
            if (bit->type == 2 && available(qr, bit->x, bit->y) == 2) {
                bit->x--; bit->y -= 2; bit->type = 1;
            }
            if (available(qr, bit->x, bit->y) == 1){
                msk = 1 << i;
                if (msk & val) qr[bit->x][bit->y] = 1;
                i--;
            } 
            if (bit->x == size - 1 && bit->y == 9) {
                bit->x = size - 9; bit->y--; bit->type = 1; 
                bit->prev = 0;
            } else  if (bit->x == 9 && bit->y == 7) {
                bit->y -= 2; bit->type = 2; bit->prev = 0;
            } else  if (bit->x == 7 && bit->y == size - 10 && version >= 7) {
                bit->x = 0; bit->y -= 2; bit->type = 2;
            } else  if (bit->type == 1) position_up(&bit->prev, &bit->x, &bit->y);  // type == 1 -> up
                else position_down(&bit->prev, &bit->x, &bit->y);                   // type == 2 -> down      
        }
    }
}

/* 
*   ===============     ===============     ======= ...
*   7 6 5 4 3 2 1 0		7 6 5 4 3 2 1 0		7 6 5 4 ...		
*   0 1 0 0 0 0 0 0		1 0 1 1 0 0 1 0 	1 0 1 1 ... 
*   ======= =================== =================== ===...
*   first 4 		next 8 bits	 next 8*(len-1) bits
*	bits encode		encode the		encode the string
*	the data type 	string len
*/
uchar * data_codewords(char *msg_in, unsigned codewords) {

    uchar * data = (uchar *) calloc(codewords + 1, sizeof(uchar));
    int _mask = 1;
    data[0] = 1 << (data_type + 3);     // data_type 
	int byte = 0, len = strlen(msg_in) - 1, bit = 3;
	for (int i = len_bit_no()-1; i >= 0; i--) { // len
        if (_is_set(&len, i, 1)) {
            data[byte] |= 1 << bit;
        } else {
            data[byte] &= ~(1 << bit);
        }
        bit--;
        if (bit < 0) {bit = 7; byte++;}
    }
    int k = 4 + len_bit_no();
    int new = k % 8, old = 0;
    int byte_old = len - 1, byte_new = byte_old + 1  + (k/8);
    for (int i = 1; i <= (len)*8; i++) {
        if (_is_set(&msg_in[byte_old], old, 0)) {
            data[byte_new] |= 1 << new;     // set bit
        } else {
            data[byte_new] &= ~(1 << new);   // unset bit
        }
        old++; new++;
        if (new > 7) {new %= 8; byte_new--;}
        if (old > 7) {old %= 8; byte_old--;}
    }
    byte = 236;
    for (int i = strlen(msg_in) + k/8;  i < codewords; i++) {
        data[i] = byte;
        if (byte % 2 == 0) byte = 17;
            else byte = 236;
    }

	return data;
}
void debug(_groups_ *segments) {
    printf("\n\nRESULTS\n=====================================================\n");
        for (int i = 0; i < segments->G1; i++) {
            printf("block %d - %d codewords:    ", i, segments->B1);
            for (int j = 0; j < segments->B1; j++) {
                printf("%d,", segments->data_blocks[i][j]);
            }printf("\n");
        }
        for (int i = 0; i < segments->G2; i++) {
            printf("block %d - %d codewords:    ", segments->G1 + i, segments->B2);
            for (int j = 0; j < segments->B2; j++) {
                printf("%d,", segments->data_blocks[segments->G1 + i][j]);
            }printf("\n");
        }
        printf("\n=====================================================\n");
        for (int i = 0; i < segments->G1 + segments->G2; i++) {
            printf("block %d - %d ec_codewords: ", i, segments->EC);
            for (int j = 0; j < segments->EC; j++) {
                printf("%d,", segments->ec_blocks[i][j]);
            }
            printf("\n");
        }
        printf("\n=====================================================\n");
        printf("total codewords: %d\n\n\n", segments->G1 * segments->B1 + segments->G2 * segments->B2 + segments->EC);
        
        
        printf("INTERLEAVED CODEWORDS\n=====================================================\n");
}
/* This function passes ownership of the allocated array */
int* convert_to_INT(const uchar* v, unsigned v_size) {
	int * w = (int *) calloc(v_size,sizeof(int));
	for (int i = 0; i < v_size; i++) {
		w[i] = (int) v[i];
	}
	return w;
}

void interleave(char **qr, _groups_ *seg) {
    _bit_coord_ bit = {.x = size - 1, .y = size-1, .type = 1, .prev = 0}; 
    int min; 
    if (!seg->B2) {
        min = seg->B1;
    } else {min = seg->B1 < seg->B2 ? seg->B1:seg->B2;}

    for (int j = 0; j < min; j++) {
        for (int i = 0; i < seg->G1 + seg->G2; i++) {
            load_codeword(qr, &bit, seg->data_blocks[i][j]);
        }
    }
    for (int i = 0; i < seg->G2; i++) {
        load_codeword(qr, &bit, seg->data_blocks[seg->G1 + i][seg->B2 - 1]);
    }
    for (int j = 0; j < seg->EC; j++) {
        for (int i = 0; i < seg->G1+seg->G2; i++) {
            load_codeword(qr, &bit, seg->ec_blocks[i][j]);
        }
    }
}
void encode_blocks(_groups_ *seg) {
    int cont = 0; int *tmp = (int*)calloc(seg->B1 > seg->B2 ? seg->B1:seg->B2, sizeof(int));
    for (int i = 0; i < seg->G1; i++) {
        memcpy(tmp, seg->data_blocks[cont], (seg->B1) * sizeof(int));
        invert_int_array(tmp, seg->B1-1);
        polynomial decoded = poly_init(seg->B1-1, tmp);
        polynomial encoded = reed_solomon(decoded, seg->EC);
        for (int j = 0; j < seg->EC; j++) {
            seg->ec_blocks[cont][j] = encoded.coef[seg->EC - j - 1];
        }
        cont++;

        free_polynomial(&encoded);
        free_polynomial(&decoded);
    }
    for (int i = 0; i < seg->G2; i++) {
         memcpy(tmp, seg->data_blocks[cont], (seg->B2) * sizeof(int));
        invert_int_array(tmp, seg->B2-1);
        polynomial decoded = poly_init(seg->B2-1, tmp);
        polynomial encoded = reed_solomon(decoded, seg->EC);
        for (int j = 0; j < seg->EC; j++) {
            seg->ec_blocks[cont][j] = encoded.coef[seg->EC - j - 1];
        }
        cont++;

        free_polynomial(&encoded);
        free_polynomial(&decoded);
    }

    free(tmp);
}
void separate_blocks(int *data_string, _groups_ *seg) {
    int cont = 0, block = 0;
    for (int  i = 0; i < seg->G1; i++) {    // for each block from group 1
        for (int j = 0; j < seg->B1; j++) { // for each of its codewords
            seg->data_blocks[block][j] = data_string[cont++];
        }
        block++;
    }
    for (int  i = 0; i < seg->G2; i++) {    // for each block from group 1
        for (int j = 0; j < seg->B2; j++) { // for each of its codewords
            seg->data_blocks[block][j] = data_string[cont++];
        }
        block++;
    }
}
_groups_ * alloc_groups(int ec_no) {
    _groups_ *seg = malloc(sizeof(_groups_));

    FILE *ain = fopen("utils/groups/blocks_no_G1.txt","r");
    FILE *bin = fopen("utils/groups/codewords_no_in_G1_blocks.txt","r");
    FILE *ein = fopen("utils/groups/blocks_no_G2.txt","r");
    FILE *din = fopen("utils/groups/codewords_no_in_G2_blocks.txt","r");

    seg->EC = ec_no;
    seg->G1 = load_capacity_data(ain);   // number of blocks in G1
    seg->B1 = load_capacity_data(bin);   // number of codewords for each of G1's block
    seg->G2 = load_capacity_data(ein);   // number of blocks in G2
    seg->B2 = load_capacity_data(din);   // number of codewords for each of G2's block
    
    /* alloc space for data codewords*/
    int max = seg->B1 > seg->B2 ? seg->B1:seg->B2;
    seg->data_blocks = (int **) calloc(seg->G1 + seg->G2, sizeof(int *));
    for (int i = 0; i < seg->G1 + seg->G2; i++) {
        seg->data_blocks[i] = (int *) calloc(max, sizeof(int));
    }

    /* alloc space for ec codewords*/
    seg->ec_blocks   = (int **) calloc(seg->G1 + seg->G2, sizeof(int *));
    for (int i = 0; i < seg->G1 + seg->G2; i++) {
        seg->ec_blocks[i] = (int *) calloc(ec_no, sizeof(int));
    }

    /* free memory */
    fclose(ain); fclose(bin); fclose(ein); fclose(din);

    return seg;
}
int fill_data(char **matrix) {
    // FILE *in  = fopen(mode_path, "r");
    // FILE *fin = fopen("utils/data_codewords_capacity.txt", "r");
    // FILE *cin = fopen("utils/ec_codewords_capacity.txt","r");
    
    unsigned capacity    = MODE[config];
    unsigned codewords   = DATA_CODEWORDS[config];
    unsigned ECcodewords = EC_CODEWORDS[config];
    _groups_ * segments = alloc_groups(ECcodewords);

 
    // fclose(in); fclose(fin); fclose(cin);
    printf("Insert text: ");
    char *msg_in = (char *)calloc(MAXLEN, sizeof(char));
    fgets(msg_in, MAXLEN, stdin);

    if (strlen(msg_in)-1 <= capacity) {
        
        uchar * data_string = data_codewords(msg_in, codewords);
        int * int_data_string = convert_to_INT(data_string, codewords);

        separate_blocks(int_data_string, segments);
        encode_blocks(segments);
        // debug(segments);
        interleave(matrix, segments);

        free(msg_in);
        free(data_string);
        free(int_data_string);
        free_groups(segments);

        return 1;

    } else {
        free(msg_in);
        error(0);
        return 0;
    }
}
// dg