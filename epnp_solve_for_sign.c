#include "header.h"

void epnp_solve_for_sign(
 double *pcs,
 int number_of_correspondences,
 double ccs[4][3]
)

{

 int i;
 int j;

 if (pcs[2] < 0.0) {
    for ( i= 0 ; i< 4 ; i++ ) {
      for ( j= 0 ; j< 3 ; j++ )
       ccs[i][j] = -ccs[i][j];
    }

    for ( i= 0 ; i< number_of_correspondences ; i++ ) {
      pcs[3 * i    ] = -pcs[3 * i];
      pcs[3 * i + 1] = -pcs[3 * i + 1];
      pcs[3 * i + 2] = -pcs[3 * i + 2];
    }
 }

}
