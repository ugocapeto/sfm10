#include "header.h"

void compute_linear_triangulation(
 double *P_arr,
 double *x_arr,
 double *y_arr,
 int imagepoint_nbr,
 double X[4]
)

/*
Compute location of 3D point X in projective space (homogeneous 4-vector)
given the camera matrices and the matching locations on the image planes
*/

/*
Here, as opposed to compute_linear_triangulation_two_cameras(),
there is no limit on the number of cameras involved
*/

{

 double *A;
 int i;
 int j;
 double *U;
 double *w;
 double *V;
 int nrows;
 int ncols;
 int imagepoint_ind;
 double P[3*4];
 double x;
 double y;

 nrows= 2*imagepoint_nbr;
 ncols= 4;

 A= (double *)calloc(nrows*ncols,sizeof(double));

 /*
 Compute A (A*X=0)
 */

 for ( imagepoint_ind= 0 ;
       imagepoint_ind< imagepoint_nbr ;
       imagepoint_ind++ ) {

    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 4 ; j++ ) {
          P[i*4+j]= P_arr[imagepoint_ind*(3*4)+i*4+j];
       }
    }

    x= x_arr[imagepoint_ind];
    y= y_arr[imagepoint_ind];

    i= 2*imagepoint_ind+0;
    for ( j= 0 ; j< 4 ; j++ )
     A[i*4+j]= x*P[(3-1)*4+j] - P[(1-1)*4+j];

    i= 2*imagepoint_ind+1;
    for ( j= 0 ; j< 4 ; j++ )
     A[i*4+j]= y*P[(3-1)*4+j] - P[(2-1)*4+j];
 }

 /*
 Perform the Singular Value Decomposition of A
 */

 U= (double *)calloc(nrows*ncols,sizeof(double));
 w= (double *)calloc(ncols,sizeof(double));
 V= (double *)calloc(ncols*ncols,sizeof(double));

 math_singular_value_decomposition(
  nrows,
  ncols,
  A,
  U,
  w,
  V
 );

 /*
 What we are looking for is the unit singular vector
 corresponding to the smallest singular value of A
 (should be the last column of V that is 4-1=3)
 */

 for ( i= 0 ; i< 4 ; i++ )
  X[i]= V[i*4+(4-1)];

 free(A);
 free(U);
 free(w);
 free(V);

}
