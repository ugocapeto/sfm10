#include "header.h"

void sfm_compute_camera_matrix_main(
 double *xyz_arr,
 double *xy_arr,
 int n, 
 double P[3*4]
)

/*
Find the camera matrix using linear least squares
given a set of n correspondences between 3D points
and image points
*/

/*
In order to have well-conditioned matrices,
the data should be normalized prior to solving
*/

/*
The solution should be improved with non-linear optimization
*/

{

 double *X_arr;
 double *x_arr;
 int ind;
 double U[4*4];
 double T[3*3];
 double *normalized_X_arr;
 double *normalized_x_arr;
 double PU[3*4];
 double T1[3*3];

 if ( n > 0 )
  X_arr= (double *)calloc(n,4*sizeof(double));

 for ( ind= 0 ; ind< n ; ind++ ) {
    X_arr[4*ind+0]= xyz_arr[3*ind+0];
    X_arr[4*ind+1]= xyz_arr[3*ind+1];
    X_arr[4*ind+2]= xyz_arr[3*ind+2];
    X_arr[4*ind+3]= 1;
 }

 if ( n > 0 )
  x_arr= (double *)calloc(n,3*sizeof(double));

 for ( ind= 0 ; ind< n ; ind++ ) {
    x_arr[3*ind+0]= xy_arr[2*ind+0];
    x_arr[3*ind+1]= xy_arr[2*ind+1];
    x_arr[3*ind+2]= 1;
 }

 /*
 Normalize the 3D points
 */

 if ( n > 0 )
  normalized_X_arr= (double *)calloc(n,4*sizeof(double));

 sfm_normalize_3dpoints(
  X_arr,
  n,
  U,
  normalized_X_arr
 );

 /*
 Normalize the 2D image points
 */

 if ( n > 0 ) 
  normalized_x_arr= (double *)calloc(n,3*sizeof(double));

 sfm_normalize_2dpoints(
  x_arr,
  n,
  T,
  normalized_x_arr
 );

 /*
 Compute the camera matrix P
 using normalized coordinates
 */

 sfm_compute_camera_matrix(
  normalized_X_arr,
  normalized_x_arr,
  n,
  P
 );

 /*
 Optimize the camera matrix P
 using normalized coordinates
 */

 /*
 sfm_optimize_camera_matrix(
  normalized_X_arr,
  normalized_x_arr,
  n,
  P
 );
 */

 /*
 Compute the denormalized camera matrix P
 */

 math_matrix_matrix_product(
  P,
  3,
  4, 
  U,
  4,
  4,
  PU
 );

 /*
 math_lu_decomposition_inverse(
  3,
  T,
  T1 
 );
 */

 math_matrix3x3_inverse(
  T,
  T1
 );

 math_matrix_matrix_product(
  T1,
  3,
  3,
  PU,
  3,
  4,
  P
 );

 if ( n > 0 )
  free(X_arr);

 if ( n > 0 )
  free(x_arr);

 if ( n > 0 )
  free(normalized_X_arr);

 if ( n > 0 )
  free(normalized_x_arr);

}
