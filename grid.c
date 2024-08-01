#include <stdio.h>
#include <time.h> 
#include <stdlib.h> 
#include <stdbool.h>

const int SIDE = 8;
const int QUARTER = SIDE * 2;

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
}

void int_matrix(const int (*arr)[SIDE]) {
	printf("int matrix[][%d] = {\n", SIDE);
    for (int i = 0; i < SIDE; i++) {
    	printf("\t{");
      	for (int j = 0; j < SIDE; j++) {
        	printf("%2d", arr[i][j]);
        	if (j < SIDE-1) {
        		printf(", ");
        	} 
      	} 
      	putchar('}');
      	if (i < SIDE-1) {
        	printf(",\n");
        } 
  	}
  	printf("\n};\n");
}

void gen_rand_cells(const int (*arr)[SIDE], int (*cells)[2], 
	       const int length, const int lower, const int upper) {
	const int *ptr = &arr[0][0];
	int uniq[QUARTER] = {0};
	int count = 0;
	time_t t;
	srand((unsigned) time(&t)); 

	while (count < length) { 
		int rand_idx = (rand() % (upper - lower + 1)) + lower;
		bool found = false;
		for (int i = 0; i < length; i++) { 
			if (uniq[i] == *(ptr + rand_idx)) { 
				found = true; 
				break;
			} 
		} 
		if (!found) { 
			uniq[count] = *(ptr + rand_idx); 
			div_t output = div(rand_idx, SIDE);
			cells[count][0] = output.quot;
			cells[count][1] = output.rem;
			count++; 
		} 
	} 
}

void int_cells(const int (*arr)[2]) {
	printf("int cells[][2] = {\n");
    for (int i = 0; i < QUARTER; i++) {
        printf("\t{%2d,%2d}", arr[i][0], arr[i][1]);
        if (i < QUARTER-1) {
        	putchar(',');
        } 
        putchar('\n');
    } 
    printf("};\n");
}

void print_cells(const int (*arr)[SIDE], const int (*cell)[2]) {
    for (int i = 0; i < QUARTER; i++) {
      int row = cell[i][0];
      int col = cell[i][1];
      printf("matrix[%d][%d] = %2d\n", row, col, arr[row][col]);
   }
}

void print_grid(const int (*cell)[2]) {
	int grid[SIDE][SIDE] = {0};
	for (int i = 0; i < QUARTER; i++) {
 		int row = cell[i][0];
      	int col = cell[i][1];
      	grid[row][col] = 1;
	}
	print_matrix(grid);
}

int main(void) {
	int matrix[SIDE][SIDE] = {0};
	int cells[QUARTER][2] = {0};
	
	init_quarters(matrix);
	print_matrix(matrix);
	int_matrix(matrix);
	
	putchar('\n');

	gen_rand_cells(matrix, cells, QUARTER, 0, SIDE*SIDE-1);
	int_cells(cells);
	print_cells(matrix, cells); 

	print_grid(cells);

	return 0;
}
