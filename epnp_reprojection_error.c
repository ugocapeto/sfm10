#include "header.h"

double epnp_reprojection_error(
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

{

 double sum2;
 double *pw;
 double Xc;
 double Yc;
 double inv_Zc;
 double ue;
 double ve;
 double u;
 double v;
 int i;

  sum2= 0.0;
  for ( i= 0 ; i< number_of_correspondences ; i++ ) {
    pw = pws + 3 * i;
    Xc = epnp_dot(R[0], pw) + t[0];
    Yc = epnp_dot(R[1], pw) + t[1];
    inv_Zc = 1.0 / (epnp_dot(R[2], pw) + t[2]);
    ue = uc + fu * Xc * inv_Zc;
    ve = vc + fv * Yc * inv_Zc;
    u = us[2 * i];
    v = us[2 * i + 1];

    sum2 += sqrt( (u - ue) * (u - ue) + (v - ve) * (v - ve) );
  }

  return sum2 / (double)number_of_correspondences;

}
