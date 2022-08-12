#include "header.h"

void compute_RQ_decomposition(
 double A[3*3],
 double R[3*3],
 double Q[3*3]
)

/*
Decompose A into R*Q where
R is upper-triangular
Q is orthogonal
In the context of the decomposition of a camera matrix X,
R corresponds to K, the calibration matrix 
Q corresponds to R, the rotation matrix 
*/

{

 double Qx[3*3];
 double Qy[3*3];
 double Qz[3*3];
 int axis;
 double AQx[3*3];
 double AQxQy[3*3];
 double AQxQyQz[3*3];
 int i;
 int j;
 double Qxt[3*3];
 double Qyt[3*3];
 double Qzt[3*3];
 double QytQxt[3*3];
 double QztQytQxt[3*3];
 double D[3*3];
 double Q2[3*3];
 double R2[3*3];
 double det;

 axis= 1-1;
 compute_givens_rotation_matrix(
  A,
  axis,
  Qx
 );

 math_matrix_matrix_product(
  A,
  3,
  3, 
  Qx,
  3,
  3,
  AQx
 );

 axis= 2-1;
 compute_givens_rotation_matrix(
  AQx,
  axis,
  Qy
 );

 math_matrix_matrix_product(
  AQx,
  3,
  3,
  Qy,
  3,
  3,
  AQxQy
 );

 axis= 3-1;
 compute_givens_rotation_matrix(
  AQxQy,
  axis,
  Qz
 );

 math_matrix_matrix_product(
  AQxQy,
  3,
  3,
  Qz,
  3,
  3,
  AQxQyQz
 );

 /*
 R = A * Qx * Qy * Qz
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[i*3+j]= AQxQyQz[i*3+j];
    }
 }

 math_matrix_transpose(
  Qx,
  3,
  3,
  Qxt 
 );

 math_matrix_transpose(
  Qy,
  3,
  3,
  Qyt
 );

 math_matrix_transpose(
  Qz,
  3,
  3,
  Qzt
 );

 math_matrix_matrix_product(
  Qyt,
  3,
  3,
  Qxt,
  3,
  3,
  QytQxt
 );

 math_matrix_matrix_product(
  Qzt,
  3,
  3,
  QytQxt,
  3,
  3,
  QztQytQxt
 );

 /*
 Q = Qzt * Qyt * Qxt
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       Q[i*3+j]= QztQytQxt[i*3+j];
    }
 }

 /*
 See stuff10/CV/CameraGeometry/RQDecomposition.m
 */

 /* turned off
 if ( Q[(1-1)*3+(1-1)] < 0 ) {

    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          D[i*3+j]= 0;
       }
    }
    D[0*3+0]=-1;
    D[1*3+1]=-1;
    D[2*3+2]= 1;

    math_matrix_matrix_product(
     Q,
     3,
     3,
     D,
     3,
     3,
     Q2
    );

    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          Q[i*3+j]= Q2[i*3+j];
       }
    }

    math_matrix_matrix_product(
     D,
     3,
     3,
     R,
     3,
     3,
     R2
    );
    
    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          R[i*3+j]= R2[i*3+j];
       }
    }
 }
 */

 /*
 The RQ decomposition is not unique
 We want the diagonal elements of R to be positive 
 This will give us a unique decomposition
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       D[i*3+j]= 0;
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    D[i*3+i]= 1;
    if ( R[i*3+i] < 0 )
     D[i*3+i]=-1;
 }

 /*
 math_matrix_matrix_product(
  Q,
  3,
  3,
  D,
  3,
  3,
  Q2
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       Q[i*3+j]= Q2[i*3+j];
    }
 }
 */

 math_matrix_matrix_product(
  R,
  3,
  3,
  D,
  3,
  3,
  R2
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[i*3+j]= R2[i*3+j];
    }
 }

 /*
 math_matrix_matrix_product(
  D,
  3,
  3,
  R,
  3,
  3,
  R2
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[i*3+j]= R2[i*3+j];
    }
 }
 */

 math_matrix_matrix_product(
  D,
  3,
  3,
  Q,
  3,
  3,
  Q2
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       Q[i*3+j]= Q2[i*3+j];
    }
 }

 /*
 At this point,
 we have an all-positive diagonal
 */

 /*
 Following comes from:
 http://ksimek.github.io/2012/08/14/decompose

 Starting from an all-positive diagonal, follow these four steps:

 1. If the image x-axis and camera x-axis point in opposite directions,
    negate the first column of K (R here) and the first row of R (Q here)
 2. If the image y-axis and camera y-axis point in opposite directions,
    negate the second column of K (R here) and the second row of R (Q here)
 3. If the camera looks down the negative z-axis,
    negate the third column of K (R here) and the third column of R (Q here)
 4. If the determinant of R (Q here) is -1,
    negate it

 Note that each of these steps leaves the combined camera matrix unchanged.
 The last step is equivalent to multiplying the entire camera matrix, P, by -1.
 Since P operates on homegeneous coordinates,
 multiplying it by any constant has no effect.
 */

 /*
 We should only be concerned by the last step
 */

 /*
 Compute the determinant of R (Q here)
 */

 det= math_matrix3x3_determinant(
  Q
 );

 if ( det < 0 ) {

    /*
    Negate R (Q here)
    */

    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          Q[i*3+j]*=-1;
       }
    }
 }

}
