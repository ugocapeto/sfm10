#include "header.h"

void epnp_compute_L_6x10(
 double *ut,
 double *l_6x10
)

{

 double *v[4];
 double dv[4][6][3];
 int i;
 int a;
 int b;
 int j;
 double *row;

 v[0] = ut + 12 * 11;
 v[1] = ut + 12 * 10;
 v[2] = ut + 12 *  9;
 v[3] = ut + 12 *  8;

 for ( i= 0 ; i< 4 ; i++ ) {
    a = 0;
    b = 1;
    for ( j= 0 ; j< 6 ; j++ ) {
      dv[i][j][0] = v[i][3 * a    ] - v[i][3 * b];
      dv[i][j][1] = v[i][3 * a + 1] - v[i][3 * b + 1];
      dv[i][j][2] = v[i][3 * a + 2] - v[i][3 * b + 2];

      b++;
      if ( b > 3 ) {
        a++;
        b = a + 1;
      }
    }
 }

 for ( i= 0 ; i< 6 ; i++ ) {
    row = l_6x10 + 10 * i;

    row[0] =        epnp_dot(dv[0][i], dv[0][i]);
    row[1] = 2.0f * epnp_dot(dv[0][i], dv[1][i]);
    row[2] =        epnp_dot(dv[1][i], dv[1][i]);
    row[3] = 2.0f * epnp_dot(dv[0][i], dv[2][i]);
    row[4] = 2.0f * epnp_dot(dv[1][i], dv[2][i]);
    row[5] =        epnp_dot(dv[2][i], dv[2][i]);
    row[6] = 2.0f * epnp_dot(dv[0][i], dv[3][i]);
    row[7] = 2.0f * epnp_dot(dv[1][i], dv[3][i]);
    row[8] = 2.0f * epnp_dot(dv[2][i], dv[3][i]);
    row[9] =        epnp_dot(dv[3][i], dv[3][i]);
 }

}
