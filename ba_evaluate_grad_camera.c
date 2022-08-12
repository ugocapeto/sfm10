#include "header.h"

void ba_evaluate_grad_camera(
 double K[3*3],
 double omega3[3],
 double T[3],
 double X3[3],
 double x3_proj[3],
 double deriv2x6[2*6]
)

{

 double C[3*6];
 double B[3*6];
 double dR[3][3*3];
 int ind;
 double X3b[3];
 int i;
 int j;
 double U;
 double V;
 double W;
 double W2;
 double A[2*3];

 U= x3_proj[0];
 V= x3_proj[1];
 W= x3_proj[2];
 W2= W*W;

 if ( W == 0 ) {
    sfm10_error_handler("ba_evaluate_grad_point");
 }

 A[0*3+0]= 1/W;
 A[0*3+1]= 0;
 A[0*3+2]=-U/W2;
 A[1*3+0]= 0;
 A[1*3+1]= 1/W;
 A[1*3+2]=-V/W2;

 /*
 Compute derivatives of the projection of the 3D point
 w/r the camera parameters
 */

 ba_rodrigues_rotation_derivatives(
  omega3,
  dR
 );

 for ( ind= 0 ; ind< 3 ; ind++ ) {

    math_matrix_vector_product(
     dR[ind],
     3,
     3,
     X3,
     3,
     X3b
    );

    j= ind;
    for ( i= 0 ; i< 3 ; i++ ) {
       C[i*6+j]= X3b[i];
    }
 }

 for ( j= 0 ; j< 3 ; j++ ) {
    for ( i= 0 ; i< 3 ; i++ ) {
       C[i*6+3+j]= 0;
    }
    i= j;
    C[i*6+3+j]= 1;
 }

 math_matrix_matrix_product(
  K,
  3,
  3,
  C,
  3,
  6,
  B
 );

 math_matrix_matrix_product(
  A,
  2,
  3,
  B,
  3,
  6,
  deriv2x6
 );

}
