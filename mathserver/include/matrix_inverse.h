#ifndef matrix_inverse
#define matrix_inverse
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 4096

typedef double matrix[MAX_SIZE][MAX_SIZE];

int	N;		/* matrix size		*/
int	maxnum;		/* max number of element*/
char* Init;		/* matrix init type	*/
int	PRINT;		/* print switch		*/
matrix	A;		/* matrix A		*/
pthread_t *children;

void find_inverse(void*);
void Init_Matrix(void);
void Print_Matrix(matrix M, char name[]);
void Init_Default(void);
int Read_Options(int, char**);
int start_mat (int, char**);

#endif