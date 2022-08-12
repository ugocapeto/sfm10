#include "header.h"

void epnp_compute_A_and_b_gauss_newton(
 double *l_6x10,
 double *rho,
 double betas[4],
 double *A,
 double *b
)

{

 double *rowL;
 double *rowA;
 int i;
 double dval;

 for ( i= 0 ; i< 6 ; i++ ) {
    rowL = l_6x10 + i * 10;
    rowA = A + i * 4;

    rowA[0] = 2 * rowL[0] * betas[0] +     rowL[1] * betas[1] +     rowL[3] * betas[2] +     rowL[6] * betas[3];
    rowA[1] =     rowL[1] * betas[0] + 2 * rowL[2] * betas[1] +     rowL[4] * betas[2] +     rowL[7] * betas[3];
    rowA[2] =     rowL[3] * betas[0] +     rowL[4] * betas[1] + 2 * rowL[5] * betas[2] +     rowL[8] * betas[3];
    rowA[3] =     rowL[6] * betas[0] +     rowL[7] * betas[1] +     rowL[8] * betas[2] + 2 * rowL[9] * betas[3];

    dval= rho[i] -
           (
            rowL[0] * betas[0] * betas[0] +
            rowL[1] * betas[0] * betas[1] +
            rowL[2] * betas[1] * betas[1] +
            rowL[3] * betas[0] * betas[2] +
            rowL[4] * betas[1] * betas[2] +
            rowL[5] * betas[2] * betas[2] +
            rowL[6] * betas[0] * betas[3] +
            rowL[7] * betas[1] * betas[3] +
            rowL[8] * betas[2] * betas[3] +
            rowL[9] * betas[3] * betas[3]
           );
    b[i]= dval;
 }

}
