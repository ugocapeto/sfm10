#include "header.h"

int sfm_remove_spurious_matches_3(
 char **image_filename_arr,
 int camera_nbr,
 sfm_temp_struct *temp_arr,
 int temp_nbr
)

/*
The track is not valid if a camera shows up more than once
*/

{

 int *count_arr;
 int temp_ind;
 int camera_ind;
 int feat_ind;
 int err_flag;
 int camera1_ind;
 int camera2_ind;
 int match_ind;

 if ( camera_nbr > 0 )
  count_arr= (int *)calloc(camera_nbr,sizeof(int));

 for ( temp_ind= 0 ;
       temp_ind< temp_nbr ;
       temp_ind++ ) {
    camera_ind= temp_arr[temp_ind].camera_ind;
    feat_ind= temp_arr[temp_ind].feat_ind;
    camera1_ind= temp_arr[temp_ind].camera1_ind;
    camera2_ind= temp_arr[temp_ind].camera2_ind;
    match_ind= temp_arr[temp_ind].match_ind;
    count_arr[camera_ind]++;
 }

 err_flag= 0;

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    if ( count_arr[camera_ind] > 1 ) {
       err_flag= 1;
    }
 }

 if ( camera_nbr > 0 )
  free(count_arr);

 return err_flag;

}
