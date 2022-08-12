#include "header.h"

double compute_3dpoint_depth(
 double X[4],
 double P[3*4],
 double K[3*3],
 double R[3*3],
 double C[3]
)

/*
P is of the form [M | p4]
where M is a 3x3 sub-matrix and p4 is 3x1 column vector
*/

{

 int i;
 int j;
 double M[3*3];
 double det;
 double w;
 double norm;
 int sign;
 double depth;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       M[i*3+j]= P[i*4+j];
    }
 }

 /*
 Compute the determinant of M
 */

 /* 
 If everything up to snuff,
 should be either +1 or -1
 */

 det= math_matrix3x3_determinant(
  M
 );

 /*
 Get the sign of the determinant
 */

 if ( det >= 0 ) {
    sign=+1;
 }
 else {
    sign=-1;
 }

 /*
 Compute w
 w = M3t * (X-C)
 where M3t is the third row of M
 */

 w= 0;
 for ( i= 0 ; i< 3 ; i++ ) {
    j= i;
    w+= M[(3-1)*3+j]*(X[i]-C[i]);
 }

 /*
 Compute the norm of M3t
 */

 /*
 If everything up to snuff,
 should be 1
 */

 norm= 0;
 for ( j= 0 ; j< 3 ; j++ )
  norm+= M[(3-1)*3+j]*M[(3-1)*3+j];
 norm= sqrt(norm);

 /*
 Compute the depth
 */

 depth= ( (double)sign*w ) / (X[4-1]*norm);

 return depth;

}
