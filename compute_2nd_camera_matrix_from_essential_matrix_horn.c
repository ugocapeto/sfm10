#include "header.h"

void compute_2nd_camera_matrix_from_essential_matrix_horn(
 double E[3*3],
 double P24[4][3*4]
)

/*
This computes the 2nd camera matrix
assuming the 1st camera matrix is [I|0]
These are the camera matrices
up to scale
*/

/*
Implementation of
"Recovering Baseline and Orientation from Essential Matrix"
by Berthold Horn
*/

{

 double Et[3*3];
 double EEt[3*3];
 double trEEt;
 int i;
 int j;
 double I[3*3];
 double bbt[3*3];
 double max_dval;
 int max_i;
 double b[3];
 double e1[3];
 double e2[3];
 double e3[3];
 double dval;
 double A[3*3];
 double e2xe3[3];
 double e3xe1[3];
 double e1xe2[3];
 double cofE[3*3];
 double fac;
 double E22[2*2];
 double detE22;
 double comE[3*3];
 double B[3*3];
 double BE[3*3];
 double bb;
 double R[3*3];
 int ind;

 /*
 Recovering the Baseline
 */

 /*
 Compute EEt
 */

 math_matrix_transpose(
  E,
  3,
  3,
  Et
 );

 math_matrix_matrix_product(
  E,
  3,
  3,
  Et,
  3,
  3,
  EEt
 );

 /*
 Compute trEEt = Trace(EEt)
 */

 trEEt= 0.0;
 for ( i= 0 ; i< 3 ; i++ ) {
    j= i; 
    trEEt+= EEt[3*i+j];
 }

 /*
 Compute bbt
 where b is the baseline aka the relative translation (the vector t in Hartley's book)
 */

 for ( i= 0 ; i< 3 ; i++ ) { 
    for ( j= 0 ; j< 3 ; j++ ) { 
       if ( j == i )
        I[3*i+j]= 1.0;
       else
        I[3*i+j]= 0.0;
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) { 
    for ( j= 0 ; j< 3 ; j++ ) { 
       bbt[3*i+j]= 0.5*trEEt*I[3*i+j]-EEt[3*i+j];
    }
 }

 /*
 Let's find the largest row
 by locating the largest diagonal element (guaranteed to be positive)
 */

 max_dval=-1.0e32;
 max_i=-1;
 for ( i= 0 ; i< 3 ; i++ ) {
    j= i;
    dval= bbt[3*i+j];
    if ( dval > max_dval ) {
       max_dval= dval;
       max_i= i;
    }
 }

 i= max_i;
 dval= max_dval;
 for ( j= 0 ; j< 3 ; j++ )
  b[j]= bbt[3*i+j]/sqrt(dval);

 /*
 The baseline (translation) can be chosen as +b or -b
 */

 /*
 Recovering the orientation
 */

 /*
 Compute cofE = matrix of cofactors of E (aka comatrix of E)
 */

 j= 0;
 for ( i= 0; i< 3 ; i++ )
  e1[i]= E[3*i+j];

 j= 1;
 for ( i= 0; i< 3 ; i++ )
  e2[i]= E[3*i+j];

 j= 2;
 for ( i= 0; i< 3 ; i++ )
  e3[i]= E[3*i+j];

 /*
 Compute e2 x e3
 */

 math_cross_product_matrix(
  e2,
  A
 );

 math_matrix_vector_product(
  A,
  3,
  3,
  e3,
  3,
  e2xe3 
 );

 /*
 Compute e3 x e1
 */

 math_cross_product_matrix(
  e3,
  A
 );

 math_matrix_vector_product(
  A,
  3,
  3,
  e1,
  3,
  e3xe1
 );

 /*
 Compute e1 x e2
 */

 math_cross_product_matrix(
  e1,
  A
 );

 math_matrix_vector_product(
  A,
  3,
  3,
  e2,
  3,
  e1xe2
 );

 j= 0;
 for ( i = 0 ; i< 3 ; i++ )
  cofE[3*i+j]= e2xe3[i];

 j= 1;
 for ( i = 0 ; i< 3 ; i++ )
  cofE[3*i+j]= e3xe1[i];

 j= 2;
 for ( i = 0 ; i< 3 ; i++ )
  cofE[3*i+j]= e1xe2[i];

 /*
 Alternative computation of the comatrix of E
 */

 fac= 1.0;
 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {

       /*
       Remove row i and column j from A
       */

       math_matrix_remove_row_col(
        E,
        3,
        3,
        i,
        j,
        E22
       );

       /*
       Compute the determinant of E22
       */

       detE22= math_matrix2x2_determinant(
        E22
       );

       comE[i*3+j]= fac*detE22;
       fac*= -1.0;
    }
 }

 /*
 comE and cofE should be the same!
 */

 /*
 Let's compute R corresponding to +b
 */

 /*
 Compute B
 */

 math_cross_product_matrix(
  b,
  B
 );

 /*
 Compute BE
 */

 math_matrix_matrix_product(
  B,
  3,
  3,
  E,
  3,
  3,
  BE
 );

 /*
 Compute bb=b.b
 */

 bb= math_vector_vector_product(
  b,
  3,
  b,
  3
 );

 /*
 Compute R
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[3*i+j]= cofE[3*i+j]-BE[3*i+j];
       R[3*i+j]/= bb;
    }
 }

 /*
 Store 2 possible camera matrix
 */

 ind= 0;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P24[ind][i*4+j]= R[i*3+j];
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P24[ind][i*4+j]=+b[i];
 }

 ind= 1;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P24[ind][i*4+j]= R[i*3+j];
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P24[ind][i*4+j]=-b[i];
 }

 /*
 Go from +b to -b
 */

 for ( i= 0 ; i< 3 ; i++ )
  b[i]=-b[i];

 /*
 Let's compute R corresponding to +b
 */

 /*
 Compute B
 */

 math_cross_product_matrix(
  b,
  B
 );

 /*
 Compute BE
 */

 math_matrix_matrix_product(
  B,
  3,
  3,
  E,
  3,
  3,
  BE
 );

 /*
 Compute bb=b.b
 */

 bb= math_vector_vector_product(
  b,
  3,
  b,
  3
 );

 /*
 Compute R
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[3*i+j]= cofE[3*i+j]-BE[3*i+j];
       R[3*i+j]/= bb;
    }
 }

 /*
 Store 2 more possible camera matrix
 */

 ind= 2+0;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P24[ind][i*4+j]= R[i*3+j];
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P24[ind][i*4+j]=+b[i];
 }

 ind= 2+1;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P24[ind][i*4+j]= R[i*3+j];
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P24[ind][i*4+j]=-b[i];
 }

}
