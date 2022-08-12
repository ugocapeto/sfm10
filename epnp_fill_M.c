#include "header.h"

void epnp_fill_M(
 double fu,
 double fv,
 double uc,
 double vc,
 double *M,
 int row,
 double *as,
 double u,
 double v
)

{

 double *M1;
 double *M2;
 int i;

 M1 = M + row * 12;
 M2 = M1 + 12;

 for ( i= 0 ; i< 4 ; i++ ) {
    M1[3 * i    ] = as[i] * fu;
    M1[3 * i + 1] = 0.0;
    M1[3 * i + 2] = as[i] * (uc - u);

    M2[3 * i    ] = 0.0;
    M2[3 * i + 1] = as[i] * fv;
    M2[3 * i + 2] = as[i] * (vc - v);
 }

}
