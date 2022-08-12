#include "header.h"

void sfm_compute_camera_matrix(
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

 double w12[12];
 double V[12*12];
 double *A;
 int ind;
 double x;
 double y;
 double X;
 double Y;
 double Z;
 double *U;
 int i;
 int j;
 double w;
 double T;

 /*
 Build the 2nx12 matrix A
 Each correspondence brings 2 rows to the matrix
 */

 A= (double *)calloc(2*n,12*sizeof(double));

 for ( ind= 0 ; ind< n ; ind++ ) {
    x= x_arr[3*ind+0];
    y= x_arr[3*ind+1];
    w= x_arr[3*ind+2];
    X= X_arr[4*ind+0];
    Y= X_arr[4*ind+1];
    Z= X_arr[4*ind+2];
    T= X_arr[4*ind+3];

    A[(2*ind+0)*12+ 0]= 0;
    A[(2*ind+0)*12+ 1]= 0;
    A[(2*ind+0)*12+ 2]= 0;
    A[(2*ind+0)*12+ 3]= 0;
    A[(2*ind+0)*12+ 4]=-w*X;
    A[(2*ind+0)*12+ 5]=-w*Y;
    A[(2*ind+0)*12+ 6]=-w*Z;
    A[(2*ind+0)*12+ 7]=-w*T;
    A[(2*ind+0)*12+ 8]= y*X;
    A[(2*ind+0)*12+ 9]= y*Y;
    A[(2*ind+0)*12+10]= y*Z;
    A[(2*ind+0)*12+11]= y*T;

    A[(2*ind+1)*12+ 0]= w*X;
    A[(2*ind+1)*12+ 1]= w*Y;
    A[(2*ind+1)*12+ 2]= w*Z;
    A[(2*ind+1)*12+ 3]= w*T;
    A[(2*ind+1)*12+ 4]= 0;
    A[(2*ind+1)*12+ 5]= 0;
    A[(2*ind+1)*12+ 6]= 0;
    A[(2*ind+1)*12+ 7]= 0;
    A[(2*ind+1)*12+ 8]=-x*X;
    A[(2*ind+1)*12+ 9]=-x*Y;
    A[(2*ind+1)*12+10]=-x*Z;
    A[(2*ind+1)*12+11]=-x*T;
 }

 /*
 Get the Singular Value Decomposition of A
 */

 U= (double *)calloc(2*n,12*sizeof(double));

 math_singular_value_decomposition(
  2*n,
  12,
  A,
  U,
  w12,
  V
 );

 /*
 The solution is the column of V
 corresponding to the smallest singular value
 Since the singular values are ordered in descending order (in w),
 the solution is the last column (index = 12-1) of V
 */

 j= 12-1;
 for ( i= 0 ; i< 12 ; i++ )
  P[i]= V[i*12+j];

 /*
 Free A
 */

 free(A);

 /*
 Free U
 */

 free(U);

}
