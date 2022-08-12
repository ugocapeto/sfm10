#include "header.h"

void sfm_normalize_2dpoints(
 double *x3_arr,
 int x3_nbr,
 double T[3*3],
 double *normalized_x3_arr
)

/*
Normalize 2D homogeheous points so that
- their centroid is at the origin and
- their mean distance from the origin is sqrt(2)
*/

{

 int x_ind;
 double x;
 double y;
 double w;
 double c[2];
 double *new_x2_arr;
 double dist;
 double *dist_arr;
 double mean_dist;
 double scale;
 double x3[3];
 double normalized_x3[3];

 /*
 Check!
 */

 for ( x_ind= 0 ; x_ind< x3_nbr ; x_ind++ ) {
    x= x3_arr[3*x_ind+0];
    y= x3_arr[3*x_ind+1];
    w= x3_arr[3*x_ind+2];
    if ( w != 1 ) {
       sfm10_error_handler("sfm_normalize_2dpoints");
    }
 }

 if ( x3_nbr > 0 )
  new_x2_arr= (double *)calloc(x3_nbr,2*sizeof(double));

 if ( x3_nbr > 0 )
  dist_arr= (double *)calloc(x3_nbr,sizeof(double));

 /*
 Compute the mean
 */

 c[0]= 0;
 c[1]= 0;
 for ( x_ind= 0 ; x_ind< x3_nbr ; x_ind++ ) {
    x= x3_arr[3*x_ind+0];
    y= x3_arr[3*x_ind+1];
    w= x3_arr[3*x_ind+2];
    c[0]+= x;
    c[1]+= y;
 }
 c[0]/= (double)x3_nbr;
 c[1]/= (double)x3_nbr;

 for ( x_ind= 0 ; x_ind< x3_nbr ; x_ind++ ) {
    new_x2_arr[2*x_ind+0]= x3_arr[3*x_ind+0] - c[0];
    new_x2_arr[2*x_ind+1]= x3_arr[3*x_ind+1] - c[1];
 }

 for ( x_ind= 0 ; x_ind< x3_nbr ; x_ind++ ) {
    x= new_x2_arr[2*x_ind+0];
    y= new_x2_arr[2*x_ind+1];
    dist= sqrt(x*x+y*y);
    dist_arr[x_ind]= dist;
 }

 mean_dist= 0;
 for ( x_ind= 0 ; x_ind< x3_nbr ; x_ind++ ) {
    dist= dist_arr[x_ind]; 
    mean_dist+= dist;
 }
 mean_dist/= (double)x3_nbr;

 scale= sqrt(2)/mean_dist;

 T[0*3+0]= scale;
 T[0*3+1]= 0;
 T[0*3+2]= -scale*c[1-1];
 T[1*3+0]= 0;
 T[1*3+1]= scale;
 T[1*3+2]= -scale*c[2-1];
 T[2*3+0]= 0;
 T[2*3+1]= 0;
 T[2*3+2]= 1;

 for ( x_ind= 0 ; x_ind< x3_nbr ; x_ind++ ) {
    x3[0]= x3_arr[3*x_ind+0];
    x3[1]= x3_arr[3*x_ind+1];
    x3[2]= x3_arr[3*x_ind+2];

    math_matrix_vector_product(
     T,
     3,
     3,
     x3,
     3,
     normalized_x3 
    );

    normalized_x3_arr[3*x_ind+0]= normalized_x3[0];
    normalized_x3_arr[3*x_ind+1]= normalized_x3[1];
    normalized_x3_arr[3*x_ind+2]= normalized_x3[2];
 }

 if ( x3_nbr > 0 )
  free(new_x2_arr);

 if ( x3_nbr > 0 )
  free(dist_arr);

}
