#include "header.h"

void epnp_gauss_newton(
 double *L_6x10,
 double *Rho,
 double betas[4]
)

{

 int iterations_number = 5;
 double A[6*4];
 double B[6];
 double X[4];
 int k;
 int i;

 for ( k= 0 ; k< iterations_number ; k++ ) {
    epnp_compute_A_and_b_gauss_newton(
     L_6x10,
     Rho,
     betas,
     A,
     B
    );

    /*
    Solve using QR factorization
    */

    epnp_qr_solve(
     A,
     6,
     4,
     B,
     X
    );

    /*
    Solve directly using normal equations
    */

    /* commented out since same result
    epnp_normal_equations_solve(
     A,
     6,
     4,
     B,
     X
    );
    */

    for ( i= 0 ; i< 4 ; i++ )
     betas[i] += X[i];
 }

}
