#include "header.h"

void compute_2nd_camera_matrix_from_fundamental_matrix(
 double F[3*3],
 double P2[3*4]
)

/*
This computes the 2nd camera matrix [e'x*F|e']
assuming the 1st camera matrix is [I|0]
These are the camera matrices
up to a projective transformation, in other words,
it can only lead to a projective reconstruction,
not an Euclidean one
*/

{

 double U[3*3];
 double w[3];
 double V[3*3];
 double e2x[3*3];
 int i;
 double e2[3];
 double e2xF[3*3];
 int j;

 /*
 Get the Singular Value Decomposition of F 
 */

 math_singular_value_decomposition(
  3,
  3,
  F,
  U,
  w,
  V
 );

 /*
 Get the epipole in the 2nd image
 (third column of U)
 */

 for ( i= 0 ; i< 3 ; i++ )
  e2[i]= U[i*3+2];

 /*
 Compute e'x
 */

 math_cross_product_matrix(
  e2,
  e2x 
 );

 /*
 Compute e'x*F
 */

 math_matrix_matrix_product(
  e2x,
  3,
  3, 
  F,
  3,
  3,
  e2xF
 );

 /*
 Compute P2, the 2nd camera matrix
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P2[i*4+j]= e2xF[i*3+j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P2[i*4+j]= e2[i];
 }

}
