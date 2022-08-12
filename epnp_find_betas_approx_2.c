#include "header.h"

void epnp_find_betas_approx_2(
 double *L_6x10,
 double *Rho,
 double *betas
)

/*
betas10        = [B11 B12 B22 B13 B23 B33 B14 B24 B34 B44]
betas_approx_2 = [B11 B12 B22                            ]
*/

{

 double L_6x3[6 * 3];
 double B3[3];
 int i;

 for ( i= 0 ; i< 6 ; i++ ) {
    L_6x3[i * 3 + 0]= L_6x10[i * 10 + 0];
    L_6x3[i * 3 + 1]= L_6x10[i * 10 + 1];
    L_6x3[i * 3 + 2]= L_6x10[i * 10 + 2];
  }

 math_singular_value_decomposition_solve(
  6,
  3,
  L_6x3,
  Rho,
  B3
 );

  if (B3[0] < 0) {
    betas[0] = sqrt(-B3[0]);
    betas[1] = (B3[2] < 0) ? sqrt(-B3[2]) : 0.0;
  } else {
    betas[0] = sqrt(B3[0]);
    betas[1] = (B3[2] > 0) ? sqrt(B3[2]) : 0.0;
  }

  if (B3[1] < 0) betas[0] = -betas[0];

  betas[2] = 0.0;
  betas[3] = 0.0;

}
