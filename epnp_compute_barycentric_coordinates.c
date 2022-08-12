#include "header.h"

void epnp_compute_barycentric_coordinates(
 double *pws,
 double *alphas,
 int number_of_correspondences,
 double cws[4][3]
)

{

 double CC[3*3];
 double CC_inv[3*3];
 double *ci;
 double *pi;
 double *a;
 int i;
 int j;

 for ( i= 0 ; i< 3 ; i++ ) {
    for( j= 1 ; j< 4 ; j++ ) {
       CC[3 * i + j - 1] = cws[j][i] - cws[0][i];
    }
 }

 math_matrix3x3_inverse(
  CC,
  CC_inv 
 );

 ci = CC_inv;

 for ( i= 0 ; i< number_of_correspondences ; i++ ) {
    pi = pws + 3 * i;
    a = alphas + 4 * i;

    for ( j= 0 ; j< 3 ; j++ ) {
       a[1 + j] =
        ci[3 * j    ] * (pi[0] - cws[0][0]) +
        ci[3 * j + 1] * (pi[1] - cws[0][1]) +
        ci[3 * j + 2] * (pi[2] - cws[0][2]);
    }
    a[0] = 1.0f - a[1] - a[2] - a[3];
 }

}
