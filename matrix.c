#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct T
{
  int row;
  int col;
  int k;
  int **mat_1;
  int **mat_2;
  int **mat_result;
} T;
int number_of_threads;

void get_input(int **, int *, int *);
int **allocate_matrix(int *, int *);
void matrix_mul(int **, int **, int *, int *, int *);
void get_dimensions(int *, int *);
void *element_by_element(void *);
void print_matrix(int **, int, int);
int main()
{
  number_of_threads=0;
  int number_of_rows, number_of_columns, number_of_rows_2, number_of_columns_2, i, j;
  get_dimensions(&number_of_rows, &number_of_columns);
  int **matrix_1 = allocate_matrix(&number_of_rows, &number_of_columns);
  get_input(matrix_1, &number_of_rows, &number_of_columns);
  get_dimensions(&number_of_rows_2, &number_of_columns_2);
  int **matrix_2 = allocate_matrix(&number_of_rows_2, &number_of_columns_2);
  get_input(matrix_2, &number_of_rows_2, &number_of_columns_2);
  matrix_mul(matrix_1, matrix_2, &number_of_rows, &number_of_columns_2, &number_of_columns);
  int **matrix_result = allocate_matrix(&number_of_rows, &number_of_columns_2);
  pthread_t threads[number_of_rows][number_of_columns_2];
  printf("------------------------------------------------------------------\n");
  for (i = 0; i < number_of_rows; i++)
  {
    for (j = 0; j < number_of_columns_2; j++)
    {
      struct T *data = (struct T *)malloc(sizeof(struct T));
      data->row = i;
      printf("%d%d\n",i,j);
      data->col = j;
      data->k = number_of_columns;
      data->mat_1 = matrix_1;
      data->mat_2 = matrix_2;
      data->mat_result = matrix_result;
      pthread_create(&threads[i][j], NULL, element_by_element, data);
      
    }
  }
  for (i = 0; i < number_of_rows; i++)
  {
    for (j = 0; j < number_of_columns_2; j++)
    {
      pthread_join(threads[i][j], NULL);
    }
  }
  print_matrix(matrix_result, number_of_rows, number_of_columns_2);

  return 0;
}

void *element_by_element(void *data)
{
  number_of_threads++;
  int row = ((struct T *)data)->row;
  int col = ((struct T *)data)->col;
  int **mat_1 = ((struct T *)data)->mat_1;
  int **mat_2 = ((struct T *)data)->mat_2;
  int **mat_result = ((struct T *)data)->mat_result;
  int i, j, accumulator, x;
  accumulator = 0;
  for (x = 0; x < ((struct T *)data)->k; x++){
    accumulator = accumulator + mat_1[row][x] * mat_2[x][col];
  }
  mat_result[row][col] = accumulator;
  
}

void get_input(int **matrix, int *rows, int *columns)
{
  int i, j;
  for (i = 0; i < *rows; i++)
  {
    printf("Enter elements in row number %d\n", i + 1);
    for (j = 0; j < *columns; j++)
    {
      scanf("%d", &matrix[i][j]);
    }
  }
}
void get_dimensions(int *number_of_rows, int *number_of_columns)
{
  int x, y;
  printf("Enter number of rows --> ");
  scanf("%i", &x);
  *number_of_rows = x;
  printf("Enter number of columns --> ");
  scanf("%i", &y);
  *number_of_columns = y;
}

int **allocate_matrix(int *number_of_rows, int *number_of_columns)
{
  int i;

  int **matrix = (int **)malloc(sizeof *matrix * *number_of_rows);
  if (matrix)
  {
    for (i = 0; i < *number_of_rows; i++)
    {
      matrix[i] = (int *)malloc(sizeof *matrix[i] * *number_of_columns);
    }
  }
  else
  {
    printf("Error allocating matrix!!\n");
  }
  return matrix;
}

void print_matrix(int **matrix, int r, int c)
{
  int i, j;
  for (i = 0; i < r; i++)
  {
    for (j = 0; j < c; j++)
    {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

void matrix_mul(int **matrix_1, int **matrix_2, int *number_of_rows_1, int *number_of_columns_2, int *number_of_columns_1)
{
  int **matrix = allocate_matrix(number_of_rows_1, number_of_columns_2);
  int i, j, k;
  for (i = 0; i < *number_of_rows_1; ++i)
    for (j = 0; j < *number_of_columns_2; ++j)
      for (k = 0; k < *number_of_columns_1; ++k)
      {
        matrix[i][j] += matrix_1[i][k] * matrix_2[k][j];
      }
  print_matrix(matrix, *number_of_rows_1, *number_of_columns_2);
}
