#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const int SIDE = 8;
const int QUARTER = SIDE * 2;

/*int matrix[][SIDE] = {
   { 1,  2,  3,  4, 13,  9,  5,  1},
   { 5,  6,  7,  8, 14, 10,  6,  2},
   { 9, 10, 11, 12, 15, 11,  7,  3},
   {13, 14, 15, 16, 16, 12,  8,  4},
   { 4,  8, 12, 16, 16, 15, 14, 13},
   { 3,  7, 11, 15, 12, 11, 10,  9},
   { 2,  6, 10, 14,  8,  7,  6,  5},
   { 1,  5,  9, 13,  4,  3,  2,  1}
};*/

int cells[][2] = {
   { 5, 6},
   { 0, 5},
   { 2, 3},
   { 0, 6},
   { 4, 3},
   { 3, 2},
   { 7, 0},
   { 1, 7},
   { 3, 6},
   { 2, 2},
   { 2, 6},
   { 4, 6},
   { 7, 3},
   { 0, 3},
   { 7, 5},
   { 6, 1}
};

void print_matrix(const int (*arr)[SIDE]) {
   for (int i = 0; i < SIDE; i++) {
      for (int j = 0; j < SIDE; j++) {
         printf("%2d ", arr[i][j]);
      } 
      putchar('\n');
   }
}

void init_grid(const int (*cell)[2], int (*grid)[SIDE]) {
   for (int i = 0; i < QUARTER; i++) {
      int row = cell[i][0];
      int col = cell[i][1];
      grid[row][col] = 1;
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

void fill_side(const char *message, int *count, 
        int (*grid)[SIDE], char (*encrypted)[SIDE]) {
   srand(time(0));
   const char *ptr = &message[0];
   const size_t len = strlen(ptr);
   printf("[%2zu] \"%s\"\n", strlen(ptr + *count), ptr + *count);

   for (int i = 0; i < SIDE; i++) {
      for (int j = 0; j < SIDE; j++) { 
         if (grid[i][j] == 1) {
            while ( (ispunct(message[*count]) || 
                       isspace(message[*count])) && *count < len) {
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
   int grid[SIDE][SIDE] = {0};
   init_grid(cells, grid);
   print_matrix(grid);
   //int_grid(grid);
   int count = 0; // counter of symbols in message 
   for (int side = 1; side <= 4; side++) {
      fill_side(message, &count, grid, encrypted);
      print_symb(encrypted);
      rotate_clockwise(grid);
   }
}


void decryptor(char *readme, const char (*encrypted)[SIDE]) {
   int grid[SIDE][SIDE] = {0};
   int count = 0;
   init_grid(cells, grid);
   for (int side = 1; side <= 4; side++) {
      printf("Side #%d : ", side);
      for (int i = 0; i < SIDE; i++) {
         for (int j = 0; j < SIDE; j++) { 
            if (grid[i][j] == 1) {
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

int main(void) {
   char encrypted[SIDE][SIDE] = {0};

   //const char *message = "Hey dude, do not let me down. Take a bad code and make it better.";
   const char *message = "Hey dude, don't let me down. Take a bad soft and make it better. "
                        "Remember to let it under your skin. Then you'll begin to make it better.";
   
   encryptor(message, encrypted);
   putchar('\n');
   char readme[SIDE*SIDE+1] = {0};
   decryptor(readme, encrypted);
   printf("readme=%s\n", readme);

   return 0;
}
