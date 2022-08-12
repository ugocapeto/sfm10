#include "header.h"

double sfm_match_distance_squared(
 double *descr1_arr,
 int descr1_nbr,
 double *descr2_arr,
 int descr2_nbr
)

{

 double dist_squared;
 int descr1_ind;
 int descr2_ind;
 double diff;

 if ( descr2_nbr != descr1_nbr ) {
    sfm10_error_handler("sfm_match_distance_squared");
 }

 dist_squared= 0;
 for ( descr1_ind= 0 ; descr1_ind< descr1_nbr ; descr1_ind++ ) {
    descr2_ind= descr1_ind;
    diff= descr1_arr[descr1_ind]-descr2_arr[descr2_ind];
    dist_squared+= diff*diff;
 }

 return dist_squared;

}
