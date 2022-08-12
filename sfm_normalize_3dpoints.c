#include "header.h"

void sfm_normalize_3dpoints(
 double *x4_arr,
 int x4_nbr,
 double T[4*4],
 double *normalized_x4_arr
)

/*
Normalize 3D homogeheous points so that
- their centroid is at the origin and
- their mean distance from the origin is sqrt(3)
*/

{

 int x_ind;
 double x;
 double y;
 double z;
 double t;
 double c[3];
 double *new_x3_arr;
 double dist;
 double *dist_arr;
 double mean_dist;
 double scale;
 double x4[4];
 double normalized_x4[4];

 /*
 Check!
 */

 for ( x_ind= 0 ; x_ind< x4_nbr ; x_ind++ ) {
    x= x4_arr[4*x_ind+0];
    y= x4_arr[4*x_ind+1];
    z= x4_arr[4*x_ind+2];
    t= x4_arr[4*x_ind+3];
    if ( t != 1 ) {
       sfm10_error_handler("sfm_normalize_3dpoints");
    }
 }

 if ( x4_nbr > 0 )
  new_x3_arr= (double *)calloc(x4_nbr,3*sizeof(double));

 if ( x4_nbr > 0 )
  dist_arr= (double *)calloc(x4_nbr,sizeof(double));

 /*
 Compute the mean
 */

 c[0]= 0;
 c[1]= 0;
 c[2]= 0;
 for ( x_ind= 0 ; x_ind< x4_nbr ; x_ind++ ) {
    x= x4_arr[4*x_ind+0];
    y= x4_arr[4*x_ind+1];
    z= x4_arr[4*x_ind+2];
    t= x4_arr[4*x_ind+3];
    c[0]+= x;
    c[1]+= y;
    c[2]+= z;
 }
 c[0]/= (double)x4_nbr;
 c[1]/= (double)x4_nbr;
 c[2]/= (double)x4_nbr;

 for ( x_ind= 0 ; x_ind< x4_nbr ; x_ind++ ) {
    new_x3_arr[3*x_ind+0]= x4_arr[4*x_ind+0] - c[0];
    new_x3_arr[3*x_ind+1]= x4_arr[4*x_ind+1] - c[1];
    new_x3_arr[3*x_ind+2]= x4_arr[4*x_ind+2] - c[2];
 }

 for ( x_ind= 0 ; x_ind< x4_nbr ; x_ind++ ) {
    x= new_x3_arr[3*x_ind+0];
    y= new_x3_arr[3*x_ind+1];
    z= new_x3_arr[3*x_ind+2];
    dist= sqrt(x*x+y*y+z*z);
    dist_arr[x_ind]= dist;
 }

 mean_dist= 0;
 for ( x_ind= 0 ; x_ind< x4_nbr ; x_ind++ ) {
    dist= dist_arr[x_ind]; 
    mean_dist+= dist;
 }
 mean_dist/= (double)x4_nbr;

 scale= sqrt(3)/mean_dist;

 T[0*4+0]= scale;
 T[0*4+1]= 0;
 T[0*4+2]= 0;
 T[0*4+3]= -scale*c[1-1];
 T[1*4+0]= 0;
 T[1*4+1]= scale;
 T[1*4+2]= 0;
 T[1*4+3]= -scale*c[2-1];
 T[2*4+0]= 0;
 T[2*4+1]= 0;
 T[2*4+2]= scale;
 T[2*4+3]= -scale*c[3-1];
 T[3*4+0]= 0;
 T[3*4+1]= 0;
 T[3*4+2]= 0;
 T[3*4+3]= 1;

 for ( x_ind= 0 ; x_ind< x4_nbr ; x_ind++ ) {
    x4[0]= x4_arr[4*x_ind+0];
    x4[1]= x4_arr[4*x_ind+1];
    x4[2]= x4_arr[4*x_ind+2];
    x4[3]= x4_arr[4*x_ind+3];

    math_matrix_vector_product(
     T,
     4,
     4,
     x4,
     4,
     normalized_x4 
    );

    normalized_x4_arr[4*x_ind+0]= normalized_x4[0];
    normalized_x4_arr[4*x_ind+1]= normalized_x4[1];
    normalized_x4_arr[4*x_ind+2]= normalized_x4[2];
    normalized_x4_arr[4*x_ind+3]= normalized_x4[3];
 }

 if ( x4_nbr > 0 )
  free(new_x3_arr);

 if ( x4_nbr > 0 )
  free(dist_arr);

}
