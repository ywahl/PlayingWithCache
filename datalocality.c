#include <stdio.h>

#define size (1 << 12)
int arr[size][size];



void loop_local()
{
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++)
      arr[i][j] = 5;
  }  
}


void loop_remote()
{
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++)
      arr[i][j] = 5;
  }  
}



int main(int argc, char *argv)
{
  if (argc > 1)
    loop_local();
  else
    loop_remote();
  return 0;
}
