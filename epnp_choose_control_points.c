#include "header.h"

void epnp_choose_control_points(
 double *pws,
 int number_of_correspondences,
 double cws[4][3]
)

{

 double PW0tPW0[3*3];
 double svd_U[3*3];
 double svd_w[3];
 double svd_V[3*3];
 double DC[3];
 double UCt[3*3];
 double dval;
 int j;
 int i;
 double *PW0;
 double *PW0t;

 /*
 Take C0 as the reference points centroid
 */

 cws[0][0] = cws[0][1] = cws[0][2] = 0;

 for ( i= 0 ; i< number_of_correspondences ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       cws[0][j] += pws[3 * i + j]; 
    }
 }

 for ( j= 0 ; j< 3 ; j++ )
  cws[0][j]/= (double)number_of_correspondences;

 /*
 Take C1, C2, and C3 from PCA on the reference points
 */

 PW0= (double *)calloc(number_of_correspondences,3*sizeof(double));

 for ( i= 0 ; i< number_of_correspondences ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       PW0[3 * i + j] = pws[3 * i + j] - cws[0][j];
    }
 }

 PW0t= (double *)calloc(3,number_of_correspondences*sizeof(double));

 math_matrix_transpose(
  PW0,
  number_of_correspondences,
  3,
  PW0t 
 );

 math_matrix_matrix_product(
  PW0t,
  3,
  number_of_correspondences,
  PW0,
  number_of_correspondences,
  3,
  PW0tPW0 
 );

 /*
 Get the Singular Value Decomposition of PW0tPW0
 */

 math_singular_value_decomposition(
  3,
  3,
  PW0tPW0,
  svd_U,
  svd_w,
  svd_V
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    DC[i]= svd_w[i];
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       UCt[3 * i + j]= svd_U[3 * j + i];
    }
 }

 for ( i= 1 ; i< 4 ; i++ ) {
    dval= sqrt(DC[i - 1]/(double)number_of_correspondences);
    for ( j= 0 ; j< 3 ; j++ ) {
       cws[i][j] = cws[0][j] + dval * UCt[3 * (i - 1) + j];
    }
 }

 /*
 Free PW0
 */

 if ( number_of_correspondences > 0 )
  free(PW0);

 /*
 Free PW0t
 */

 if ( number_of_correspondences > 0 )
  free(PW0t);

}
