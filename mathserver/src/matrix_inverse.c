/***************************************************************************
 *
 * Sequential version of Matrix Inverse
 * An adapted version of the code by H�kan Grahn
 * Upgraded to parallell inversion by David Värmfors & Albin Södervall
 ***************************************************************************/


#include "../include/matrix_inverse.h"

matrix I = {0.0};

// int
// main(int arguments, char** buffer)
// {
//     for(int j=0; j<arguments; j++){
//         // for (int i = 0; i < 6; i++)
//         // {
//         //     printf("%s", buffer[j]);
//         // }
//         printf("%s", buffer[j]);
//     }
//     printf("%d", arguments);
// }

void matStart(int arguments, char** buffer)
{
    // printf("Hej");
    // puts(buffer);
    // printf("%i\n", arguments);
    // printf("Matrix Inverse\n");
    // int i, timestart, timeend, iter;
    // pthread_t *children;
    // unsigned long id = 0;
    // children = malloc( MAX_SIZE * sizeof(pthread_t) );

    // Init_Default();		/* Init default values	*/
    // printf("Färdigt för bövelen");
    // Read_Option(arguments, buffer);	/* Read arguments	*/
    // Init_Matrix();		/* Init the matrix	*/
    // find_inverse();

    if (PRINT == 1)
    {
        //Print_Matrix(A, "End: Input");
        Print_Matrix(I, "Inversed");
    }
}

void find_inverse()
{
    int row, col, p; // 'p' stands for pivot (numbered from 0 to N-1)
    double pivalue; // pivot value

    /* Bringing the matrix A to the identity form */
    for (p = 0; p < N; p++) { /* Outer loop */
        pivalue = A[p][p];
        for (col = 0; col < N; col++)
        {
            A[p][col] = A[p][col] / pivalue; /* Division step on A */
            I[p][col] = I[p][col] / pivalue; /* Division step on I */
        }
        assert(A[p][p] == 1.0);

        double multiplier;
        for (row = 0; row < N; row++) {
            multiplier = A[row][p];
            if (row != p) // Perform elimination on all except the current pivot row 
            {
                for (col = 0; col < N; col++)
                {
                    A[row][col] = A[row][col] - A[p][col] * multiplier; /* Elimination step on A */
                    I[row][col] = I[row][col] - I[p][col] * multiplier; /* Elimination step on I */
                }      
                assert(A[row][p] == 0.0);
            }
        }
    }
}

void
Init_Matrix()
{
    int row, col;
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

    if (strcmp(Init, "rand") == 0) {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (row == col) /* diagonal dominance */
                    A[row][col] = (double)(rand() % maxnum) + 5.0;
                else
                    A[row][col] = (double)(rand() % maxnum) + 1.0;
            }
        }
    }
    if (strcmp(Init, "fast") == 0) {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (row == col) /* diagonal dominance */
                    A[row][col] = 5.0;
                else
                    A[row][col] = 2.0;
            }
        }
    }

    printf("done \n\n");
    if (PRINT == 1)
    {
        //Print_Matrix(A, "Begin: Input");
        Print_Matrix(I, "Begin: Inverse");
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
    printf("INIT FÄRDIG");
}

void
Read_Option(int arguments, char** buffer)
{
    printf("HÄR KOM VI");
    char* prog;
    prog = *buffer;
    int count=0;
    while (++buffer, --arguments > 0)
        
        if (**buffer == '-')
            switch (*++ * buffer) {
            case 'n':
                printf("-n\n");
                --arguments;
                N = atoi(*++buffer);
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
                printf("-I\n");
                --arguments;
                Init = *++buffer;
                break;
            case 'm':
                --arguments;
                maxnum = atoi(*++buffer);
                break;
            case 'P':
                printf("-P\n");
                --arguments;
                PRINT = atoi(*++buffer);
                break;
            default:
                printf("%s: ignored option: -%s\n", prog, *buffer);
                printf("HELP: try %s -u \n\n", prog);
                break;
            }
}

    // printf("\n");
    // for(int i=0; i<1024; i++){
    //     for(int k=0; k<1024; k++){
    //         printf("%d", buffer[i][k]);
    //     }
    // }

    // for(int j=0; j<1024; j++){
    //     // printf("%d HÄR", buffer[1][j]);
    //     if(**buffer == 45){
    //         switch(buffer[0][j++]){
    //             case 'n':
    //                 printf("VI NÅDDE N");
    //                 N = atoi(buffer[j+2]);
    //                 break;
    //             case 'h':
    //                 printf("\nHELP: try matinv -u \n\n");
    //                 exit(0);
    //                 break;
    //             case 'u':
    //                 printf("\nUsage: matinv [-n problemsize]\n");
    //                 printf("           [-D] show default values \n");
    //                 printf("           [-h] help \n");
    //                 printf("           [-I init_type] fast/rand \n");
    //                 printf("           [-m maxnum] max random no \n");
    //                 printf("           [-P print_switch] 0/1 \n");
    //                 exit(0);
    //                 break;
    //             case 'D':
    //                 printf("\nDefault:  n         = %d ", N);
    //                 printf("\n          Init      = rand");
    //                 printf("\n          maxnum    = 5 ");
    //                 printf("\n          P         = 0 \n\n");
    //                 exit(0);
    //                 break;
    //             case 'I':
    //                 Init = *++buffer;
    //                 break;
    //             default:
    //             printf("%s: ignored option: -%s\n", prog, *buffer);
    //             printf("HELP: try %s -u \n\n", prog);
    //             break;
    //         }
    //     }
    // }

    // printf("%i \n", arguments);
    // for(int j=0; j<1024; j++){
    //     printf("%s", buffer[j]);
    // }