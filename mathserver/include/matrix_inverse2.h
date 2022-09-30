#ifndef matrix_inverse2
#define matrix_inverse2
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 4096
typedef double matrix[MAX_SIZE][MAX_SIZE];

int	N;		/* matrix size		*/
int	maxnum;		/* max number of element*/
char* Init;		/* matrix init type	*/
int	PRINT;		/* print switch		*/
matrix	A;		/* matrix A		*/

void find_inverse(void);
void Init_Matrix(void);
void Print_Matrix(matrix M, char name[]);
void Init_Default(void);
int Read_Options(int, char**);
int matStart (int, char**);

#endif