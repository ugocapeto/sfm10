#include "header.h"

void epnp_find_betas_approx_3(
 double *L_6x10,
 double *Rho,
 double *betas
)

/*
betas10        = [B11 B12 B22 B13 B23 B33 B14 B24 B34 B44]
betas_approx_3 = [B11 B12 B22 B13 B23                    ]
*/

{

 double L_6x5[6 * 5];
 double B5[5];
 int i;

 for ( i= 0 ; i< 6 ; i++ ) {
    L_6x5[i * 5 + 0]= L_6x10[i * 10 + 0];
    L_6x5[i * 5 + 1]= L_6x10[i * 10 + 1];
    L_6x5[i * 5 + 2]= L_6x10[i * 10 + 2];
    L_6x5[i * 5 + 3]= L_6x10[i * 10 + 3];
    L_6x5[i * 5 + 4]= L_6x10[i * 10 + 4];
  }

 math_singular_value_decomposition_solve(
  6,
  5,
  L_6x5,
  Rho,
  B5
 );

  if (B5[0] < 0) {
    betas[0] = sqrt(-B5[0]);
    betas[1] = (B5[2] < 0) ? sqrt(-B5[2]) : 0.0;
  } else {
    betas[0] = sqrt(B5[0]);
    betas[1] = (B5[2] > 0) ? sqrt(B5[2]) : 0.0;
  }
  if (B5[1] < 0) betas[0] = -betas[0];
  betas[2] = B5[3] / betas[0];
  betas[3] = 0.0;

}
