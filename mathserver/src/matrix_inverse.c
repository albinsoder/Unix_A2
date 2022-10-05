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
        // Print_Matrix(A, "End: Input");
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
    children = malloc(N * sizeof(pthread_t));
    int n_threads = 8; // Number of threads to be created
    int rest=0;
    int factor = 0;
    int flag = 0;
    if(N < n_threads){
        n_threads=N;
        rest = 0;
    }
    else if(N % 8 != 0){
        rest = N%8;
        flag=1;
        factor = (N-rest)/8;
    }

    pthread_barrier_init(&barrier, NULL, n_threads);                          // Initialize barrier
    struct Th *t_info;
    int n_mod8 = (N-rest);
    double pi;
    for(int i=0; i<N; i++){
        pi = A[i][i];
        for (int id = 0; id < n_threads; id++) {                              /* Outer loop */
            if(flag && id == n_threads-1){
                t_info = (struct Th*)malloc(sizeof(struct Th));              //if N%8!=0 then we give last thread more workload
                t_info->size=-1;                                             //functions as a flag
                t_info->pivalue=pi;                                          //pivalue
                t_info->p = i;                                               //i=outer loop iteration
                t_info->i = id*factor;                                       //factor =
                t_info->end = N;                                             //where we stop
                pthread_create(&(children[id]), NULL, help_inverse, t_info); // Let current thread perform its inversion
            } else{
                t_info = (struct Th*)malloc(sizeof(struct Th));
                t_info->size=n_mod8;
                t_info->pivalue=pi;
                t_info->p = i;
                t_info->i = id;
                pthread_create(&(children[id]), NULL, help_inverse, t_info); // Let current thread perform its inversion
            }
        }
        for (int id = 0; id < n_threads; id++) {
            pthread_join(children[id], NULL);                                 // Collect/join result from the threads
        }
        pthread_barrier_destroy(&barrier);
    }
    free(t_info);
    free(children);
}

void* help_inverse(void* id)
{
    int row, col, start, end, thread_div, n_thread, size_n; // 'p' stands for pivot (numbered from 0 to N-1)

    struct Th *t_info = (struct Th*)id;

    size_n= (int)t_info->size;
    if(size_n == -1){
        start = t_info->i;
        end = t_info->end;
    }
    else if(size_n < 8){
        thread_div=1;
        start = t_info->i*thread_div;
        end = (t_info->i+1)*thread_div;
        // printf("SIZE: %d", sizeN);
    } else{
        thread_div = size_n/8;
        start = t_info->i*thread_div;
        end = (t_info->i+1)*thread_div;
    }

    for (col = start; col < end; col++)
    {
        A[t_info->p][col] = A[t_info->p][col] / t_info->pivalue; /* Division step on A */
        I[t_info->p][col] = I[t_info->p][col] / t_info->pivalue; /* Division step on I */

    }
    pthread_barrier_wait(&barrier);
    assert(A[t_info->p][t_info->p] == 1.0);
    double multiplier;

    for (row = start; row < end; row++) 
    {
        multiplier = A[row][t_info->p];
        if (row != t_info->p) // Perform elimination on all except the current pivot row 
        {
            for (col = 0; col < N; col++)
            {
                A[row][col] = A[row][col] - A[t_info->p][col] * multiplier; /* Elimination step on A */
                I[row][col] = I[row][col] - I[t_info->p][col] * multiplier; /* Elimination step on I */
            }
            assert(A[row][t_info->p] == 0.0);
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
        // Print_Matrix(A, "Begin: Input");
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
