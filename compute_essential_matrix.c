#include "header.h"

void compute_essential_matrix(
 double F[3*3],
 double K1[3*3],
 double K2[3*3],
 double E[3*3]
)

/*
Go from fundamental matrix to essential matrix
using the two calibration matrices
E = K't * F * K
*/

{

 double FK1[3*3];
 double K2t[3*3];

 /*
 Compute FK1 = F * K
 */

 math_matrix_matrix_product(
  F,
  3,
  3,
  K1,
  3,
  3,
  FK1
 );

 /*
 Compute K2t = K't
 */

 math_matrix_transpose(
  K2,
  3,
  3,
  K2t 
 );

 /*
 Compute E = K't * F * K
 */

 math_matrix_matrix_product(
  K2t,
  3,
  3,
  FK1,
  3,
  3,
  E
 );

}
