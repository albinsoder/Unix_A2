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
  /* The A inverse matrix, which will be initialized to the identity matrix */

/* forward declarations */
void find_inverse(void);
void Init_Matrix(void);
void Print_Matrix(matrix M, char name[]);
void Init_Default(void);
// void Read_Option(char buff[1024]);
int Read_Option(int, char**);
// char findNext2(int pos, char* buff);
void matStart(int, char**);

#endif