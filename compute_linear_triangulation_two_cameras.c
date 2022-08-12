#include "header.h"

void compute_linear_triangulation_two_cameras(
 double P1[3*4],
 double P2[3*4],
 double x1,
 double y1,
 double x2,
 double y2,
 double X[4]
)

/*
Compute location of 3D point X in projective space (homogeneous 4-vector)
given the camera matrices and the matching locations on the image planes
*/

{

 double A[4*4];
 int i;
 int j;
 double U[4*4];
 double w[4];
 double V[4*4];

 /*
 Compute A (A*X=0)
 */

 i= 0;
 for ( j= 0 ; j< 4 ; j++ )
  A[i*4+j]= x1*P1[(3-1)*4+j] - P1[(1-1)*4+j];

 i= 1;
 for ( j= 0 ; j< 4 ; j++ )
  A[i*4+j]= y1*P1[(3-1)*4+j] - P1[(2-1)*4+j];

 i= 2;
 for ( j= 0 ; j< 4 ; j++ )
  A[i*4+j]= x2*P2[(3-1)*4+j] - P2[(1-1)*4+j];

 i= 3;
 for ( j= 0 ; j< 4 ; j++ )
  A[i*4+j]= y2*P2[(3-1)*4+j] - P2[(2-1)*4+j];

 /*
 Perform the Singular Value Decomposition of A
 */

 math_singular_value_decomposition(
  4,
  4,
  A,
  U,
  w,
  V
 );

 /*
 What we are looking for is the unit singular vector
 corresponding to the smallest singular value of A
 (should be the last column of V that is 4-1=3)
 */

 for ( i= 0 ; i< 4 ; i++ )
  X[i]= V[i*4+(4-1)];

}
