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

// Th = Thread holder, keeps track of thread id and iteration
struct Th {
    int p;
    int i;
};

void find_inverse();
void* Init_Matrix(void* buf);
void Print_Matrix(matrix M, char name[]);
void Init_Default(void);
int Read_Options(int, char**);
int start_mat (int, char**);
void* help_inverse(void* id);
void* child(void* buf);

#endif