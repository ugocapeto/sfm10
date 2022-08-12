#include "header.h"

void compute_2nd_camera_matrix_from_essential_matrix(
 double E[3*3],
 double P24[4][3*4]
)

/*
This computes the 2nd camera matrix
assuming the 1st camera matrix is [I|0]
These are the camera matrices
up to scale (the scale is fixed by setting the baseline to be 1)
*/

/*
It's really not clear whether the essential matrix
should be recomputed using new E = U*diag(1,1,0)*Vt
and redecomposed using SVD
If you look at
"Estimation of Relative Camera Positions for Uncalibrated Cameras"
by Richard Hartley,
he does not say anything about recomputing the essential matrix
*/

/*
If the determinant of the rotation is -1,
I consider -R instead of R so that its determinant becomes +1
I believe that if one R has a -1 determinant,
the other R will also have a -1 determinant
Alternative would be to consider -E instead of E
and recompute the rotations
*/

{

 double U[3*3];
 double w[3];
 double V[3*3];
 int i;
 int j;
 double W[3*3];
 double Z[3*3];
 double Vt[3*3];
 double WVt[3*3];
 double UWVt[3*3];
 double Wt[3*3];
 double WtVt[3*3];
 double UWtVt[3*3];
 double t[3];
 int ind;
 /*
 double max_abs_val;
 double abs_val;
 */
 double diag110[3*3];
 double diag110Vt[3*3];
 double newE[3*3];
 double det;
 double det1;
 double det2;
 double R1[3*3];
 double R2[3*3];

 /*
 Get the Singular Value Decomposition of E
 */

 math_singular_value_decomposition(
  3,
  3,
  E,
  U,
  w,
  V
 );

 /*
 Compute new E = U*diag(1,1,0)*Vt
 */

 math_matrix_transpose(
  V,
  3,
  3,
  Vt
 );

 diag110[0*3+0]=1;
 diag110[0*3+1]=0;
 diag110[0*3+2]=0;
 diag110[1*3+0]=0;
 diag110[1*3+1]=1;
 diag110[1*3+2]=0;
 diag110[2*3+0]=0;
 diag110[2*3+1]=0;
 diag110[2*3+2]=0;
       
 math_matrix_matrix_product(
  diag110,
  3,
  3,
  Vt,
  3,
  3,
  diag110Vt
 );

 math_matrix_matrix_product(
  U,
  3,
  3,
  diag110Vt,
  3,
  3,
  newE
 );

 /*
 Get the Singular Value Decomposition of new E
 (replaces the Singular Value Decomposition of E)
 */

 /* commented out because I have no idea if should be done
 math_singular_value_decomposition(
  3,
  3,
  newE,
  U,
  w,
  V
 );
 */

 /*
 Define matrix W
 */

 W[0*3+0]= 0;
 W[0*3+1]=-1;
 W[0*3+2]= 0;

 W[1*3+0]= 1;
 W[1*3+1]= 0;
 W[1*3+2]= 0;

 W[2*3+0]= 0;
 W[2*3+1]= 0;
 W[2*3+2]= 1;

 /*
 Define matrix Z
 */

 Z[0*3+0]= 0;
 Z[0*3+1]= 1;
 Z[0*3+2]= 0;

 Z[1*3+0]=-1;
 Z[1*3+1]= 0;
 Z[1*3+2]= 0;

 Z[2*3+0]= 0;
 Z[2*3+1]= 0;
 Z[2*3+2]= 0;

 /*
 Compute rotation 1
 R1 = U * W * Vt
 */

 /*
 Compute Vt
 */

 math_matrix_transpose(
  V,
  3,
  3,
  Vt
 );

 /*
 Compute WVt = W * Vt
 */

 math_matrix_matrix_product(
  W,
  3,
  3,
  Vt,
  3,
  3,
  WVt
 );

 /*
 Compute UWVt = U * W * Vt
 */

 math_matrix_matrix_product(
  U,
  3,
  3,
  WVt,
  3,
  3,
  UWVt
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R1[3*i+j]= UWVt[3*i+j]; 
    }
 }

 det= math_matrix3x3_determinant(
  R1
 );

 det1= det;

 if ( det1 < 0. ) {
    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          R1[3*i+j]=-R1[3*i+j];
       }
    }
 }

 /*
 Compute rotation 2
 R2 = U * Wt * Vt 
 */

 /*
 Compute Vt
 */

 math_matrix_transpose(
  V,
  3,
  3,
  Vt
 );

 /*
 Compute Wt
 */

 math_matrix_transpose(
  W,
  3,
  3,
  Wt
 );

 /*
 Compute WtVt = Wt * Vt
 */

 math_matrix_matrix_product(
  Wt,
  3,
  3,
  Vt,
  3,
  3,
  WtVt
 );

 /*
 Compute UWtVt = U * Wt * Vt
 */

 math_matrix_matrix_product(
  U,
  3,
  3,
  WtVt,
  3,
  3,
  UWtVt
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R2[3*i+j]= UWtVt[3*i+j];
    }
 }

 det= math_matrix3x3_determinant(
  R2
 );

 det2= det;

 if ( det2 < 0. ) {
    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          R2[3*i+j]=-R2[3*i+j];
       }
    }
 }

 /*
 Compute the translation (up to sign)
 t = last column of U (the one with index 3-1=2)
 */

 for ( i= 0 ; i< 3 ; i++ )
  t[i]= U[i*3+(3-1)];

 /* 
 See stuff10/CV/CameraGeometry/EssentialMatrixToCameraMatrix.m
 */

 /* turned off
 max_abs_val= 0;
 for ( i= 0 ; i< 3 ; i++ ) {
    abs_val= fabs(U[i*3+(3-1)]);
    if ( abs_val > max_abs_val )
     max_abs_val= abs_val;
 }

 for ( i= 0 ; i< 3 ; i++ )
  t[i]= U[i*3+(3-1)]/max_abs_val;
 */

 /*
 Build the 4 possible camera matrix for the 2nd image
 */

 ind= 0;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P24[ind][i*4+j]= R1[i*3+j];
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P24[ind][i*4+j]= t[i];
 }

 ind= 1;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P24[ind][i*4+j]= R1[i*3+j];
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P24[ind][i*4+j]=-t[i];
 }

 ind= 2;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P24[ind][i*4+j]= R2[i*3+j];
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P24[ind][i*4+j]= t[i];
 }

 ind= 3;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P24[ind][i*4+j]= R2[i*3+j];
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P24[ind][i*4+j]=-t[i];
 }

}
