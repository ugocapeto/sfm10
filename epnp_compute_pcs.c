#include "header.h"

void epnp_compute_pcs(
 double *alphas,
 double *pcs,
 int number_of_correspondences,
 double ccs[4][3]
)

{

 int i;
 double *a;
 double *pc;
 int j;

 for ( i= 0 ; i< number_of_correspondences ; i++ ) {
    a = alphas + 4 * i;
    pc = pcs + 3 * i;

    for ( j= 0 ; j< 3 ; j++ )
      pc[j] = a[0] * ccs[0][j] + a[1] * ccs[1][j] + a[2] * ccs[2][j] + a[3] * ccs[3][j];
 }

}
