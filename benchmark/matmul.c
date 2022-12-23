#include <stdio.h>

#define N 1536
float A[32][64];
float B[64][128];
float C[32][128];

//void init_array()
//{
//    int i, j;
//
//    for (i=0; i<N; i++) {
//        for (j=0; j<N; j++) {
//            A[i][j] = (1+(i*j)%1024)/2.0;
//            B[i][j] = (1+(i*j)%1024)/2.0;
//        }
//    }
//}

// void print_array()
// {
//     int i, j;

//     for (i=0; i<N; i++) {
//         for (j=0; j<N; j++) {
//             fprintf(stdout, "%lf ", C[i][j]);
//             if (j%80 == 79) fprintf(stdout, "\n");
//         }
//         fprintf(stdout, "\n");
//     }
// }

int main()
{
    int i, j, k;
    double t_start, t_end;

    for(i=0; i<32; i++)  {
        for(j=0; j<128; j++)  {
            C[i][j] = 0;
            for(k=0; k<64; k++)
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
        }
    }

#ifdef TEST
    print_array();
#endif
    return 0;
}
