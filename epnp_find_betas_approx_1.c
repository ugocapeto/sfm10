#include "header.h"

void epnp_find_betas_approx_1(
 double *L_6x10,
 double *Rho,
 double *betas
)

/*
betas10        = [B11 B12 B22 B13 B23 B33 B14 B24 B34 B44]
betas_approx_1 = [B11 B12     B13         B14]
*/

{

 double L_6x4[6 * 4];
 double B4[4];
 int i;

 for ( i= 0 ; i< 6 ; i++ ) {
    L_6x4[i * 4 + 0]= L_6x10[i * 10 + 0];
    L_6x4[i * 4 + 1]= L_6x10[i * 10 + 1];
    L_6x4[i * 4 + 2]= L_6x10[i * 10 + 3];
    L_6x4[i * 4 + 3]= L_6x10[i * 10 + 6]; 
 }

 math_singular_value_decomposition_solve(
  6,
  4,
  L_6x4,
  Rho,
  B4 
 );

  if (B4[0] < 0) {
    betas[0] = sqrt(-B4[0]);
    betas[1] = -B4[1] / betas[0];
    betas[2] = -B4[2] / betas[0];
    betas[3] = -B4[3] / betas[0];
  } else {
    betas[0] = sqrt(B4[0]);
    betas[1] = B4[1] / betas[0];
    betas[2] = B4[2] / betas[0];
    betas[3] = B4[3] / betas[0];
  }

}
