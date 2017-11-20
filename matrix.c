#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct T {
int i;
int j;
};

void get_input(int**,int*,int*);
int** allocate_matrix(int*,int*);
void matrix_mul(int**,int**,int*,int*,int* );
void get_dimentions(int*,int*);
int main() {
  int number_of_rows,number_of_columns,number_of_rows_2,number_of_columns_2;
  get_dimentions(&number_of_rows,&number_of_columns);
  int** matrix_1 = allocate_matrix(&number_of_rows,&number_of_columns);
  get_input(matrix_1,&number_of_rows,&number_of_columns);
  get_dimentions(&number_of_rows_2,&number_of_columns_2);
  int** matrix_2 = allocate_matrix(&number_of_rows_2,&number_of_columns_2);
  get_input(matrix_2,&number_of_rows_2,&number_of_columns_2);
  matrix_mul(matrix_1,matrix_2,&number_of_rows,&number_of_columns_2,&number_of_columns);
  pthread_t threads[number_of_rows*number_of_columns_2];
  

  return 0;
}

void get_input(int** matrix,int* rows,int* columns) {
  int i,j;
  for (i = 0; i < *rows; i++) {
    printf("Enter elements in row number %d\n",i+1);
    for (j = 0; j < *columns; j++) {
      scanf("%d",&matrix[i][j]);
    }
  }
}
void get_dimentions(int* number_of_rows,int* number_of_columns){
  int x,y;
  printf("Enter number of rows --> " );
  scanf("%i",&x);
  *number_of_rows=x;
  printf("Enter number of columns --> " );
  scanf("%i",&y);
  *number_of_columns =y;
}

int** allocate_matrix(int* number_of_rows,int* number_of_columns){
  int i;
  
  int **matrix = (int**)malloc(sizeof *matrix * *number_of_rows);
  if (matrix)
  {
    for (i = 0; i <*number_of_rows; i++)
    {
        matrix[i] = (int*)malloc(sizeof *matrix[i] * *number_of_columns);
    }
  }
  else{
    printf("Error allocating matrix!!\n" );
  }
  return matrix;
}

void matrix_mul(int** matrix_1,int** matrix_2,int* number_of_rows_1,int* number_of_columns_2,int* number_of_columns_1){
   int** matrix = allocate_matrix(number_of_rows_1,number_of_columns_2);
   int i,j,k;
     for(i=0; i<*number_of_rows_1; ++i)
            for(j=0; j<*number_of_columns_2; ++j)
                for(k=0; k<*number_of_columns_1; ++k)
                {
                    matrix[i][j]+=matrix_1[i][k]*matrix_2[k][j];
                }
    for (i = 0; i < *number_of_rows_1; i++) {
      for (j = 0; j < *number_of_columns_2; j++) {
        printf("%d ",matrix[i][j]);
      }
      printf("\n");
    }

}
