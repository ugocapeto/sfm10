#include "header.h"

void epnp_compute_ccs(
 double *betas,
 double *ut,
 double ccs[4][3]
)

{

 int i;
 double *v;
 int j;
 int k;

 for ( i= 0 ; i< 4 ; i++ ) {
    ccs[i][0] = 0.0f;
    ccs[i][1] = 0.0f;
    ccs[i][2] = 0.0f;
 }

 for ( i= 0 ; i< 4 ; i++ ) {
    v = ut + 12 * (11 - i);
    for ( j= 0 ; j< 4 ; j++ ) {
       for ( k= 0 ; k< 3 ; k++ )
         ccs[j][k] += betas[i] * v[3 * j + k];
    }
 }

}
