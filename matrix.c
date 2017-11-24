#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct T
{
  int row;
  int col;
  int k;
  float **mat_1;
  float **mat_2;
  float **mat_result;
} T;

void get_input(float **, int *, int *, char *);
float **allocate_matrix(int *, int *);
void matrix_mul(float **, float **, int *, int *, int *);
void get_dimensions(int *, int *, char *);
void *element_by_element(void *);
void print_matrix(float **, int, int);
void *row_by_row(void *);
int main()
{
  int number_of_rows, number_of_columns, number_of_rows_2, number_of_columns_2, i, j;
  char filename[30];
  printf("enter first file name  ");
  scanf("%s", filename);
  get_dimensions(&number_of_rows, &number_of_columns, filename);
  float **matrix_1 = allocate_matrix(&number_of_rows, &number_of_columns);
  get_input(matrix_1, &number_of_rows, &number_of_columns, filename);
  printf("enter second file name  ");
  scanf("%s", filename);
  get_dimensions(&number_of_rows_2, &number_of_columns_2, filename);
  float **matrix_2 = allocate_matrix(&number_of_rows_2, &number_of_columns_2);
  get_input(matrix_2, &number_of_rows_2, &number_of_columns_2, filename);
  if (number_of_columns != number_of_rows_2)
  {
    printf("matrix format error!  exiting!...\n");
    return 0;
  }
  matrix_mul(matrix_1, matrix_2, &number_of_rows, &number_of_columns_2, &number_of_columns);
  float **matrix_result = allocate_matrix(&number_of_rows, &number_of_columns_2);
  pthread_t threads[number_of_rows][number_of_columns_2];
  printf("------------------------------------------------------------------\n");
  for (i = 0; i < number_of_rows; i++)
  {
    for (j = 0; j < number_of_columns_2; j++)
    {
      struct T *data = (struct T *)malloc(sizeof(struct T));
      data->row = i;
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
  printf("------------------------------------------------------------------\n");
  pthread_t threads2[number_of_rows];
  float **matrix_result_2 = allocate_matrix(&number_of_rows, &number_of_columns_2);
  for (i = 0; i < number_of_rows; i++)
  {
    struct T *data = (struct T *)malloc(sizeof(struct T));
    data->row = i;
    data->col = number_of_columns_2;
    data->k = number_of_columns;
    data->mat_1 = matrix_1;
    data->mat_2 = matrix_2;
    data->mat_result = matrix_result_2;
    pthread_create(&threads2[i], NULL, row_by_row, data);
  }
  for (j = 0; j < number_of_columns_2; j++)
  {
    pthread_join(threads2[j], NULL);
  }
  print_matrix(matrix_result_2, number_of_rows, number_of_columns_2);
  return 0;
}
void *row_by_row(void *data)
{
  int i, j, x;
  float accumulator;
  j = 0;
  for (i = 0; i < ((struct T *)data)->col; i++)
  {
    accumulator = 0;
    for (x = 0; x < ((struct T *)data)->k; x++)
    {
      accumulator = accumulator + ((struct T *)data)->mat_1[((struct T *)data)->row][x] * ((struct T *)data)->mat_2[x][i];
    }
    ((struct T *)data)->mat_result[((struct T *)data)->row][i] = accumulator;
  }
}
void *element_by_element(void *data)
{
  int i, j, x;
  float accumulator = 0;
  for (x = 0; x < ((struct T *)data)->k; x++)
  {
    accumulator = accumulator + ((struct T *)data)->mat_1[((struct T *)data)->row][x] * ((struct T *)data)->mat_2[x][((struct T *)data)->col];
  }
  ((struct T *)data)->mat_result[((struct T *)data)->row][((struct T *)data)->col] = accumulator;
}
void get_input(float **matrix, int *rows, int *columns, char *file_name)
{
  int i, j;
  int k = 0;
  float temp[*rows * *columns * 10];
  FILE *file = fopen(file_name, "r");
  if (file)
  {
    while (!feof(file))
      fscanf(file, "%f", &temp[k++]);
    k = 0;
    for (i = 0; i < *rows; i++)
      for (j = 0; j < *columns; j++)
        matrix[i][j] = temp[k++];
    fclose(file);
  }
  else
  {
    printf("error reading file\n");
    exit(-1);
  }
}

void get_dimensions(int *number_of_rows, int *number_of_columns, char *file_name)
{
  int flag = 0;
  *number_of_columns = 1;
  *number_of_rows = 1;
  FILE *file = fopen(file_name, "r");
  if (file)
  {
    while (!feof(file))
    {
      char c = fgetc(file);
      if (c == ' ' && flag == 0)
        *number_of_columns = *number_of_columns + 1;
      if (c == '\n')
      {
        *number_of_rows = *number_of_rows + 1;
        flag = 1;
      }
    }

    fclose(file);
  }
  else
  {
    printf("error loading file\n");
    exit(-1);
  }
}

float **allocate_matrix(int *number_of_rows, int *number_of_columns)
{
  int i;

  float **matrix = (float **)malloc(sizeof *matrix * *number_of_rows);
  if (matrix)
  {
    for (i = 0; i < *number_of_rows; i++)
    {
      matrix[i] = (float *)malloc(sizeof *matrix[i] * *number_of_columns);
    }
  }
  else
  {
    printf("Error allocating matrix!!\n");
  }
  return matrix;
}

void print_matrix(float **matrix, int r, int c)
{
  int i, j;
  for (i = 0; i < r; i++)
  {
    for (j = 0; j < c; j++)
    {
      printf("%.2f ", matrix[i][j]);
    }
    printf("\n");
  }
}

void matrix_mul(float **matrix_1, float **matrix_2, int *number_of_rows_1, int *number_of_columns_2, int *number_of_columns_1)
{
  float **matrix = allocate_matrix(number_of_rows_1, number_of_columns_2);
  int i, j, k;
  for (i = 0; i < *number_of_rows_1; ++i)
    for (j = 0; j < *number_of_columns_2; ++j)
      for (k = 0; k < *number_of_columns_1; ++k)
      {
        matrix[i][j] += matrix_1[i][k] * matrix_2[k][j];
      }
  print_matrix(matrix, *number_of_rows_1, *number_of_columns_2);
}
