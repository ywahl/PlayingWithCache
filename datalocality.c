#include <stdio.h>

#define size (1 << 12)
int arr[size][size];


int main(int argc, char *argv)
{
  int i, j;
  int closerow = 0;
  if (argc > 1)
    closerow = 1;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (closerow)
	arr[i][j] = 5;
      else
	arr[j][i] = 5;
    }
  }
  return 0;
}
