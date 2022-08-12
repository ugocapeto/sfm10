#include "header.h"

void decompose_camera_matrix(
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double C[3]
)

/*
Decompose the camera matrix P into K, R, and C
P = K[R | -RC]
K is the 3x3 calibration matrix
R is the 3x3 rotation matrix
C is the 3x1 camera coordinates
*/

{

 double X; 
 double Y; 
 double Z; 
 double T; 
 double det;
 int i;
 int j;
 double A[3*3];
 double RC[3];
 double t[3];
 double KR[3*3];
 double Kt[3];
 double P_check[3*4];

 /*
 Compute the camera center
 */

 /*
 Compute X
 */

 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+0]= P[i*4+(2-1)];
 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+1]= P[i*4+(3-1)];
 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+2]= P[i*4+(4-1)];

 det= math_matrix3x3_determinant(
  A 
 );

 X= det;

 /*
 Compute Y
 */

 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+0]= P[i*4+(1-1)];
 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+1]= P[i*4+(3-1)];
 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+2]= P[i*4+(4-1)];

 det= math_matrix3x3_determinant(
  A
 );

 Y=-det;

 /*
 Compute Z
 */

 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+0]= P[i*4+(1-1)];
 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+1]= P[i*4+(2-1)];
 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+2]= P[i*4+(4-1)];

 det= math_matrix3x3_determinant(
  A
 );

 Z= det;

 /*
 Compute T
 */

 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+0]= P[i*4+(1-1)];
 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+1]= P[i*4+(2-1)];
 for ( i= 0 ; i< 3 ; i++ )
  A[i*3+2]= P[i*4+(3-1)];

 det= math_matrix3x3_determinant(
  A
 );

 T=-det;

 C[0]= X/T;
 C[1]= Y/T;
 C[2]= Z/T;

 /*
 Find rotation and calibration matrix
 P = [M | -MC] = K[R | -RC]
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       A[i*3+j]= P[i*4+j];
    }
 }

 compute_RQ_decomposition(
  A,
  K,
  R
 );

 /*
 If all went well,
 the determinant of the rotation matrix should be +1
 */

 det= math_matrix3x3_determinant(
  R
 );

 /*
 Check that P_check=K[R|t] (t=-RC) is equal to P
 It's possible R got negated because of det=-1,
 in that case, P_check is equal to -1*P
 */

 math_matrix_vector_product(
  R,
  3,
  3,
  C,
  3,
  RC 
 );

 for ( i= 0 ; i< 3 ; i++ )
  t[i]=-RC[i];

 math_matrix_matrix_product(
  K,
  3,
  3,
  R,
  3,
  3,
  KR 
 );

 math_matrix_vector_product(
  K,
  3,
  3,
  t,
  3,
  Kt
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P_check[i*4+j]= KR[i*3+j];
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P_check[i*4+j]= Kt[i];
 }

}
