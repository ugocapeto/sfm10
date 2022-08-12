#include "header.h"

void sfm_check_initial_camera_pair(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 int camera1_ind,
 int camera2_ind,
 int *perr_flag
)

/*
Instead of computing homographies for each pair like in Bundler,
I am using the same method as Pierre Moulon in openMVG
*/

{

 char filename_match[80];
 match_struct *match_arr;
 int match_nbr;
 int iMin_inliers_count;
 double fRequired_min_angle;
 double fLimit_max_angle;
 int max_match_nbr;
 int max_camera1_ind;
 int max_camera2_ind;
 char image_filename1[80];
 int err_flag1;
 int *image1_arr;
 int width1;
 int height1;
 char image_filename2[80];
 int err_flag2;
 int *image2_arr;
 int width2;
 int height2;
 double P1[3*4];
 double P2[3*4];
 double avg_angle;
 int err_flag;
 int i;
 double vec[3];
 double dist2;
 double dist;
 int max_iter;
 int iter;
 FILE *fp;
 int err_flag3;

 /*
 The following thresholds come straight from openMVG
 */

 iMin_inliers_count= 100;
 fRequired_min_angle= 3.0f;
 fLimit_max_angle= 60.0f;

 /*
 Initialize to no error
 */

 err_flag= 0;

 /*
 Get corresponding image file name
 */

 strcpy(image_filename1,image_filename_arr[camera1_ind]);

 /*
 Load the corresponding image to get width and height
 */

 err_flag1= load_rgb_image(
  image_filename1,
  &image1_arr,
  &width1,
  &height1
 );

 if ( err_flag1 == 1 ) {  
    sfm10_error_handler("sfm_get_initial_camera_pair");
 }

 /*
 Free image1_arr
 */

 free(image1_arr);

 /*
 Get corresponding image file name
 */

 strcpy(image_filename2,image_filename_arr[camera2_ind]);

 /*
 Load the corresponding image to get width and height
 */

 err_flag2= load_rgb_image(
  image_filename2,
  &image2_arr,
  &width2,
  &height2
 );

 if ( err_flag2 == 1 ) {
    sfm10_error_handler("sfm_get_initial_camera_pair");
 }

 /*
 Free image2_arr 
 */

 free(image2_arr);

 /*
 Check that the dimensions of the 2 images are the same
 */

 if ( width2 != width1 ) {
    sfm10_error_handler("sfm_get_initial_camera_pair");
 }
 if ( height2 != height1 ) {
    sfm10_error_handler("sfm_get_initial_camera_pair");
 }

 /*
 Read the best matches
 (just to get the number of matches)
 */

 sprintf(filename_match,"best_match_%d_%d.sfm",
  camera1_ind,camera2_ind);

 sfm_read_matches(
  filename_match,
  &match_arr,
  &match_nbr
 );

 fprintf(stdout,"Number of best matches = %d\n",
  match_nbr);

 /*
 Free the best matches
 */

 if ( match_nbr > 0 )
  free(match_arr);

 /*
 Check if number of matches is sufficient
 */

 if ( match_nbr < iMin_inliers_count ) {
    fprintf(stdout,"Not enough best matches!\n");
    err_flag= 1;
    goto END;
 }

 /*
 Compute the relative pose of the 2 cameras
 */

 err_flag3= sfm_compute_relative_camera_pose(
  image_filename_arr,
  focal_length_arr,
  camera_nbr,
  camera1_ind,
  camera2_ind,
  P1,
  P2
 );

 if ( err_flag3 == 1 ) {
    fprintf(stdout,"More than one solution for which 3D points are in front of both cameras!\n");
    fprintf(stdout,"Change the initial camera pair or the focal length!\n");
    err_flag= 1;
    goto END;
 }

 END:

 (*perr_flag)= err_flag;

}
