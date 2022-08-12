#include "header.h"

void sfm_compute_P_from_K_R_t(
 double K[3*3],
 double R[3*3],
 double t[3],
 double P[3*4]
)

/*
Compute P = K[R|t]
*/

{

 double KR[3*3];
 double Kt[3];
 int i;
 int j;

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
       P[i*4+j]= KR[i*3+j];
    }
    j= 3;
    P[i*4+j]= Kt[i];
 }

}
