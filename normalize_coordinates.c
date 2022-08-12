#include "header.h"

void normalize_coordinates(
 double K1[3*3],
 double K2[3*3],
 match_struct *match_arr,
 int match_nbr
)

{

 double K1_inv[3*3];
 double K2_inv[3*3];
 int match_ind;
 double x1;
 double y1;
 double x2;
 double y2;
 double xvec1[3];
 double xvec1_normalized[3];
 double xvec2[3];
 double xvec2_normalized[3];
 double x1_normalized;
 double y1_normalized;
 double x2_normalized;
 double y2_normalized;

 /*
 Compute the inverse of K1
 */

 math_lu_decomposition_inverse(
  3,
  K1,
  K1_inv 
 );

 /*
 Compute the inverse of K2
 */

 math_lu_decomposition_inverse(
  3,
  K2,
  K2_inv
 );

 for ( match_ind= 0 ; match_ind< match_nbr ; match_ind++ ) {
    x1= match_arr[match_ind].x1;
    y1= match_arr[match_ind].y1;
    x2= match_arr[match_ind].x2;
    y2= match_arr[match_ind].y2;

    /*
    Normalize (x1,y1)
    */

    xvec1[0]= x1;
    xvec1[1]= y1;
    xvec1[2]= 1;

    math_matrix_vector_product(
     K1_inv,
     3,
     3,
     xvec1,
     3,
     xvec1_normalized 
    );

    x1_normalized= xvec1_normalized[0]/xvec1_normalized[2];
    y1_normalized= xvec1_normalized[1]/xvec1_normalized[2];

    /*
    Normalize (x2,y2)
    */

    xvec2[0]= x2;
    xvec2[1]= y2;
    xvec2[2]= 1;

    math_matrix_vector_product(
     K2_inv,
     3,
     3,
     xvec2,
     3,
     xvec2_normalized
    );
  
    x2_normalized= xvec2_normalized[0]/xvec2_normalized[2];
    y2_normalized= xvec2_normalized[1]/xvec2_normalized[2];

    match_arr[match_ind].x1= x1_normalized;
    match_arr[match_ind].y1= y1_normalized;
    match_arr[match_ind].x2= x2_normalized;
    match_arr[match_ind].y2= y2_normalized;
 }

}
