/***************************************************************************
 *
 * Sequential version of Matrix Inverse
 * An adapted version of the code by H�kan Grahn
 ***************************************************************************/

#include "../include/matrix_inverse.h"


/* forward declarations */

matrix I = {0.0};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


int
main(int argc, char** argv)
{
    printf("Matrix Inverse\n");
    int i, timestart, timeend, iter;
    unsigned long id=0;

    Init_Default();		/* Init default values	*/
    Read_Options(argc, argv);	/* Read arguments	*/
    Init_Matrix((void*)id);
    // Init_Matrix(NULL);
    find_inverse();
    // Init_Matrix();		/* Init the matrix	*/
    // find_inverse();

    if (PRINT == 1)
    {
        Print_Matrix(A, "End: Input");
        Print_Matrix(I, "Inversed");
    }
}
void* child(void* buf){
    unsigned long childID = (unsigned long)buf;
    find_inverse(childID);
    return NULL;
}

int
start_mat(int argc, char** argv){
    // printf("Matrix Inverse\n");
    // int i, timestart, timeend, iter;

    // Init_Default();		/* Init default values	*/
    // Read_Options(argc, argv);	/* Read arguments	*/
    // Init_Matrix();		/* Init the matrix	*/
    // find_inverse();

    // if (PRINT == 1)
    // {
    //     //Print_Matrix(A, "End: Input");
    //     Print_Matrix(I, "Inversed");
    // }    
}

void find_inverse()
{
    /* Bringing the matrix A to the identity form */
    children = malloc(N * sizeof(pthread_t));
    int nThreads = 8; // Number of threads to be created
    int rest=0;
    int count = 0;
    int flag = 0;
    if(N < nThreads){
        nThreads=N;
        rest = 0;
    }
    else if(N % 8 != 0){
        rest = N%8;
        flag=1;
    }

    pthread_barrier_init(&barrier, NULL, nThreads); // Initialize barrier
    struct Th *ptr;
    int n = (N-rest);
    double pi;
    for(int i=0; i<N; i++){
        pi = A[i][i];
        for (int id = 0; id < nThreads; id++) { /* Outer loop */
            if(flag && id == nThreads-1){
                ptr = (struct Th*)malloc(sizeof(struct Th));
                ptr->size=-1; //functions as a flag
                ptr->pivalue=pi;
                ptr->p = i;
                ptr->i = id;
                ptr->end = N;
                pthread_create(&(children[id]), NULL, help_inverse, ptr); // Let current thread perform its inversion
            } else{
                ptr = (struct Th*)malloc(sizeof(struct Th));
                ptr->size=n;
                ptr->pivalue=pi;
                ptr->p = i;
                ptr->i = id;
                pthread_create(&(children[id]), NULL, help_inverse, ptr); // Let current thread perform its inversion
            }
        }
        for (int id = 0; id < nThreads+1; id++) { /* Outer loop */
            pthread_join(children[id], NULL); // Collect/join result from the threads
        }
        pthread_barrier_destroy(&barrier);
    }
    free(ptr);
    free(children);
}

void* help_inverse(void* id)
{
    int row, col, start, end, thread_div, n_thread; // 'p' stands for pivot (numbered from 0 to N-1)

    struct Th *t_holder = (struct Th*)id;

    int sizeN= (int)t_holder->size;
    if(sizeN == -1){
        start = t_holder->i;
        end = t_holder->end;
    }
    else if(sizeN < 8){
        thread_div=1;
        start = t_holder->i*thread_div;
        end = (t_holder->i+1)*thread_div;
        // printf("SIZE: %d", sizeN);
    } else{
        thread_div = sizeN/8;
        start = t_holder->i*thread_div;
        end = (t_holder->i+1)*thread_div;
    }

    for (col = start; col < end; col++)
    {
        A[t_holder->p][col] = A[t_holder->p][col] / t_holder->pivalue; /* Division step on A */
        I[t_holder->p][col] = I[t_holder->p][col] / t_holder->pivalue; /* Division step on I */

    }
    pthread_barrier_wait(&barrier);
    assert(A[t_holder->p][t_holder->p] == 1.0);
    double multiplier;

    for (row = start; row < end; row++) 
    {
        multiplier = A[row][t_holder->p];
        if (row != t_holder->p) // Perform elimination on all except the current pivot row 
        {
            for (col = 0; col < N; col++)
            {
                A[row][col] = A[row][col] - A[t_holder->p][col] * multiplier; /* Elimination step on A */
                I[row][col] = I[row][col] - I[t_holder->p][col] * multiplier; /* Elimination step on I */
            }
            assert(A[row][t_holder->p] == 0.0);
        }
    }
    
}

void*
Init_Matrix(void* buf)
{
    int row, col;
    // pthread_mutex_lock(&mutex);
    // Set the diagonal elements of the inverse matrix to 1.0
    // So that you get an identity matrix to begin with
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (row == col)
                I[row][col] = 1.0;
        }
    }

    printf("\nsize      = %dx%d ", N, N);
    printf("\nmaxnum    = %d \n", maxnum);
    printf("Init	  = %s \n", Init);
    printf("Initializing matrix...");

    if (Init[0] == 'r') {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (row == col) /* diagonal dominance */
                    A[row][col] = (double)(rand() % maxnum) + 5.0;
                else
                    A[row][col] = (double)(rand() % maxnum) + 1.0;
            }
        }
    }
    // printf("LENGTH: %ld", strlen(Init));
    if (Init[0] == 'f') {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (row == col) /* diagonal dominance */
                    A[row][col] = 5.0;
                else
                    A[row][col] = 2.0;
            }
        }
    }
    // pthread_mutex_unlock(&mutex);

    printf("done \n\n");
    if (PRINT == 1)
    {
        Print_Matrix(A, "Begin: Input");
        // Print_Matrix(I, "Begin: Inverse");
    }
}

void
Print_Matrix(matrix M, char name[])
{
    int row, col;

    printf("%s Matrix:\n", name);
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++)
            printf(" %5.2f", M[row][col]);
        printf("\n");
    }
    printf("\n\n");
}

void
Init_Default()
{
    N = 5;
    Init = "fast";
    maxnum = 15.0;
    PRINT = 1;
}

int
Read_Options(int argc, char** argv)
{
    char* prog;

    prog = *argv;
    while (++argv, --argc > 0)
        if (**argv == '-')
            switch (*++ * argv) {
            case 'n':
                --argc;
                N = atoi(*++argv);
                break;
            case 'h':
                printf("\nHELP: try matinv -u \n\n");
                exit(0);
                break;
            case 'u':
                printf("\nUsage: matinv [-n problemsize]\n");
                printf("           [-D] show default values \n");
                printf("           [-h] help \n");
                printf("           [-I init_type] fast/rand \n");
                printf("           [-m maxnum] max random no \n");
                printf("           [-P print_switch] 0/1 \n");
                exit(0);
                break;
            case 'D':
                printf("\nDefault:  n         = %d ", N);
                printf("\n          Init      = rand");
                printf("\n          maxnum    = 5 ");
                printf("\n          P         = 0 \n\n");
                exit(0);
                break;
            case 'I':
                --argc;
                Init = *++argv;
                break;
            case 'm':
                --argc;
                maxnum = atoi(*++argv);
                break;
            case 'P':
                --argc;
                PRINT = atoi(*++argv);
                break;
            default:
                printf("%s: ignored option: -%s\n", prog, *argv);
                printf("HELP: try %s -u \n\n", prog);
                break;
            }
}
