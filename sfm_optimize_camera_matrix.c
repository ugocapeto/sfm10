#include "header.h"

void sfm_optimize_camera_matrix(
 double *X_arr,
 double *x_arr,
 int n, 
 double P[3*4]
)

/*
X_arr are the space point coordinates in homogeneous form
x_arr are the image point coordinates in homogeneous form
*/

/*
Optimize the camera matrix using non-linear least squares
given a set of n correspondences between 3D points
and image points
*/

/*
In order to have well-conditioned matrices,
the data should be normalized prior to solving
*/

{

 double B[12];
 double JtJ[12*12];
 double delta[12];
 double lambda_init;
 double lambda_fact;
 int max_iter;
 double *res_vec;
 double *J;
 double *Jt;
 int iter;
 double norm2;
 double lambda;
 int i;
 int j;
 double H[12*12];
 double try_P[3*4];
 double avg_error2;
 double avg_error;
 double try_norm2;

 lambda_init= 1.0e-3;
 lambda_fact= 10.0;
 max_iter= 300;
 /* commented out
 max_iter= 10000;
 */

 /*
 Allocate memory for res_vec
 Vector of size 2*n
 You need to multiply the number of correspondences by 2
 because there's a coordinate along x and y in the image
 res_vec[2*i+0] is the x coordinate
 res_vec[2*i+1] is the y coordinate
 */

 res_vec= (double *)calloc(2*n,sizeof(double));

 /*
 Allocate memory for J
 Matrix with 2*n rows and 12 columns
 */

 J= (double *)calloc(2*n,12*sizeof(double));

 /*
 Allocate memory for Jt
 Matrix with 12 rows and 2*n columns
 */

 Jt= (double *)calloc(12,2*n*sizeof(double));

 iter=-1;

 /*
 Compute the residual vector
 y - f(beta)
 where beta are the parameters
 y is the vector of measured 2d image points
 f(beta) is the vector of projected 3d points onto the image
 */

 sfm_optimize_camera_matrix_residual(
  X_arr,
  x_arr,
  n,
  P,
  res_vec
 );

 /*
 Compute the sum of the errors squared
 */

 norm2= math_vector_vector_product(
  res_vec,
  2*n,
  res_vec,
  2*n
 );

 avg_error2= norm2/(double)n;
 avg_error= sqrt(avg_error2);
 fprintf(stdout,"Initial average reprojection error (in pixels) = %f\n",avg_error);

 /*
 Compute the Jacobian matrix of f
 Row (2*i+0) contains the derivative of (PXi)[1]/(PXi)[3]
 w/r to the parameters
 Row (2*i+1) contains the derivative of (PXi)[2]/(PXi)[3]
 w/r to the parameters
 */

 sfm_optimize_camera_matrix_jacobian(
  X_arr,
  x_arr,
  n,
  P,
  J
 );

 /*
 Compute the transpose of the jacobian
 */

 math_matrix_transpose(
  J,
  2*n,
  12,
  Jt
 );

 /*
 Compute JtJ=Jt*J
 */

 math_matrix_matrix_product(
  Jt,
  12,
  2*n,
  J,
  2*n,
  12,
  JtJ
 );

 /*
 Compute right hand side vector
 B=Jt*(y-f(beta))
 */

 math_matrix_vector_product(
  Jt,
  12,
  2*n,
  res_vec,
  2*n, 
  B
 );

 lambda= lambda_init;

 START:

 iter++;

 /*
 Compute left hand side matrix
 H=Jt*J+lambda*diag(Jt*J)
 */

 for ( i= 0 ; i< 12 ; i++ ) {
    for ( j= 0 ; j< 12 ; j++ ) {
       H[i*12+j]= JtJ[i*12+j];
    }
 }
 for ( i= 0 ; i< 12 ; i++ ) {
    j= i;
    H[i*12+j]+= lambda*JtJ[i*12+j];
 }

 /*
 Solve the linear system
 H*delta=B
 for delta, the increment to P
 */

 math_lu_decomposition_solve(
  12,
  H,
  B,
  delta
 );

 /*
 Add the increment to P
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 4 ; j++ ) {
       try_P[i*4+j]= P[i*4+j]+delta[i*4+j];
    }
 }

 /*
 Compute the residual vector
 y - f(beta)
 where beta are the parameters
 y is the vector of measured 2d image points
 f(beta) is the vector of projected 3d points onto the image
 */

 sfm_optimize_camera_matrix_residual(
  X_arr,
  x_arr,
  n,
  try_P,
  res_vec
 );

 /*
 Compute the sum of the errors squared
 */

 try_norm2= math_vector_vector_product(
  res_vec,
  2*n,
  res_vec,
  2*n
 );

 if ( try_norm2 > norm2 ) {
    lambda *= lambda_fact;
 }
 else {
    lambda/= lambda_fact;

    norm2= try_norm2;

    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 4 ; j++ ) {
          P[i*4+j]= try_P[i*4+j];
       }
    }

    /*
    Compute the Jacobian matrix of f
    Row (2*i+0) contains the derivative of (PXi)[1]/(PXi)[3]
    w/r to the parameters
    Row (2*i+1) contains the derivative of (PXi)[2]/(PXi)[3]
    w/r to the parameters
    */

    sfm_optimize_camera_matrix_jacobian(
     X_arr,
     x_arr,
     n,
     P,
     J
    );

    /*
    Compute the transpose of the jacobian
    */

    math_matrix_transpose(
     J,
     2*n,
     12,
     Jt
    );

    /*
    Compute JtJ=Jt*J
    */

    math_matrix_matrix_product(
     Jt,
     12,
     2*n,
     J,
     2*n,
     12,
     JtJ
    );

    /*
    Compute right hand side vector
    B=Jt*(y-f(beta))
    */

    math_matrix_vector_product(
     Jt,
     12,
     2*n,
     res_vec,
     2*n,
     B
    );

 }

 if ( iter < max_iter )
  goto START;

 avg_error2= norm2/(double)n;
 avg_error= sqrt(avg_error2);
 fprintf(stdout,"Final average reprojection error (in pixels) = %f\n",avg_error);

 /*
 Free memory for res_vec
 */

 free(res_vec);

 /*
 Free memory for J
 */

 free(J);

 /*
 Free memory for Jt
 */

 free(Jt);

}
