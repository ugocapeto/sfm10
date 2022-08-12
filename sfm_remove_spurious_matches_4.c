#include "header.h"

void sfm_remove_spurious_matches_4(
 char **image_filename_arr,
 int camera_nbr,
 match_struct ***cam_match_arr,
 int **cam_match_nbr,
 sfm_temp_struct *temp_arr,
 int temp_nbr
)

{

 int temp_ind;
 int camera_ind;
 int feat_ind;
 int camera1_ind;
 int camera2_ind;
 int match_ind;
 match_struct *match_arr;
 int match_nbr;

 for ( temp_ind= 0 ;
       temp_ind< temp_nbr ;
       temp_ind++ ) {
    camera_ind= temp_arr[temp_ind].camera_ind;
    feat_ind= temp_arr[temp_ind].feat_ind;
    camera1_ind= temp_arr[temp_ind].camera1_ind;
    camera2_ind= temp_arr[temp_ind].camera2_ind;
    match_ind= temp_arr[temp_ind].match_ind;

    /*
    Remove the match (virtually)
    */

    match_arr= cam_match_arr[camera1_ind][camera2_ind];
    match_nbr= cam_match_nbr[camera1_ind][camera2_ind];

    match_arr[match_ind].feat1_ind=-1;
    match_arr[match_ind].x1= 0;
    match_arr[match_ind].y1= 0;
    match_arr[match_ind].feat2_ind=-1;
    match_arr[match_ind].x2= 0;
    match_arr[match_ind].y2= 0;
 }

}
