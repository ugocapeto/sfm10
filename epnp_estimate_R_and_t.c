#include "header.h"

void epnp_estimate_R_and_t(
 double *pws,
 double *pcs,
 int number_of_correspondences,
 double R[3][3],
 double t[3]
)

{

 double pc0[3];
 double pw0[3];
 int i;
 double *pc;
 double *pw;
 int j;
 double ABt[3 * 3];
 double ABt_D[3];
 double ABt_U[3 * 3];
 double ABt_V[3 * 3];
 double svd_U[3*3];
 double svd_w[3];
 double svd_V[3*3];
 double det;

  pc0[0] = pc0[1] = pc0[2] = 0.0;
  pw0[0] = pw0[1] = pw0[2] = 0.0;

  for ( i= 0 ; i< number_of_correspondences ; i++ ) {
    pc = pcs + 3 * i;
    pw = pws + 3 * i;

    for ( j= 0 ; j< 3 ; j++ ) {
      pc0[j] += pc[j];
      pw0[j] += pw[j];
    }
  }
  for ( j= 0 ; j< 3 ; j++ ) {
    pc0[j] /= (double)number_of_correspondences;
    pw0[j] /= (double)number_of_correspondences;
  }

  for ( i= 0 ; i< 3 ; i++ ) {
     for ( j= 0 ; j< 3 ; j++ ) {
        ABt[i*3+j]= 0;
     }
  }

  for ( i= 0 ; i< number_of_correspondences ; i++ ) {
    pc = pcs + 3 * i;
    pw = pws + 3 * i;

    for ( j= 0 ; j< 3 ; j++ ) {
      ABt[3 * j    ] += (pc[j] - pc0[j]) * (pw[0] - pw0[0]);
      ABt[3 * j + 1] += (pc[j] - pc0[j]) * (pw[1] - pw0[1]);
      ABt[3 * j + 2] += (pc[j] - pc0[j]) * (pw[2] - pw0[2]);
    }
  }

 /*
 Get the Singular Value Decomposition of ABt
 */

 math_singular_value_decomposition(
  3,
  3,
  ABt,
  svd_U,
  svd_w,
  svd_V
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    ABt_D[i]= svd_w[i];
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       ABt_U[3 * i + j]= svd_U[3 * i + j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       ABt_V[3 * i + j]= svd_V[3 * i + j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[i][j] = epnp_dot(ABt_U + 3 * i, ABt_V + 3 * j);
    }
 }

 det =
    R[0][0] * R[1][1] * R[2][2] +
    R[0][1] * R[1][2] * R[2][0] +
    R[0][2] * R[1][0] * R[2][1] -
    R[0][2] * R[1][1] * R[2][0] -
    R[0][1] * R[1][0] * R[2][2] -
    R[0][0] * R[1][2] * R[2][1];

  if (det < 0) {
    R[2][0] = -R[2][0];
    R[2][1] = -R[2][1];
    R[2][2] = -R[2][2];
  }

  t[0] = pc0[0] - epnp_dot(R[0], pw0);
  t[1] = pc0[1] - epnp_dot(R[1], pw0);
  t[2] = pc0[2] - epnp_dot(R[2], pw0);

}
