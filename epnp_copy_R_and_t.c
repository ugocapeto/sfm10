#include "header.h"

void epnp_copy_R_and_t(
 double R_src[3][3],
 double t_src[3],
 double R_dst[3][3],
 double t_dst[3]
)

{

 int i;
 int j;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ )
      R_dst[i][j] = R_src[i][j];
    t_dst[i] = t_src[i];
 }

}
