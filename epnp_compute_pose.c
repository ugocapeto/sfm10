#include "header.h"

double epnp_compute_pose(
 double fu,
 double fv,
 double uc,
 double vc,
 double *pws,
 double *us,
 int number_of_correspondences,
 double R[3][3],
 double t[3]
)

/*
This is an implementation of:
"EPnP: An Accurate O(n) Solution to the PnP Problem" by
Vincent Lepetit, Francesc Moreno-Noguer, Pascal Fua
*/

/*
fu,fv = focal length
uc,vc = principal point coords
pws = 3D points
us = corresponding 2D image points
*/

{

 double *alphas;
 double *pcs;
 double cws[4][3];
 double *M;
 int i;
 double *Mt;
 double MtM[12*12];
 double svd_U[12*12];
 double svd_w[12];
 double svd_V[12*12];
 double D[12];
 double Ut[12*12];
 double L_6x10[6*10];
 double Rho[6];
 double Betas[4][4];
 double rep_errors[4];
 double Rs[4][3][3];
 double ts[4][3];
 int N;
 int j;

 /*
 Need at least 4 correspondences!
 */

 if ( !(number_of_correspondences >= 4) ) { 
    sfm10_error_handler("epnp_compute_pose");
 }

 /*
 Allocate memory for alphas
 */

 alphas= (double *)calloc(4 * number_of_correspondences,sizeof(double));

 /*
 Allocate memory for pcs
 */

 pcs= (double *)calloc(3 * number_of_correspondences,sizeof(double));

 /*
 Compute the control points
 */

 epnp_choose_control_points(
  pws,
  number_of_correspondences,
  cws
 );

 /*
 Compute barycentric coordinates
 */

 epnp_compute_barycentric_coordinates(
  pws,
  alphas,
  number_of_correspondences,
  cws
 );

 /*
 Allocate memory for M
 number of rows = 2 * number_of_correspondences
 number of cols = 12
 */

 M= (double *)calloc(2 * number_of_correspondences,12*sizeof(double));

 /*
 Fill M
 */

 for ( i= 0 ; i< number_of_correspondences ; i++ ) {
    epnp_fill_M(
     fu,
     fv,
     uc,
     vc,
     M,
     2 * i,
     alphas + 4 * i,
     us[2 * i],
     us[2 * i + 1] 
    );
 }

 /*
 Allocate memory for Mt
 number of rows = 12
 number of cols = 2 * number_of_correspondences
 */

 Mt= (double *)calloc(12,2 * number_of_correspondences*sizeof(double));

 math_matrix_transpose(
  M,
  2*number_of_correspondences,
  12,
  Mt
 );

 math_matrix_matrix_product(
  Mt,
  12,
  2*number_of_correspondences,
  M,
  2*number_of_correspondences,
  12,
  MtM
 );

 /*
 Get the Singular Value Decomposition of MtM
 */

 math_singular_value_decomposition(
  12,
  12,
  MtM,
  svd_U,
  svd_w,
  svd_V
 );

 fprintf(stdout,"MtM singular values:\n");
 for ( i= 0 ; i< 12 ; i++ ) {
    fprintf(stdout," %g",svd_w[i]);
 }
 fprintf(stdout,"\n");

 for ( i= 0 ; i< 12 ; i++ ) {
    D[i]= svd_w[i];
 }

 for ( i= 0 ; i< 12 ; i++ ) {
    for ( j= 0 ; j< 12 ; j++ ) {
       Ut[12 * i + j]= svd_U[12 * j + i];
    }
 }

 epnp_compute_L_6x10(
  Ut,
  L_6x10
 );

 epnp_compute_rho(
  cws,
  Rho
 );

 epnp_find_betas_approx_1(
  L_6x10,
  Rho,
  Betas[1]
 );

 epnp_gauss_newton(
  L_6x10,
  Rho,
  Betas[1]
 );

 rep_errors[1] = epnp_compute_R_and_t(
  fu,
  fv,
  uc,
  vc,
  pws,
  us,
  alphas,
  pcs,
  number_of_correspondences,
  Ut,
  Betas[1],
  Rs[1],
  ts[1]
 );

 fprintf(stdout,"rotation for null space of dimension 1:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       fprintf(stdout," %g",Rs[1][i][j]);
    }
 }
 fprintf(stdout,"\n");
 fprintf(stdout,"translation for null space of dimension 1:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    fprintf(stdout," %g",ts[1][i]);
 }
 fprintf(stdout,"\n");
 fprintf(stdout,"reprojection error for null space of dimension 1 = %g\n",
  rep_errors[1]);

 epnp_find_betas_approx_2(
  L_6x10,
  Rho,
  Betas[2]
 );

 epnp_gauss_newton(
  L_6x10,
  Rho,
  Betas[2]
 );

 rep_errors[2] = epnp_compute_R_and_t(
  fu,
  fv,
  uc,
  vc,
  pws,
  us,
  alphas,
  pcs,
  number_of_correspondences,
  Ut,
  Betas[2],
  Rs[2],
  ts[2]
 );

 fprintf(stdout,"rotation for null space of dimension 2:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       fprintf(stdout," %g",Rs[2][i][j]);
    }
 }
 fprintf(stdout,"\n");
 fprintf(stdout,"translation for null space of dimension 2:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    fprintf(stdout," %g",ts[2][i]);
 }
 fprintf(stdout,"\n");
 fprintf(stdout,"reprojection error for null space of dimension 2 = %g\n",
  rep_errors[2]);

 epnp_find_betas_approx_3(
  L_6x10,
  Rho,
  Betas[3]
 );

 epnp_gauss_newton(
  L_6x10,
  Rho,
  Betas[3]
 );
  
 rep_errors[3] = epnp_compute_R_and_t(
  fu,
  fv,
  uc,
  vc,
  pws,
  us,
  alphas,
  pcs,
  number_of_correspondences,
  Ut,
  Betas[3],
  Rs[3],
  ts[3]
 );

 fprintf(stdout,"rotation for null space of dimension 3:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       fprintf(stdout," %g",Rs[3][i][j]);
    }
 }
 fprintf(stdout,"\n");
 fprintf(stdout,"translation for null space of dimension 3:\n");
 for ( i= 0 ; i< 3 ; i++ ) {
    fprintf(stdout," %g",ts[3][i]);
 }
 fprintf(stdout,"\n");
 fprintf(stdout,"reprojection error for null space of dimension 3 = %g\n",
  rep_errors[3]);

 N = 1;
 if (rep_errors[2] < rep_errors[1]) N = 2;
 if (rep_errors[3] < rep_errors[N]) N = 3;

 fprintf(stdout,"lowest reprojection error null space dimension = %d\n",N);

 epnp_copy_R_and_t(
  Rs[N],
  ts[N],
  R,
  t
 );

 /*
 Free alphas
 */

 if ( number_of_correspondences > 0 )
  free(alphas);

 /*
 Free pcs
 */

 if ( number_of_correspondences > 0 )
  free(pcs);

 /*
 Free M
 */

 if ( number_of_correspondences > 0 )
  free(M);

 /*
 Free Mt
 */

 if ( number_of_correspondences > 0 )
  free(Mt);

 /*
 Return the reprojection error
 */

 return rep_errors[N];

}
