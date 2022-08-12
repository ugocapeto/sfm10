#include "header.h"

double epnp_compute_R_and_t(
 double fu,
 double fv,
 double uc,
 double vc,
 double *pws,
 double *us,
 double *alphas,
 double *pcs,
 int number_of_correspondences,
 double *ut,
 double *betas,
 double R[3][3],
 double t[3]
)

{

 double dval;
 double ccs[4][3];

 epnp_compute_ccs(
  betas,
  ut,
  ccs
 );

 epnp_compute_pcs(
  alphas,
  pcs,
  number_of_correspondences,
  ccs
 );

 epnp_solve_for_sign(
  pcs,
  number_of_correspondences,
  ccs
 );

 epnp_estimate_R_and_t(
  pws,
  pcs,
  number_of_correspondences,
  R,
  t 
 );

 dval= epnp_reprojection_error(
  fu,
  fv,
  uc,
  vc,
  pws,
  us,
  number_of_correspondences,
  R,
  t
 );

 return dval;

}
