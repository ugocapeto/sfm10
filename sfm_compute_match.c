#include "header.h"

void sfm_compute_match(
 sfm_feat_struct *feat1_arr,
 int feat1_ind,
 sfm_feat_struct *feat2_arr,
 int feat2_nbr,
 int *pfeat2_ind
)

{

 double *descr1_arr;
 int descr1_nbr;
 double *descr2_arr;
 int descr2_nbr;
 double x1;
 double y1;
 double min_dist_squared;
 double sec_min_dist_squared;
 int min_feat2_ind;
 int sec_min_feat2_ind;
 int feat2_ind;
 double x2;
 double y2;
 double dist_squared;
 double ratio;
 double sq_dist_ratio_thresh;

 x1= feat1_arr[feat1_ind].x;
 y1= feat1_arr[feat1_ind].y;
 descr1_arr= feat1_arr[feat1_ind].descr_arr;
 descr1_nbr= feat1_arr[feat1_ind].descr_nbr;

 /*
 Go through the features in feat2_arr
 to find the 2 closest features to the input feature
 1 is the closest
 2 is the next closest
 */

 min_dist_squared= 1.0e32;
 sec_min_dist_squared= 1.0e32;
 min_feat2_ind=-1;
 sec_min_feat2_ind=-1;

 for ( feat2_ind= 0 ;
       feat2_ind< feat2_nbr ;
       feat2_ind++ ) {
    x2= feat2_arr[feat2_ind].x;
    y2= feat2_arr[feat2_ind].y;
    descr2_arr= feat2_arr[feat2_ind].descr_arr;
    descr2_nbr= feat2_arr[feat2_ind].descr_nbr;

    /*
    Compute distance squared between the 2 features
    */

    dist_squared= sfm_match_distance_squared(
     descr1_arr,
     descr1_nbr,
     descr2_arr,
     descr2_nbr
    );

    if ( dist_squared < min_dist_squared ) {
       sec_min_dist_squared= min_dist_squared;
       sec_min_feat2_ind= min_feat2_ind;
       min_dist_squared= dist_squared;
       min_feat2_ind= feat2_ind;
    }
    else if ( dist_squared < sec_min_dist_squared ) {
       sec_min_dist_squared= dist_squared;
       sec_min_feat2_ind= feat2_ind;
    }
 }

 /*
 Compute the ratio of the distance squared
 */

 ratio= min_dist_squared/sec_min_dist_squared;

 sq_dist_ratio_thresh= 0.49;

 if ( ratio < sq_dist_ratio_thresh )
  feat2_ind= min_feat2_ind;
 else
  feat2_ind=-1;

 (*pfeat2_ind)= feat2_ind;

}
