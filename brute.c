#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


const int SIDE = 8;
const int DIGITS = SIDE * 2;
typedef unsigned int UINT;

void init_quarters(int (*arr)[SIDE]) {
	/*
	Quarters:
		1 2
		3 4
	*/
	const int dim = SIDE/2;
	int x = 1;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            arr[i][j] = x;                 // 1 quarter
            arr[j][SIDE-1-i] = x;          // 2 quarter
            arr[SIDE-1-j][i] = x;          // 3 quarter
            arr[SIDE-1-i][SIDE-1-j] = x;   // 4 quarter
            x++;
        }
    }
}


void print_matrix(const int (*arr)[SIDE]) {
    for (int i = 0; i < SIDE; i++) {
      	for (int j = 0; j < SIDE; j++) {
        	printf("%2d ", arr[i][j]);
      	} 
      	putchar('\n');
  	}
  	putchar('\n');
}


void int_to_bin_digit(UINT in, int count, int *out) {
    /* assert: count <= sizeof(int)*CHAR_BIT */
    UINT mask = 1U << (count-1);
    for (int i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}


void print_arr(const int *arr, const int size) {
	for (int i = 0; i < size; i++) {
		printf("%2d ", arr[i]);
	}
	putchar('\n');
}


void get_coords(const int quarter, const int index, int *x, int *y) {
	const div_t t = div(index, SIDE/2);
	const int row = t.quot;
	const int col = t.rem;

	switch (quarter) {
	    case 1:
	      *x = row; 
	      *y = col;
	      break;
	    case 2:
	      *x = col; 
	      *y = SIDE-1-row;
	      break;
	    case 3:
	      *x = SIDE-1-col; 
	      *y = row;
	      break;
	    case 4:
	      *x = SIDE-1-row;  
	      *y = SIDE-1-col;
	      break;
	}
}


void make_grid_quarter(int (*grid)[SIDE], const UINT value, const int quarter) {
	int digit[DIGITS];
	int_to_bin_digit(value, DIGITS, digit);
	for (int i = 0; i < DIGITS; i++) {
		if (digit[i] == 0) {
			continue;
		}
		int x, y;
		get_coords(quarter, i, &x, &y);
		grid[x][y] = 1;
	}
}

const char encrypted[][8] = {
	{'e' , 'a' , 'e' , 'n' , 'd' , 'u' , 'f' , 'k' },
	{'m' , 'a' , 'd' , 'k' , 'e' , 'i' , 'q' , 'e' },
	{'t' , 'o' , 'w' , 'n' , 'b' , 'e' , 't' , 't' },
	{'t' , 'a' , 'k' , 'e' , 'k' , 'r' , 'b' , 'r' },
	{'e' , 'a' , 'h' , 'b' , 'e' , 'x' , 'g' , 'u' },
	{'y' , 'd' , 'u' , 'd' , 'f' , 'm' , 'c' , 'e' },
	{'a' , 'd' , 'd' , 'o' , 'n' , 'g' , 'o' , 't' },
	{'c' , 'o' , 'd' , 'l' , 'e' , 'r' , 't' , 'm' }
};

const int grid[][8] = {
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0, 12,  0, 16,  0,  0,  0},
	{ 3,  7, 11, 15,  0,  0,  0,  9},
	{ 0,  0, 10, 14,  8,  0,  6,  5},
	{ 0,  0,  0, 13,  4,  0,  2,  1}
};


//const char *message = "Hey dude, do not let me down. Take a bad code and make it better.";
const char *message = "heydudedonotletm";

void decryptor(const int (*grid)[SIDE], const char (*encrypted)[SIDE], char *readme) {
	int count = 0;
	for (int i = 0; i < SIDE; i++) {
		for (int j = 0; j < SIDE; j++) { 
			if (grid[i][j] == 1) {
				*(readme + count) = encrypted[i][j];
				count++;
			}
		}
	}
}


void print_grid(const int (*arr)[SIDE], const int (*grid)[SIDE]) {
    for (int i = 0; i < SIDE; i++) {
      	for (int j = 0; j < SIDE; j++) {
      		if (grid[i][j] == 1) {
        		printf("%2d ", arr[i][j]);
        	} else {
        		printf("%2c ", '.');
        	}
      	} 
      	putchar('\n');
  	}
  	putchar('\n');
}

int main(void) {
	const clock_t before = clock();
	int matrix[SIDE][SIDE] = {0};
	init_quarters(matrix);
	
    const UINT top = 65536; // 65536 = 2^(SIDE*2)
    int grid[SIDE][SIDE] = {0};
    const size_t area = sizeof(grid[0][0]) * SIDE * SIDE;
    unsigned long int iter = 0;

	for (UINT i = 0; i < top; i++) {
		UINT uniq_i = i;
		for (UINT j = 0; j < top; j++) {
			if (uniq_i & j) { 
				continue;
			}	
			UINT uniq_j = uniq_i | j;
			printf("[%010lu] %5d %5d\n", iter, i, j);
			for (UINT k = 0; k < top; k++) {
				if (uniq_j & k) { 
					continue;
				}
				UINT uniq_k = uniq_j | k;
				int base[SIDE][SIDE] = {0};
				make_grid_quarter(base, i, 1);
				make_grid_quarter(base, j, 2);
				make_grid_quarter(base, k, 3);
				for (UINT m = 0; m < top; m++) {
					if ((uniq_k ^ m) == 65535) {
						printf ("%5d %5d %5d %5d\n", i, j, k, m);
						memcpy(grid, base, area);
						make_grid_quarter(grid, m, 4);
						print_grid(matrix, grid);

						char readme[DIGITS+1] = {'\0'};
						decryptor(grid, encrypted, readme);
						if (strcmp(readme, message) == 0) {
							printf("Bingo!\n");
							printf("%5d %5d %5d %5d\n", i, j, k, m);
							print_grid(matrix, grid);
							const clock_t difference = clock() - before;
							const int msec = difference * 1000 / CLOCKS_PER_SEC;
							printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
							return 0;
						}
						
    					iter++;
					}
				}
			}
		}
	}
	const clock_t difference = clock() - before;
	const int msec = difference * 1000 / CLOCKS_PER_SEC;
	printf(":( Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
	return 0;
}
