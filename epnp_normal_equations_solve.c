#include "header.h"

void epnp_normal_equations_solve(
 double *A,
 int nr,
 int nc,
 double *b,
 double *x
)

/*
Solve A*x=b via the normal equations (At*A)x=At*b
nr = number of rows for matrix A
nc = number of cols for matrix A
using x = (At*A)^-1*At*b
*/

/*
This is assumed to be an over-determined system (nr>=nc)
*/

/*
Wrote that to check epnp_qr_solve()
epnp_normal_equations_solve() gives the same results as epnp_qr_solve()
which shows that epnp_qr_solve() works correctly
even if the output x doesn't satisfy Ax=b
x is however the best solution in the least squares sense
*/

{

 double *At;
 double *AtA;
 double *AtA1;
 double *Atb;
 double *Ax;

 /*
 Compute At
 */

 At= (double *)calloc(nc*nr,sizeof(double));

 math_matrix_transpose(
  A,
  nr,
  nc,
  At 
 );

 /*
 Compute At*A
 */

 AtA= (double *)calloc(nc*nc,sizeof(double));

 math_matrix_matrix_product(
  At,
  nc,
  nr,
  A,
  nr,
  nc,
  AtA 
 );

 /*
 Compute (At*A)^-1
 */

 AtA1= (double *)calloc(nc*nc,sizeof(double));

 math_lu_decomposition_inverse(
  nc,
  AtA,
  AtA1
 );

 /*
 Compute At*b
 */

 Atb= (double *)calloc(nc,sizeof(double));

 math_matrix_vector_product(
  At,
  nc,
  nr,
  b,
  nr,
  Atb
 );

 /*
 Compute x=(At*A)^-1*At*b
 */

 math_matrix_vector_product(
  AtA1,
  nc,
  nc,
  Atb,
  nc,
  x
 );

 /*
 Free At
 */

 free(At);

 /*
 Free AtA
 */

 free(AtA);

 /*
 Free AtA1
 */

 free(AtA1);

 /*
 Free Atb
 */

 free(Atb);

 /*
 Check!
 */

 Ax= (double *)calloc(nr,sizeof(double));

 math_matrix_vector_product(
  A,
  nr,
  nc,
  x,
  nc,
  Ax
 );

 free(Ax);

}
