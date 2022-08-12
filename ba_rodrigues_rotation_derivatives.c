#include "header.h"

void ba_rodrigues_rotation_derivatives(
 double omega3[3],
 double dR[3][3*3]
)

/*
Compute derivatives of R
w/r to rotation vector
dR1[3*3] is the derivative of R
w/r to r1=omega_x
dR2[3*3] is the derivative of R
w/r to r2=omega_y
dR3[3*3] is the derivative of R
w/r to r3=omega_z
*/

{

 double theta2;
 double theta;
 int ind;
 double dtheta3[3];
 double omega3x3[3*3];
 double omega3x3_sq[3*3];
 double domega3x3[3][3*3];
 double domega3x3_sq[3][3*3];
 double num;
 double den;
 double deriv3_1[3];
 double deriv3_2[3];
 int i;
 int j;
 double tol=1.0e-6;

 /*
 Initialize the derivatives to 0
 */

 for ( ind= 0 ; ind< 3 ; ind++ ) {
    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          dR[ind][i*3+j]= 0;
       }
    }
 }

 /*
 Compute theta
 */

 theta2= math_vector_vector_product(
  omega3,
  3,
  omega3,
  3
 );
 theta= sqrt(theta2);

 if ( theta < tol ) {
    return;
 }

 /*
 Compute derivative of theta
 w/r to omega3[ind]
 */

 for ( ind= 0 ; ind< 3 ; ind++ ) {
    dtheta3[ind]= omega3[ind]/theta;
 }

 /*
 Compute omega matrix
 */

 omega3x3[0*3+0]= 0;
 omega3x3[0*3+1]=-omega3[3-1];
 omega3x3[0*3+2]=+omega3[2-1];

 omega3x3[1*3+0]=+omega3[3-1];
 omega3x3[1*3+1]= 0;
 omega3x3[1*3+2]=-omega3[1-1];

 omega3x3[2*3+0]=-omega3[2-1];
 omega3x3[2*3+1]=+omega3[1-1];
 omega3x3[2*3+2]= 0;

 /*
 Compute omega matrix squared
 */

 math_matrix_matrix_product(
  omega3x3,
  3,
  3,
  omega3x3,
  3,
  3,
  omega3x3_sq
 );

 /*
 Compute derivative of omega matrix
 w/r to omega3[ind]
 */

 ind= 0;

 domega3x3[ind][0*3+0]= 0;
 domega3x3[ind][0*3+1]= 0;
 domega3x3[ind][0*3+2]= 0;

 domega3x3[ind][1*3+0]= 0;
 domega3x3[ind][1*3+1]= 0;
 domega3x3[ind][1*3+2]=-1;

 domega3x3[ind][2*3+0]= 0;
 domega3x3[ind][2*3+1]=+1;
 domega3x3[ind][2*3+2]= 0;

 ind= 1;

 domega3x3[ind][0*3+0]= 0;
 domega3x3[ind][0*3+1]= 0;
 domega3x3[ind][0*3+2]=+1;

 domega3x3[ind][1*3+0]= 0;
 domega3x3[ind][1*3+1]= 0;
 domega3x3[ind][1*3+2]= 0;

 domega3x3[ind][2*3+0]=-1;
 domega3x3[ind][2*3+1]= 0;
 domega3x3[ind][2*3+2]= 0;

 ind= 2;

 domega3x3[ind][0*3+0]= 0;
 domega3x3[ind][0*3+1]=-1;
 domega3x3[ind][0*3+2]= 0;

 domega3x3[ind][1*3+0]=+1;
 domega3x3[ind][1*3+1]= 0;
 domega3x3[ind][1*3+2]= 0;

 domega3x3[ind][2*3+0]= 0;
 domega3x3[ind][2*3+1]= 0;
 domega3x3[ind][2*3+2]= 0;

 /*
 Compute derivative of omega matrix squared
 w/r to omega3[ind]
 */

 ind= 0;

 domega3x3_sq[ind][0*3+0]= 0;
 domega3x3_sq[ind][0*3+1]=   omega3[2-1];
 domega3x3_sq[ind][0*3+2]=   omega3[3-1];

 domega3x3_sq[ind][1*3+0]=   omega3[2-1];
 domega3x3_sq[ind][1*3+1]=-2*omega3[1-1];
 domega3x3_sq[ind][1*3+2]= 0;

 domega3x3_sq[ind][2*3+0]=   omega3[3-1];
 domega3x3_sq[ind][2*3+1]= 0;
 domega3x3_sq[ind][2*3+2]=-2*omega3[1-1];

 ind= 1;

 domega3x3_sq[ind][0*3+0]=-2*omega3[2-1];
 domega3x3_sq[ind][0*3+1]=   omega3[1-1];
 domega3x3_sq[ind][0*3+2]= 0;

 domega3x3_sq[ind][1*3+0]=   omega3[1-1];
 domega3x3_sq[ind][1*3+1]= 0;
 domega3x3_sq[ind][1*3+2]=   omega3[3-1];

 domega3x3_sq[ind][2*3+0]= 0;
 domega3x3_sq[ind][2*3+1]=   omega3[3-1];
 domega3x3_sq[ind][2*3+2]=-2*omega3[2-1];

 ind= 2;

 domega3x3_sq[ind][0*3+0]=-2*omega3[3-1];
 domega3x3_sq[ind][0*3+1]= 0;
 domega3x3_sq[ind][0*3+2]=   omega3[1-1];

 domega3x3_sq[ind][1*3+0]= 0;
 domega3x3_sq[ind][1*3+1]=-2*omega3[3-1];
 domega3x3_sq[ind][1*3+2]=   omega3[2-1];

 domega3x3_sq[ind][2*3+0]=   omega3[1-1];
 domega3x3_sq[ind][2*3+1]=   omega3[2-1];
 domega3x3_sq[ind][2*3+2]= 0;

 /*
 Compute derivative of sin(theta)/theta
 w/r to omega3[ind]
 */

 for ( ind= 0 ; ind< 3 ; ind++ ) {
    num= cos(theta)*dtheta3[ind]*theta;
    num-= sin(theta)*dtheta3[ind];
    den= theta2;
    deriv3_1[ind]= num/den;
 }

 /*
 Compute derivative of (1-cos(theta))/theta^2
 w/r to omega3[ind]
 */

 for ( ind= 0 ; ind< 3 ; ind++ ) {
    num= sin(theta)*dtheta3[ind]*theta2;
    num-= (1-cos(theta))*2*theta*dtheta3[ind];
    den= theta2*theta2;
    deriv3_2[ind]= num/den;
 }

 /*
 Compute derivatives of R
 w/r to omega3[ind]
 */

 for ( ind= 0 ; ind< 3 ; ind++ ) {
    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          dR[ind][i*3+j]=
           deriv3_1[ind]*omega3x3[i*3+j]+
           (sin(theta)/theta)*domega3x3[ind][i*3+j];
          dR[ind][i*3+j]+=
           deriv3_2[ind]*omega3x3_sq[i*3+j]+
           ((1-cos(theta))/theta2)*domega3x3_sq[ind][i*3+j];
       }
    }
 }

}
