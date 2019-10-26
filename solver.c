
#include <stdio.h>
#include <string.h>
#include "sudokus.h"

int invalid_sudoku = 0;
int comparison_sudoku[9][9];

int spaces[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
int space_xs[9] = {0, 0, 0, 1, 1, 1, 2, 2, 2};
int space_ys[9] = {0, 1, 2, 0, 1, 2, 0, 1, 2};

int  check(int in[9][9]);
void solve(int in[9][9]);
void print(int in[9][9]);

int main() {
	int in[9][9];
	memcpy(in, sud4, sizeof(int) * 81);
	memcpy(comparison_sudoku, in, sizeof(int) * 81);
	solve(in);
	print(in);
  return 0;
}

int check(int in [9][9]) {
  return 1;
}

void white() {
  printf("\033[0m");
}

void red() {
  printf("\033[1;31m");
}

void blue() {
  printf("\033[0;34m");
}

void print_num(int in[9][9], int row, int col) {
  int num = in[row][col];
  if (num == comparison_sudoku[row][col]) {
    blue();
  } else {
    red();
  }
  if (num == 0) {
    printf(" ");
  } else {
    printf("%d", num);
  }
}

void print(int in[9][9]) {
  for (int section_row = 0; section_row < 3; section_row++) {
    white();
    puts(" +-------+-------+-------+");
    for (int space_row = 0; space_row < 3; space_row++) {
      int row = 3 * section_row + space_row;
      for (int section_col = 0; section_col < 3; section_col++) {
        white();
        printf(" | ");
        print_num(in, row, section_col * 3);
        printf(" ");
        print_num(in, row, section_col * 3 + 1);
        printf(" ");
        print_num(in, row, section_col * 3 + 2);
      }
      white();
      printf(" |\n");
    }
  }
  puts(" +-------+-------+-------+\n");
}

int is_free(int in[9][9], int row, int col) {
  return in[row][col] == 0;
}

int is_suitable(int in[9][9], int row, int col, int num) {
  if (!is_free(in, row, col)) {
    return 0;
  }
  for (int i = 0; i < 9; i++) {
    if(in[row][i] == num) {
      return 0;
    }
    if(in[i][col] == num) {
      return 0;
    }
  }

  int space_row;
  int space_col;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (row == spaces[i][j]) {
        space_row = i;
      }
      if (col == spaces[i][j]) {
        space_col = i;
      }
    }
  }

  int test_row, test_col;
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      test_row = spaces[space_row][row];
      test_col = spaces[space_col][col];
      if (in[test_row][test_col] == num) {
        return 0;
      }
    }
  }
  return 1;
}

int is_solved(int in[9][9]) {
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      if (in[row][col] == 0) {
        return 0;
      }
    }
  }
  return 1;
}

void search_space_wise(int in[9][9], int num, int index);
void search_row_wise  (int in[9][9], int num, int index);
void search_col_wise  (int in[9][9], int num, int index);

void guessing(int in[9][9]);

void solve(int in[9][9]) {
  int count = 0;
  int previous[9][9];
  while(!is_solved(in)) {
    memcpy(previous, in, sizeof(int) * 81);
    count++;
    for (int current = 1; current <= 9; current++) {
      for (int index = 0; index < 9; index++) {
//         if (count > 100 && current == 2 && index == 0) {
//           puts("halt");
//         }
        search_space_wise(in, current, index);
        if (invalid_sudoku == 1) {
          invalid_sudoku = 0;
          return;
        }
        search_row_wise(in, current, index);
        if (invalid_sudoku == 1) {
          invalid_sudoku = 0;
          return;
        }
        search_col_wise(in, current, index);
        if (invalid_sudoku == 1) {
          invalid_sudoku = 0;
          return;
        }
      }
    }
    if (memcmp(in, previous, sizeof(int) * 81) == 0) {
      guessing(in);
      break;
    }
  }
}

void search_space_wise(int in[9][9], int num, int index) {
  int space_x = space_xs[index];
  int space_y = space_ys[index];
  int suitable_row = -1;
  int suitable_col = -1;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int row = spaces[space_y][i];
      int col = spaces[space_x][j];
      int field_num = in[row][col];
      if (field_num == num) {
        return;
      }
      if (is_free(in, row, col)) {
        if (is_suitable(in, row, col, num)) {
          if (suitable_row == -1 && suitable_col == -1) {
            suitable_row = row;
            suitable_col = col;
          } else {
            return;
          }
        }
      }
    }
  }
  if (suitable_row != -1 && suitable_col != -1) {
    in[suitable_row][suitable_col] = num;
  } else {
    invalid_sudoku = 1;
  }
}



void search_row_wise (int in[9][9], int num, int index) {
  int suitable_row = -1;
  int suitable_col = -1;
  for (int col = 0; col < 9; col++) {
    int row = index;
    int field_num = in[row][col];
    if (field_num == num) {
      return;
    }
    if (is_free(in, row, col)) {
      if (is_suitable(in, row, col, num)) {
        if (suitable_row == -1 && suitable_col == -1) {
          suitable_row = row;
          suitable_col = col;
        } else {
          return;
        }
      }
    }
  }
  if (suitable_row != -1 && suitable_col != -1) {
    in[suitable_row][suitable_col] = num;
  } else {
    invalid_sudoku = 1;
  }
}


void search_col_wise  (int in[9][9], int num, int index) {
  int suitable_row = -1;
  int suitable_col = -1;
  for (int row = 0; row < 9; row++) {
    int col = index;
    int field_num = in[row][col];
    if (field_num == num) {
      return;
    }
    if (is_free(in, row, col)) {
      if (is_suitable(in, row, col, num)) {
        if (suitable_row == -1 && suitable_col == -1) {
          suitable_row = row;
          suitable_col = col;
        } else {
          return;
        }
      }
    }
  }
  if (suitable_row != -1 && suitable_col != -1) {
    in[suitable_row][suitable_col] = num;
  } else {
    invalid_sudoku = 1;
  }
}

void guessing(int in[9][9]) {
  // TODO: how can the program go back to a previous guessing?
  int guessing_row = -1;
  int guessing_col = -1;
  int guessings[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      if (is_free(in, row, col)) {
        for (int current = 1; current <= 9; current++) {
          if (is_suitable(in, row, col, current)) {
            guessing_row = row;
            guessing_col = col;
            guessings[current - 1] = 1;
          }
        }
        if (guessing_row != -1 && guessing_col != -1) {
          row = 1000;
          col = 1000;
        }
      }
    }
  }
  int temp[9][9];
  for (int current = 1; current <= 9; current++) {
    if (guessings[current - 1]) {
      memcpy(temp, in, sizeof(int) * 81);
      in[guessing_row][guessing_col] = current;
      solve(in);
      if (is_solved(in)) {
        break;
      } else {
        memcpy(in, temp, sizeof(int) * 81);
      }
    }
  }
}


