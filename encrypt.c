#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

const int SIDE = 8;

int grid[][SIDE] = {
	 { 0,  0,  0,  0,  0,  0,  0,  0 },
	 { 0,  0,  0,  0,  0,  0,  0,  0 },
	 { 0,  0,  0,  0,  0,  0,  0,  0 },
	 { 0,  0,  0,  0,  0,  0,  0,  0 },
	 { 0,  0, 12,  0, 16,  0,  0,  0 },
	 { 3,  7, 11, 15,  0,  0,  0,  9 },
	 { 0,  0, 10, 14,  8,  0,  6,  5 },
	 { 0,  0,  0, 13,  4,  0,  2,  1 }
};


void print_matrix(const int (*arr)[SIDE]) {
    for (int i = 0; i < SIDE; i++) {
      	for (int j = 0; j < SIDE; j++) {
        	printf("%2d ", arr[i][j]);
      	} 
        putchar('\n');
    }
}

void print_symb(const char (*arr)[SIDE]) {
    for (int i = 0; i < SIDE; i++) {
      	for (int j = 0; j < SIDE; j++) {
      		printf("%2c ", arr[i][j] ? arr[i][j] : '.');
      	} 
      	putchar('\n');
    }
}


void rotate_clockwise(int (*arr)[SIDE]) {
   for (int i = 0; i < SIDE; i++){
      for (int j = 0; j < SIDE-i; j++) {
         int x = arr[i][j];
         arr[i][j] = arr[SIDE-1-j][SIDE-1-i];
         arr[SIDE-1-j][SIDE-1-i] = x;
      }
   }
   for (int i = 0; i < SIDE/2; i++) {
      for (int j = 0; j < SIDE; j++) {
         int x = arr[i][j];
         arr[i][j] = arr[SIDE-1-i][j];
         arr[SIDE-1-i][j] = x;
      }
   }
}


void fill_side(const char *message, int *count, int (*grid)[SIDE], char (*encrypted)[SIDE]) {
   srand(time(0));
   const char *ptr = &message[0];
   const size_t len = strlen(ptr);
   printf("[%2zu] \"%s\"\n", strlen(ptr + *count), ptr + *count);

   for (int i = 0; i < SIDE; i++) {
      for (int j = 0; j < SIDE; j++) { 
	 if (grid[i][j] > 0) {
	    while ( (ispunct(message[*count]) || isspace(message[*count])) && *count < len) {
		(*count)++;
	    }	
	    if (*count < len) {
	       encrypted[i][j] = tolower(message[*count]);
	    } else {
	       char lower = 'a', upper = 'z';
	       encrypted[i][j] = (rand() % (upper - lower + 1)) + lower;
	    }
	    (*count)++;
	 } 
      }
   }
}


void encryptor(const char *message, char (*encrypted)[SIDE]) {
	print_matrix(grid);
	int count = 0; // counter of symbols in message 
	for (int side = 1; side <= 4; side++) {
		fill_side(message, &count, grid, encrypted);
		print_symb(encrypted);
		rotate_clockwise(grid);
	}
}


void decryptor(char *readme, const char (*encrypted)[SIDE]) {
	int count = 0;
	int grid[][SIDE] = {
		 { 0,  0,  0,  0,  0,  0,  0,  0 },
		 { 0,  0,  0,  0,  0,  0,  0,  0 },
		 { 0,  0,  0,  0,  0,  0,  0,  0 },
		 { 0,  0,  0,  0,  0,  0,  0,  0 },
		 { 0,  0, 12,  0, 16,  0,  0,  0 },
		 { 3,  7, 11, 15,  0,  0,  0,  9 },
		 { 0,  0, 10, 14,  8,  0,  6,  5 },
		 { 0,  0,  0, 13,  4,  0,  2,  1 }
	};
	for (int side = 1; side <= 4; side++) {
		printf("Side #%d : ", side);
		for (int i = 0; i < SIDE; i++) {
			for (int j = 0; j < SIDE; j++) { 
				if (grid[i][j] > 0) {
					putchar(encrypted[i][j]);
					readme[count] = encrypted[i][j];
					count++;
				}
			}
		}
		putchar('\n');
		rotate_clockwise(grid);
	}
}

void symb_matrix(const char (*arr)[SIDE]) {
    printf("const char encrypted[][%d] = {\n", SIDE);
    for (int i = 0; i < SIDE; i++) {
    	printf("\t{");
      	for (int j = 0; j < SIDE; j++) {
        	printf("'%c' ", arr[i][j]);
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


void int_grid(const int (*arr)[SIDE]) {
    printf("const int grid[][%d] = {\n", SIDE);
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

int main(void) {
	char encrypted[SIDE][SIDE] = {0};

	const char *message = "Hey dude, do not let me down. Take a bad code and make it better.";

	encryptor(message, encrypted);
	putchar('\n');

	symb_matrix(encrypted);
	putchar('\n');
	int_grid(grid);
	putchar('\n');

	char readme[SIDE*SIDE+1] = {0};
	decryptor(readme, encrypted);
	printf("readme=%s\n", readme);

	return 0;
}
