#include "header.h"

void sfm_compute_focal_length(
 double F[3*3],
 int width,
 int height,
 double *pfocal_length
)

/*
Compute the focal length (assumed to be same for both cameras)
using the method described in
"On Focal Length Calibration from Two Views"
by Peter Sturm
*/

/*
The two images are assumed to be of the same width and height
In the paper, they do not have to be
*/

{

 double U[3*3];
 double w[3];
 double V[3*3];
 double u0;
 double v0;
 double K[3*3];
 double Kprime[3*3];
 double Kprimet[3*3];
 double FK[3*3];
 double G[3*3];
 double f0;
 double diagf0[3*3];
 double Gdiagf0[3*3];
 double Gprime[3*3];
 double w1;
 double w2;
 double u13;
 double u23;
 double v13;
 double v23;
 int i;
 int j;
 double a;
 double b;
 double c;
 double delta;
 double f12;
 double f1;
 double f22;
 double f2;
 double focal_length;
 double norm;
 double norm2;
 int err_flag1;
 int err_flag2;

 focal_length= 1.0e32;

 /*
 Undo the known intrinsic parameters
 */

 u0= (double)width/2.;
 v0= (double)height/2.;

 K[3*0+0]= 1.;
 K[3*0+1]= 0.;
 K[3*0+2]= u0;
 K[3*1+0]= 0.;
 K[3*1+1]= 1.;
 K[3*1+2]= v0;
 K[3*2+0]= 0.;
 K[3*2+1]= 0.;
 K[3*2+2]= 1.;

 Kprime[3*0+0]= 1.;
 Kprime[3*0+1]= 0.;
 Kprime[3*0+2]= u0;
 Kprime[3*1+0]= 0.;
 Kprime[3*1+1]= 1.;
 Kprime[3*1+2]= v0;
 Kprime[3*2+0]= 0.;
 Kprime[3*2+1]= 0.;
 Kprime[3*2+2]= 1.;

 math_matrix_transpose(
  Kprime,
  3,
  3,
  Kprimet
 );

 math_matrix_matrix_product(
  F,
  3,
  3,
  K,
  3,
  3,
  FK
 );

 math_matrix_matrix_product(
  Kprimet,
  3,
  3,
  FK,
  3,
  3,
  G
 );

 /*
 Also undo a typical value f0 for the focal length
 */

 f0= 100000.;

 diagf0[3*0+0]= f0;
 diagf0[3*0+1]= 0.;
 diagf0[3*0+2]= 0.;
 diagf0[3*1+0]= 0.;
 diagf0[3*1+1]= f0;
 diagf0[3*1+2]= 0.;
 diagf0[3*2+0]= 0.;
 diagf0[3*2+1]= 0.;
 diagf0[3*2+2]= 1.;

 math_matrix_matrix_product(
  G,
  3,
  3,
  diagf0,
  3,
  3,
  Gdiagf0
 );

 math_matrix_matrix_product(
  diagf0,
  3,
  3,
  Gdiagf0,
  3,
  3,
  Gprime
 );

 /*
 Scale G' to unit Frobenius norm
 */

 norm2= 0.;
 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       norm2+= Gprime[3*i+j]*Gprime[3*i+j];
    }
 }
 norm= sqrt(norm2);

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       Gprime[3*i+j]/= norm;
    }
 }

 /*
 Compute the SVD of G'
 */

 math_singular_value_decomposition(
  3,
  3,
  Gprime,
  U,
  w,
  V
 );

 w1= w[1-1];
 w2= w[2-1];

 j= 1-1;
 i= 3-1;
 u13= U[3*i+j];
 v13= V[3*i+j];

 j= 2-1;
 i= 3-1;
 u23= U[3*i+j];
 v23= V[3*i+j];

 /*
 Construct and solve linear equation in f^2
 */

 /*
 b = w1*u13*u23*(1.-v13*v13);
 b+= w2*v13*v23*(1.-u23*u23);

 c = u23*v13*(w1*u13*v13+w2*u23*v23);

 f12= -c/b;
 f1= sqrt(f12);
 f1*= f0;
 */

 /*
 Construct and solve linear equation in f^2
 */

 /*
 b = w1*v13*v23*(1.-u13*u13);
 b+= w2*u13*u23*(1.-v23*v23);

 c = u13*v23*(w1*u13*v13+w2*u23*v23);

 f12= -c/b;
 f1= sqrt(f12);
 f1*= f0;
 */

 /*
 Construct and solve the quadratic equation in f^2
 */

 a = w1*w1*(1.-u13*u13)*(1.-v13*v13);
 a-= w2*w2*(1.-u23*u23)*(1.-v23*v23);

 b = w1*w1*(u13*u13+v13*v13-2.*u13*u13*v13*v13); 
 b-= w2*w2*(u23*u23+v23*v23-2.*u23*u23*v23*v23); 

 c = w1*w1*u13*u13*v13*v13;
 c-= w2*w2*u23*u23*v23*v23;

 delta= b*b-4.*a*c;

 if ( !(delta >= 0.) ) {
    goto END;
 }

 f12= (-b+sqrt(delta))/(2.*a);
 err_flag1= 1;
 if ( f12 >= 0. ) {
    f1= sqrt(f12);
    f1*= f0;
    err_flag1= 0;
 }
 f22= (-b-sqrt(delta))/(2.*a);
 err_flag2= 1;
 if ( f22 >= 0. ) {
    f2= sqrt(f22);
    f2*= f0;
    err_flag2= 0;
 }

 if ( err_flag1 == 1 && err_flag2 == 1 ) {
    goto END;
 }

 /*
 Take the solution closest to f0
 */

 if ( err_flag1 == 0 && fabs(f1-f0) < fabs(focal_length-f0) )
  focal_length= f1;
 if ( err_flag2 == 0 && fabs(f2-f0) < fabs(focal_length-f0) )
  focal_length= f2;

 END:

 (*pfocal_length)= focal_length;

}
