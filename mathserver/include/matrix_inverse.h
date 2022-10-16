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
pthread_barrier_t barrier;
FILE *fp;

// Th = Thread holder, keeps track of thread id and iteration
struct Th {
    int p;
    int i;
    int size;
    int end;
    double pivalue;
};

void find_inverse();
void Init_Matrix(void);
void Print_Matrix(matrix, char name[]);
void Write_To_File(matrix, int); // Write matrix M to file
void Write_Text_To_File(int, int);
void Init_Default(void);
int Read_Options(int, char**, int);
int start_mat (int, char**, int); 
void* help_inverse(void*);

#endif