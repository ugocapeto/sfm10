#include "header.h"

int sfm_compute_relative_camera_pose(
 char **image_filename_arr,
 double *focal_length_arr,
 int camera_nbr,
 int camera1_ind,
 int camera2_ind,
 double P1[3*4],
 double P2[3*4]
)

/*
This is the same code as sfm_reconstruct_3d_from_2cameras()
minus sfm_compute_threedpoints()
*/

/*
Note that the camera matrices P1 and P2 are output
*/

{

 char image_filename1[80];
 char image_filename2[80];
 int width;
 int height;
 int *image1_arr;
 int *image2_arr;
 double F[3*3];
 char filename_match[80];
 int i;
 int j;
 int match_nbr;
 match_struct *match_arr;
 double K1[3*3];
 double K2[3*3];
 double E[3*3];
 double P24[4][3*4];
 match_struct *match_normalized_arr;
 int match_ind;
 double R1[3*3];
 double t1[3];
 double K1R1[3*3];
 double K1t1[3];
 double R2[3*3];
 double t2[3];
 double K2R2[3*3];
 double K2t2[3];
 int err_flag1;
 int err_flag2;
 int width1;
 int height1;
 int width2;
 int height2;
 char filename_F[80];
 double focal_length1;
 double focal_length2;
 int err_flag;
 double Eb[3*3];
 double P24b[4][3*4];

 /*
 Get filename for image 1
 */

 strcpy(image_filename1,image_filename_arr[camera1_ind]);

 /*
 Load image 1
 */

 err_flag1= load_rgb_image(
  image_filename1,
  &image1_arr,
  &width1,
  &height1
 );

 /*
 Get filename for image 2
 */

 strcpy(image_filename2,image_filename_arr[camera2_ind]);

 /*
 Load image 2
 */

 err_flag2= load_rgb_image(
  image_filename2,
  &image2_arr,
  &width2,
  &height2
 );

 /*
 Check that the 2 images have the same width and height
 */

 if ( width2 != width1 ) {
    sfm10_error_handler("sfm_reconstruct_3d_from_2cameras");
 }
 if ( height2 != height1 ) {
    sfm10_error_handler("sfm_reconstruct_3d_from_2cameras");
 }

 /*
 Set the common width and height
 */

 width= width1;
 height= height1;

 /*
 Read the matches
 */

 sprintf(filename_match,"best_match_%d_%d.sfm",
  camera1_ind,camera2_ind);

 sfm_read_matches(
  filename_match,
  &match_arr,
  &match_nbr
 );

 /*
 Read the fundamental matrix F
 */

 sprintf(filename_F,"F_%d_%d.sfm",
  camera1_ind,camera2_ind);

 sfm_read_F(
  filename_F,
  F
 );

 /*
 Get the focal length for the 1st image
 */

 focal_length1= focal_length_arr[camera1_ind];

 /*
 Compute the calibration matrix for the 1st image
 */

 /*
 It is assumed that the origin of the image coordinate system
 is at top-left with y axis going down
 It is also assumed that the y axis of the camera coordinate system points downward
 like in VisualSFM
 */

 K1[0*3+0]= focal_length1;
 K1[0*3+1]= 0;
 K1[0*3+2]= (double)width/2;

 K1[1*3+0]= 0;
 K1[1*3+1]= focal_length1;
 /*
 K1[1*3+1]=-focal_length1;
 */
 K1[1*3+2]= (double)height/2;

 K1[2*3+0]= 0;
 K1[2*3+1]= 0;
 K1[2*3+2]= 1;

 /*
 Get the focal length for the 2nd image
 */

 focal_length2= focal_length_arr[camera2_ind];

 /*
 Compute the calibration matrix for the 2nd image
 */

 /*
 It is assumed that the origin of the image coordinate system
 is at top-left with y axis going down
 It is also assumed that the y axis of the camera coordinate system points downward
 like in VisualSFM
 */

 K2[0*3+0]= focal_length2;
 K2[0*3+1]= 0;
 K2[0*3+2]= (double)width/2;

 K2[1*3+0]= 0;
 K2[1*3+1]= focal_length2;
 /*
 K2[1*3+1]=-focal_length2;
 */
 K2[1*3+2]= (double)height/2;

 K2[2*3+0]= 0;
 K2[2*3+1]= 0;
 K2[2*3+2]= 1;

 /*
 Normalize the image coordinates
 */

 if ( match_nbr > 0 )
  match_normalized_arr= (match_struct *)calloc(match_nbr,sizeof(match_struct));   

 for ( match_ind= 0 ; match_ind< match_nbr ; match_ind++ )
  match_normalized_arr[match_ind]= match_arr[match_ind];

 normalize_coordinates(
  K1,
  K2,
  match_normalized_arr,
  match_nbr
 );

 /*
 Compute the 1st camera matrix
 P1 is of the form [R | t] (normalized camera matrix),
 not K[R | t] since we are using normalized coordinates
 */

 compute_1st_camera_matrix(
  P1
 );

 /* 
 Compute the essential matrix
 */

 compute_essential_matrix(
  F,
  K1,
  K2,
  E
 );

 /*
 Compute the 4 possible 2nd camera matrices
 using Hartley's method
 */

 compute_2nd_camera_matrix_from_essential_matrix(
  E,
  P24
 );

 /*
 Let's consider -E instead of E,
 we should get the same possible 2nd camera matrices
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       Eb[3*i+j]=-E[3*i+j];
    }
 }

 compute_2nd_camera_matrix_from_essential_matrix(
  Eb,
  P24b
 );

 /*
 Compute the 4 possible 2nd camera matrices
 using Horn's method
 */

 /*
 compute_2nd_camera_matrix_from_essential_matrix_horn(
  E,
  P24
 );
 */

 /*
 Select the correct 2nd camera matrix
 using the normalized coordinates
 P2 is of the form [R | t] (normalized camera matrix),
 not K[R | t] since we are using normalized coordinates
 */

 err_flag= select_correct_2nd_camera_matrix(
  P1,
  P24,
  match_normalized_arr,
  match_arr,
  match_nbr,
  P2
 );

 /*
 I think it would be a good idea to denormalize
 */

 /*
 Put P1 in the form K[R | t]
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R1[i*3+j]= P1[i*4+j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    t1[i]= P1[i*4+j];
 }

 math_matrix_matrix_product(
  K1,
  3,
  3,
  R1,
  3,
  3,
  K1R1
 );

 math_matrix_vector_product(
  K1,
  3,
  3,
  t1,
  3,
  K1t1 
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P1[i*4+j]= K1R1[i*3+j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P1[i*4+j]= K1t1[i];
 }

 /*
 Put P2 in the form K[R | t]
 */

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R2[i*3+j]= P2[i*4+j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    t2[i]= P2[i*4+j];
 }

 math_matrix_matrix_product(
  K2,
  3,
  3,
  R2,
  3,
  3,
  K2R2
 );

 math_matrix_vector_product(
  K2,
  3,
  3,
  t2,
  3,
  K2t2
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       P2[i*4+j]= K2R2[i*3+j];
    }
 }

 for ( i= 0 ; i< 3 ; i++ ) {
    j= 3;
    P2[i*4+j]= K2t2[i];
 }

 /*
 Free memory
 */

 free(image1_arr);

 free(image2_arr);

 if ( match_nbr > 0 )
  free(match_arr);

 if ( match_nbr > 0 )
  free(match_normalized_arr);

 return err_flag;

}
