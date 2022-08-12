#include "header.h"

void sfm_compute_C_from_R_t(
 double R[3*3],
 double t[3],
 double C[3]
)

/*
Compute C (camera center) from R and t
Since t=-R*C, C=-(R^-1)*t
Since R is a rotation, R^-1=Rt
*/

{

 double Rt[3*3]; 
 double Rtt[3]; 
 int i;

 math_matrix_transpose(
  R,
  3,
  3,
  Rt
 );

 math_matrix_vector_product(
  Rt,
  3,
  3,
  t,
  3,
  Rtt
 );

 for ( i= 0 ; i< 3 ; i++ )
  C[i]=-Rtt[i];

}
