#include "header.h"

void ba_evaluate_grad_point(
 double K[3*3],
 double omega3[3],
 double T[3],
 double X3[3],
 double x3_proj[3],
 double deriv2x3[2*3]
)

{

 double B[3*3];
 double R[3*3];
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

 ba_create_rotation_matrix_rodrigues(
  omega3,
  R
 );

 math_matrix_matrix_product(
  K,
  3,
  3,
  R,
  3,
  3,
  B
 );

 math_matrix_matrix_product(
  A,
  2,
  3,
  B,
  3,
  3,
  deriv2x3
 );

}
